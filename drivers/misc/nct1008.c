/*
 * drivers/misc/nct1008.c
 *
 * Driver for NCT1008, temperature monitoring device from ON Semiconductors
 *
 * Copyright (c) 2010, NVIDIA Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include <linux/interrupt.h>
#include <linux/mutex.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/gpio.h>

#include <linux/nct1008.h>

#define DRIVER_NAME "nct1008"

/* Register Addresses */
#define LOCAL_TEMP_RD			0x00
#define EXT_HI_TEMP_RD			0x01
#define EXT_LO_TEMP_RD			0x10
#define STATUS_RD			0x02
#define CONFIG_RD			0x03

#define CONFIG_WR			0x09
#define CONV_RATE_WR			0x0A
#define LOCAL_TEMP_HI_LIMIT_WR		0x0B
#define EXT_TEMP_HI_LIMIT_HI_BYTE	0x0D
#define OFFSET_WR			0x11
#define EXT_THERM_LIMIT_WR		0x19
#define LOCAL_THERM_LIMIT_WR		0x20
#define THERM_HYSTERESIS_WR		0x21

/* Configuration Register Bits */
#define EXTENDED_RANGE_BIT		(0x1 << 2)
#define THERM2_BIT			(0x1 << 5)
#define STANDBY_BIT			(0x1 << 6)

/* Max Temperature Measurements */
#define EXTENDED_RANGE_OFFSET		64U
#define STANDARD_RANGE_MAX		127U
#define EXTENDED_RANGE_MAX		(150U + EXTENDED_RANGE_OFFSET)

struct nct1008_data {
	struct work_struct work;
	struct i2c_client *client;
	struct mutex mutex;
	u8 config;
	void (*alarm_fn)(bool raised);
};

static ssize_t nct1008_show_temp(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	signed int temp_value = 0;
	u8 data = 0;

	if (!dev || !buf || !attr)
		return -EINVAL;

	data = i2c_smbus_read_byte_data(client, LOCAL_TEMP_RD);
	if (data < 0) {
		dev_err(&client->dev, "%s: failed to read "
			"temperature\n", __func__);
		return -EINVAL;
	}

	temp_value = (signed int)data;
	return sprintf(buf, "%d\n", temp_value);
}

static ssize_t nct1008_show_ext_temp(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	signed int temp_value = 0;
	u8 data = 0;

	if (!dev || !buf || !attr)
		return -EINVAL;

	data = i2c_smbus_read_byte_data(client, EXT_HI_TEMP_RD);
	if (data < 0) {
		dev_err(&client->dev, "%s: failed to read "
			"ext_temperature\n", __func__);
		return -EINVAL;
	}

	temp_value = (signed int)data;

	data = i2c_smbus_read_byte_data(client, EXT_LO_TEMP_RD);

	return sprintf(buf, "%d.%d\n", temp_value, (25 * (data >> 6)));
}


static ssize_t nct1008_store_conversion_rate(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	unsigned long val;

	if (!dev || !buf || !attr)
		return -EINVAL;

	strict_strtoul(buf, 10, &val);
	if (val > 0xff)
		return -EINVAL;

	if (i2c_smbus_write_byte_data(client, CONV_RATE_WR, val) < 0)
		return -EINVAL;

	return count;
}


static DEVICE_ATTR(temperature, S_IRUGO, nct1008_show_temp, NULL);
static DEVICE_ATTR(ext_temperature, S_IRUGO, nct1008_show_ext_temp, NULL);

static DEVICE_ATTR(conversion_rate, 0664, NULL, nct1008_store_conversion_rate);


static struct attribute *nct1008_attributes[] = {
	&dev_attr_temperature.attr,
	&dev_attr_ext_temperature.attr,
	
	&dev_attr_conversion_rate.attr,
	
	NULL
};

static const struct attribute_group nct1008_attr_group = {
	.attrs = nct1008_attributes,
};

static void nct1008_enable(struct i2c_client *client)
{
	struct nct1008_data *data = i2c_get_clientdata(client);

	i2c_smbus_write_byte_data(client, CONFIG_WR,
				  data->config & ~STANDBY_BIT);
}

static void nct1008_disable(struct i2c_client *client)
{
	struct nct1008_data *data = i2c_get_clientdata(client);

	i2c_smbus_write_byte_data(client, CONFIG_WR,
				  data->config | STANDBY_BIT);
}


static void nct1008_work_func(struct work_struct *work)
{
	struct nct1008_data *data = container_of(work, struct nct1008_data, work);
	int irq = data->client->irq;

	mutex_lock(&data->mutex);

	if (data->alarm_fn) {
		/* Therm2 line is active low */
		data->alarm_fn(!gpio_get_value(irq_to_gpio(irq)));
	}

	mutex_unlock(&data->mutex);
}

static irqreturn_t nct1008_irq(int irq, void *dev_id)
{
	struct nct1008_data *data = dev_id;
	schedule_work(&data->work);

	return IRQ_HANDLED;
}

static inline u8 value_to_temperature(bool extended, u8 value)
{
	return (extended ? (u8)(value - EXTENDED_RANGE_OFFSET) : value);
}

static inline u8 temperature_to_value(bool extended, u8 temp)
{
	return (extended ? (u8)(temp + EXTENDED_RANGE_OFFSET) : temp);
}

static int __devinit nct1008_configure_sensor(struct nct1008_data* data)
{
	struct i2c_client *client           = data->client;
	struct nct1008_platform_data *pdata = client->dev.platform_data;
	u8 value;
	int err;

	if (!pdata || !pdata->supported_hwrev)
		return -ENODEV;

	/*
	 * Initial Configuration - device is placed in standby and
	 * ALERT/THERM2 pin is configured as THERM2
	 */
	data->config = value = pdata->ext_range ?
		(STANDBY_BIT | THERM2_BIT | EXTENDED_RANGE_BIT) :
		(STANDBY_BIT | THERM2_BIT);

	err = i2c_smbus_write_byte_data(client, CONFIG_WR, value);
	if (err < 0)
		goto error;

	/* Temperature conversion rate */
	err = i2c_smbus_write_byte_data(client, CONV_RATE_WR, pdata->conv_rate);
	if (err < 0)
		goto error;

	/* External temperature h/w shutdown limit */
	value = temperature_to_value(pdata->ext_range, pdata->shutdown_ext_limit);
	err = i2c_smbus_write_byte_data(client, EXT_THERM_LIMIT_WR, value);
	if (err < 0)
		goto error;

	/* Local temperature h/w shutdown limit */
	value = temperature_to_value(pdata->ext_range, pdata->shutdown_local_limit);
	err = i2c_smbus_write_byte_data(client, LOCAL_THERM_LIMIT_WR, value);
	if (err < 0)
		goto error;

	/* External Temperature Throttling limit */
	value = temperature_to_value(pdata->ext_range, pdata->throttling_ext_limit);
	err = i2c_smbus_write_byte_data(client, EXT_TEMP_HI_LIMIT_HI_BYTE, value);
	if (err < 0)
		goto error;

	/* Local Temperature Throttling limit */
	value = pdata->ext_range ? EXTENDED_RANGE_MAX : STANDARD_RANGE_MAX;
	err = i2c_smbus_write_byte_data(client, LOCAL_TEMP_HI_LIMIT_WR, value);
	if (err < 0)
		goto error;

	/* Remote channel offset */
	err = i2c_smbus_write_byte_data(client, OFFSET_WR, pdata->offset);
	if (err < 0)
		goto error;

	/* THERM hysteresis */
	err = i2c_smbus_write_byte_data(client, THERM_HYSTERESIS_WR, pdata->hysteresis);
	if (err < 0)
		goto error;

	data->alarm_fn = pdata->alarm_fn;
	return 0;
error:
	return err;
}

static int __devinit nct1008_configure_irq(struct nct1008_data *data)
{
	INIT_WORK(&data->work, nct1008_work_func);

	return request_irq(data->client->irq, nct1008_irq, IRQF_TRIGGER_RISING |
				IRQF_TRIGGER_FALLING, DRIVER_NAME, data);
}



static struct nct1008_data *nct1008_data_p;
void luna_tmon_callback(int up)
{
	if (up)
	{
		nct1008_configure_sensor(nct1008_data_p);
		nct1008_enable(nct1008_data_p->client);
		schedule_work(&nct1008_data_p->work);
	}
}
EXPORT_SYMBOL(luna_tmon_callback);


static int __devinit nct1008_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct nct1008_data *data;
	int err;

	data = kzalloc(sizeof(struct nct1008_data), GFP_KERNEL);

	if (!data)
		return -ENOMEM;

	
	
	nct1008_data_p = data;
	

	data->client = client;
	i2c_set_clientdata(client, data);
	mutex_init(&data->mutex);

	err = nct1008_configure_sensor(data);	/* sensor is in standby */
	if (err < 0)
		goto error;

	err = nct1008_configure_irq(data);
	if (err < 0)
		goto error;

	/* register sysfs hooks */
	err = sysfs_create_group(&client->dev.kobj, &nct1008_attr_group);
	if (err < 0)
		goto error;

	dev_info(&client->dev, "%s: initialized\n", __func__);

	nct1008_enable(client);		/* sensor is running */

	schedule_work(&data->work);		/* check initial state */

	return 0;

error:
	kfree(data);
	return err;
}

static int __devexit nct1008_remove(struct i2c_client *client)
{
	struct nct1008_data *data = i2c_get_clientdata(client);

	free_irq(data->client->irq, data);
	cancel_work_sync(&data->work);
	sysfs_remove_group(&client->dev.kobj, &nct1008_attr_group);
	kfree(data);

	return 0;
}

#ifdef CONFIG_PM
static int nct1008_suspend(struct i2c_client *client, pm_message_t state)
{
	disable_irq(client->irq);
	nct1008_disable(client);

	return 0;
}

static int nct1008_resume(struct i2c_client *client)
{
	struct nct1008_data *data = i2c_get_clientdata(client);

	nct1008_enable(client);
	enable_irq(client->irq);
	schedule_work(&data->work);

	return 0;
}
#endif

static const struct i2c_device_id nct1008_id[] = {
	{ DRIVER_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, nct1008_id);

static struct i2c_driver nct1008_driver = {
	.driver = {
		.name	= DRIVER_NAME,
	},
	.probe		= nct1008_probe,
	.remove		= __devexit_p(nct1008_remove),
	.id_table	= nct1008_id,
#ifdef CONFIG_PM
	.suspend	= nct1008_suspend,
	.resume		= nct1008_resume,
#endif
};

static int __init nct1008_init(void)
{
	return i2c_add_driver(&nct1008_driver);
}

static void __exit nct1008_exit(void)
{
	i2c_del_driver(&nct1008_driver);
}

MODULE_DESCRIPTION("Temperature sensor driver for OnSemi NCT1008");
MODULE_LICENSE("GPL");

module_init (nct1008_init);
module_exit (nct1008_exit);
