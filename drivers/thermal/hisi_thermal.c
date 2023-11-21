/*
 * Hisilicon thermal sensor driver
 *
 * Copyright (c) 2014-2015 Hisilicon Limited.
 * Copyright (c) 2014-2015 Linaro Limited.
 *
 * Xinwei Kong <kong.kongxinwei@hisilicon.com>
 * Leo Yan <leo.yan@linaro.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/cpufreq.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
<<<<<<< HEAD
#include <linux/of_device.h>

#include "thermal_core.h"

#define HI6220_TEMP0_LAG			(0x0)
#define HI6220_TEMP0_TH				(0x4)
#define HI6220_TEMP0_RST_TH			(0x8)
#define HI6220_TEMP0_CFG			(0xC)
#define HI6220_TEMP0_CFG_SS_MSK			(0xF000)
#define HI6220_TEMP0_CFG_HDAK_MSK		(0x30)
#define HI6220_TEMP0_EN				(0x10)
#define HI6220_TEMP0_INT_EN			(0x14)
#define HI6220_TEMP0_INT_CLR			(0x18)
#define HI6220_TEMP0_RST_MSK			(0x1C)
#define HI6220_TEMP0_VALUE			(0x28)

#define HI3660_OFFSET(chan)		((chan) * 0x40)
#define HI3660_TEMP(chan)		(HI3660_OFFSET(chan) + 0x1C)
#define HI3660_TH(chan)			(HI3660_OFFSET(chan) + 0x20)
#define HI3660_LAG(chan)		(HI3660_OFFSET(chan) + 0x28)
#define HI3660_INT_EN(chan)		(HI3660_OFFSET(chan) + 0x2C)
#define HI3660_INT_CLR(chan)		(HI3660_OFFSET(chan) + 0x30)

#define HI6220_TEMP_BASE			(-60000)
#define HI6220_TEMP_RESET			(100000)
#define HI6220_TEMP_STEP			(785)
#define HI6220_TEMP_LAG				(3500)

#define HI3660_TEMP_BASE		(-63780)
#define HI3660_TEMP_STEP		(205)
#define HI3660_TEMP_LAG			(4000)

#define HI6220_DEFAULT_SENSOR		2
#define HI3660_DEFAULT_SENSOR		1

struct hisi_thermal_sensor {
	struct thermal_zone_device *tzd;
=======

#include "thermal_core.h"

#define TEMP0_TH			(0x4)
#define TEMP0_RST_TH			(0x8)
#define TEMP0_CFG			(0xC)
#define TEMP0_EN			(0x10)
#define TEMP0_INT_EN			(0x14)
#define TEMP0_INT_CLR			(0x18)
#define TEMP0_RST_MSK			(0x1C)
#define TEMP0_VALUE			(0x28)

#define HISI_TEMP_BASE			(-60000)
#define HISI_TEMP_RESET			(100000)
#define HISI_TEMP_STEP			(784)

#define HISI_MAX_SENSORS		4

struct hisi_thermal_sensor {
	struct hisi_thermal_data *thermal;
	struct thermal_zone_device *tzd;

	long sensor_temp;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	uint32_t id;
	uint32_t thres_temp;
};

struct hisi_thermal_data {
<<<<<<< HEAD
	int (*get_temp)(struct hisi_thermal_data *data);
	int (*enable_sensor)(struct hisi_thermal_data *data);
	int (*disable_sensor)(struct hisi_thermal_data *data);
	int (*irq_handler)(struct hisi_thermal_data *data);
	struct platform_device *pdev;
	struct clk *clk;
	struct hisi_thermal_sensor sensor;
	void __iomem *regs;
	int irq;
=======
	struct mutex thermal_lock;    /* protects register data */
	struct platform_device *pdev;
	struct clk *clk;
	struct hisi_thermal_sensor sensors[HISI_MAX_SENSORS];

	int irq, irq_bind_sensor;
	bool irq_enabled;

	void __iomem *regs;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
};

/*
 * The temperature computation on the tsensor is as follow:
 *	Unit: millidegree Celsius
<<<<<<< HEAD
 *	Step: 200/255 (0.7843)
 *	Temperature base: -60°C
 *
 * The register is programmed in temperature steps, every step is 785
=======
 *	Step: 255/200 (0.7843)
 *	Temperature base: -60°C
 *
 * The register is programmed in temperature steps, every step is 784
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
 * millidegree and begins at -60 000 m°C
 *
 * The temperature from the steps:
 *
<<<<<<< HEAD
 *	Temp = TempBase + (steps x 785)
 *
 * and the steps from the temperature:
 *
 *	steps = (Temp - TempBase) / 785
 *
 */
static inline int hi6220_thermal_step_to_temp(int step)
{
	return HI6220_TEMP_BASE + (step * HI6220_TEMP_STEP);
}

static inline int hi6220_thermal_temp_to_step(int temp)
{
	return DIV_ROUND_UP(temp - HI6220_TEMP_BASE, HI6220_TEMP_STEP);
}

/*
 * for Hi3660,
 *	Step: 189/922 (0.205)
 *	Temperature base: -63.780°C
 *
 * The register is programmed in temperature steps, every step is 205
 * millidegree and begins at -63 780 m°C
 */
static inline int hi3660_thermal_step_to_temp(int step)
{
	return HI3660_TEMP_BASE + step * HI3660_TEMP_STEP;
}

static inline int hi3660_thermal_temp_to_step(int temp)
{
	return DIV_ROUND_UP(temp - HI3660_TEMP_BASE, HI3660_TEMP_STEP);
}

/*
 * The lag register contains 5 bits encoding the temperature in steps.
 *
 * Each time the temperature crosses the threshold boundary, an
 * interrupt is raised. It could be when the temperature is going
 * above the threshold or below. However, if the temperature is
 * fluctuating around this value due to the load, we can receive
 * several interrupts which may not desired.
 *
 * We can setup a temperature representing the delta between the
 * threshold and the current temperature when the temperature is
 * decreasing.
 *
 * For instance: the lag register is 5°C, the threshold is 65°C, when
 * the temperature reaches 65°C an interrupt is raised and when the
 * temperature decrease to 65°C - 5°C another interrupt is raised.
 *
 * A very short lag can lead to an interrupt storm, a long lag
 * increase the latency to react to the temperature changes.  In our
 * case, that is not really a problem as we are polling the
 * temperature.
 *
 * [0:4] : lag register
 *
 * The temperature is coded in steps, cf. HI6220_TEMP_STEP.
 *
 * Min : 0x00 :  0.0 °C
 * Max : 0x1F : 24.3 °C
 *
 * The 'value' parameter is in milliCelsius.
 */
static inline void hi6220_thermal_set_lag(void __iomem *addr, int value)
{
	writel(DIV_ROUND_UP(value, HI6220_TEMP_STEP) & 0x1F,
			addr + HI6220_TEMP0_LAG);
}

static inline void hi6220_thermal_alarm_clear(void __iomem *addr, int value)
{
	writel(value, addr + HI6220_TEMP0_INT_CLR);
}

static inline void hi6220_thermal_alarm_enable(void __iomem *addr, int value)
{
	writel(value, addr + HI6220_TEMP0_INT_EN);
}

static inline void hi6220_thermal_alarm_set(void __iomem *addr, int temp)
{
	writel(hi6220_thermal_temp_to_step(temp) | 0x0FFFFFF00,
	       addr + HI6220_TEMP0_TH);
}

static inline void hi6220_thermal_reset_set(void __iomem *addr, int temp)
{
	writel(hi6220_thermal_temp_to_step(temp), addr + HI6220_TEMP0_RST_TH);
}

static inline void hi6220_thermal_reset_enable(void __iomem *addr, int value)
{
	writel(value, addr + HI6220_TEMP0_RST_MSK);
}

static inline void hi6220_thermal_enable(void __iomem *addr, int value)
{
	writel(value, addr + HI6220_TEMP0_EN);
}

static inline int hi6220_thermal_get_temperature(void __iomem *addr)
{
	return hi6220_thermal_step_to_temp(readl(addr + HI6220_TEMP0_VALUE));
}

/*
 * [0:6] lag register
 *
 * The temperature is coded in steps, cf. HI3660_TEMP_STEP.
 *
 * Min : 0x00 :  0.0 °C
 * Max : 0x7F : 26.0 °C
 *
 */
static inline void hi3660_thermal_set_lag(void __iomem *addr,
					  int id, int value)
{
	writel(DIV_ROUND_UP(value, HI3660_TEMP_STEP) & 0x7F,
			addr + HI3660_LAG(id));
}

static inline void hi3660_thermal_alarm_clear(void __iomem *addr,
					      int id, int value)
{
	writel(value, addr + HI3660_INT_CLR(id));
}

static inline void hi3660_thermal_alarm_enable(void __iomem *addr,
					       int id, int value)
{
	writel(value, addr + HI3660_INT_EN(id));
}

static inline void hi3660_thermal_alarm_set(void __iomem *addr,
					    int id, int value)
{
	writel(value, addr + HI3660_TH(id));
}

static inline int hi3660_thermal_get_temperature(void __iomem *addr, int id)
{
	return hi3660_thermal_step_to_temp(readl(addr + HI3660_TEMP(id)));
}

/*
 * Temperature configuration register - Sensor selection
 *
 * Bits [19:12]
 *
 * 0x0: local sensor (default)
 * 0x1: remote sensor 1 (ACPU cluster 1)
 * 0x2: remote sensor 2 (ACPU cluster 0)
 * 0x3: remote sensor 3 (G3D)
 */
static inline void hi6220_thermal_sensor_select(void __iomem *addr, int sensor)
{
	writel((readl(addr + HI6220_TEMP0_CFG) & ~HI6220_TEMP0_CFG_SS_MSK) |
	       (sensor << 12), addr + HI6220_TEMP0_CFG);
}

/*
 * Temperature configuration register - Hdak conversion polling interval
 *
 * Bits [5:4]
 *
 * 0x0 :   0.768 ms
 * 0x1 :   6.144 ms
 * 0x2 :  49.152 ms
 * 0x3 : 393.216 ms
 */
static inline void hi6220_thermal_hdak_set(void __iomem *addr, int value)
{
	writel((readl(addr + HI6220_TEMP0_CFG) & ~HI6220_TEMP0_CFG_HDAK_MSK) |
	       (value << 4), addr + HI6220_TEMP0_CFG);
}

static int hi6220_thermal_irq_handler(struct hisi_thermal_data *data)
{
	hi6220_thermal_alarm_clear(data->regs, 1);
	return 0;
}

static int hi3660_thermal_irq_handler(struct hisi_thermal_data *data)
{
	hi3660_thermal_alarm_clear(data->regs, data->sensor.id, 1);
	return 0;
}

static int hi6220_thermal_get_temp(struct hisi_thermal_data *data)
{
	return hi6220_thermal_get_temperature(data->regs);
}

static int hi3660_thermal_get_temp(struct hisi_thermal_data *data)
{
	return hi3660_thermal_get_temperature(data->regs, data->sensor.id);
}

static int hi6220_thermal_disable_sensor(struct hisi_thermal_data *data)
{
	/* disable sensor module */
	hi6220_thermal_enable(data->regs, 0);
	hi6220_thermal_alarm_enable(data->regs, 0);
	hi6220_thermal_reset_enable(data->regs, 0);

	clk_disable_unprepare(data->clk);

	return 0;
}

static int hi3660_thermal_disable_sensor(struct hisi_thermal_data *data)
{
	/* disable sensor module */
	hi3660_thermal_alarm_enable(data->regs, data->sensor.id, 0);
	return 0;
}

static int hi6220_thermal_enable_sensor(struct hisi_thermal_data *data)
{
	struct hisi_thermal_sensor *sensor = &data->sensor;
	int ret;

	/* enable clock for tsensor */
	ret = clk_prepare_enable(data->clk);
	if (ret)
		return ret;

	/* disable module firstly */
	hi6220_thermal_reset_enable(data->regs, 0);
	hi6220_thermal_enable(data->regs, 0);

	/* select sensor id */
	hi6220_thermal_sensor_select(data->regs, sensor->id);

	/* setting the hdak time */
	hi6220_thermal_hdak_set(data->regs, 0);

	/* setting lag value between current temp and the threshold */
	hi6220_thermal_set_lag(data->regs, HI6220_TEMP_LAG);

	/* enable for interrupt */
	hi6220_thermal_alarm_set(data->regs, sensor->thres_temp);

	hi6220_thermal_reset_set(data->regs, HI6220_TEMP_RESET);

	/* enable module */
	hi6220_thermal_reset_enable(data->regs, 1);
	hi6220_thermal_enable(data->regs, 1);

	hi6220_thermal_alarm_clear(data->regs, 0);
	hi6220_thermal_alarm_enable(data->regs, 1);

	return 0;
}

static int hi3660_thermal_enable_sensor(struct hisi_thermal_data *data)
{
	unsigned int value;
	struct hisi_thermal_sensor *sensor = &data->sensor;

	/* disable interrupt */
	hi3660_thermal_alarm_enable(data->regs, sensor->id, 0);

	/* setting lag value between current temp and the threshold */
	hi3660_thermal_set_lag(data->regs, sensor->id, HI3660_TEMP_LAG);

	/* set interrupt threshold */
	value = hi3660_thermal_temp_to_step(sensor->thres_temp);
	hi3660_thermal_alarm_set(data->regs, sensor->id, value);

	/* enable interrupt */
	hi3660_thermal_alarm_clear(data->regs, sensor->id, 1);
	hi3660_thermal_alarm_enable(data->regs, sensor->id, 1);

	return 0;
}

static int hi6220_thermal_probe(struct hisi_thermal_data *data)
{
	struct platform_device *pdev = data->pdev;
	struct device *dev = &pdev->dev;
	struct resource *res;
	int ret;

	data->get_temp = hi6220_thermal_get_temp;
	data->enable_sensor = hi6220_thermal_enable_sensor;
	data->disable_sensor = hi6220_thermal_disable_sensor;
	data->irq_handler = hi6220_thermal_irq_handler;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	data->regs = devm_ioremap_resource(dev, res);
	if (IS_ERR(data->regs)) {
		dev_err(dev, "failed to get io address\n");
		return PTR_ERR(data->regs);
	}

	data->clk = devm_clk_get(dev, "thermal_clk");
	if (IS_ERR(data->clk)) {
		ret = PTR_ERR(data->clk);
		if (ret != -EPROBE_DEFER)
			dev_err(dev, "failed to get thermal clk: %d\n", ret);
		return ret;
	}

	data->irq = platform_get_irq(pdev, 0);
	if (data->irq < 0)
		return data->irq;

	data->sensor.id = HI6220_DEFAULT_SENSOR;

	return 0;
}

static int hi3660_thermal_probe(struct hisi_thermal_data *data)
{
	struct platform_device *pdev = data->pdev;
	struct device *dev = &pdev->dev;
	struct resource *res;

	data->get_temp = hi3660_thermal_get_temp;
	data->enable_sensor = hi3660_thermal_enable_sensor;
	data->disable_sensor = hi3660_thermal_disable_sensor;
	data->irq_handler = hi3660_thermal_irq_handler;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	data->regs = devm_ioremap_resource(dev, res);
	if (IS_ERR(data->regs)) {
		dev_err(dev, "failed to get io address\n");
		return PTR_ERR(data->regs);
	}

	data->irq = platform_get_irq(pdev, 0);
	if (data->irq < 0)
		return data->irq;

	data->sensor.id = HI3660_DEFAULT_SENSOR;
=======
 *	Temp = TempBase + (steps x 784)
 *
 * and the steps from the temperature:
 *
 *	steps = (Temp - TempBase) / 784
 *
 */
static inline int hisi_thermal_step_to_temp(int step)
{
	return HISI_TEMP_BASE + (step * HISI_TEMP_STEP);
}

static inline long hisi_thermal_temp_to_step(long temp)
{
	return (temp - HISI_TEMP_BASE) / HISI_TEMP_STEP;
}

static inline long hisi_thermal_round_temp(int temp)
{
	return hisi_thermal_step_to_temp(
		hisi_thermal_temp_to_step(temp));
}

static long hisi_thermal_get_sensor_temp(struct hisi_thermal_data *data,
					 struct hisi_thermal_sensor *sensor)
{
	long val;

	mutex_lock(&data->thermal_lock);

	/* disable interrupt */
	writel(0x0, data->regs + TEMP0_INT_EN);
	writel(0x1, data->regs + TEMP0_INT_CLR);

	/* disable module firstly */
	writel(0x0, data->regs + TEMP0_EN);

	/* select sensor id */
	writel((sensor->id << 12), data->regs + TEMP0_CFG);

	/* enable module */
	writel(0x1, data->regs + TEMP0_EN);

	usleep_range(3000, 5000);

	val = readl(data->regs + TEMP0_VALUE);
	val = hisi_thermal_step_to_temp(val);

	mutex_unlock(&data->thermal_lock);

	return val;
}

static void hisi_thermal_enable_bind_irq_sensor
			(struct hisi_thermal_data *data)
{
	struct hisi_thermal_sensor *sensor;

	mutex_lock(&data->thermal_lock);

	sensor = &data->sensors[data->irq_bind_sensor];

	/* setting the hdak time */
	writel(0x0, data->regs + TEMP0_CFG);

	/* disable module firstly */
	writel(0x0, data->regs + TEMP0_RST_MSK);
	writel(0x0, data->regs + TEMP0_EN);

	/* select sensor id */
	writel((sensor->id << 12), data->regs + TEMP0_CFG);

	/* enable for interrupt */
	writel(hisi_thermal_temp_to_step(sensor->thres_temp) | 0x0FFFFFF00,
	       data->regs + TEMP0_TH);

	writel(hisi_thermal_temp_to_step(HISI_TEMP_RESET),
	       data->regs + TEMP0_RST_TH);

	/* enable module */
	writel(0x1, data->regs + TEMP0_RST_MSK);
	writel(0x1, data->regs + TEMP0_EN);

	writel(0x0, data->regs + TEMP0_INT_CLR);
	writel(0x1, data->regs + TEMP0_INT_EN);

	usleep_range(3000, 5000);

	mutex_unlock(&data->thermal_lock);
}

static void hisi_thermal_disable_sensor(struct hisi_thermal_data *data)
{
	mutex_lock(&data->thermal_lock);

	/* disable sensor module */
	writel(0x0, data->regs + TEMP0_INT_EN);
	writel(0x0, data->regs + TEMP0_RST_MSK);
	writel(0x0, data->regs + TEMP0_EN);

	mutex_unlock(&data->thermal_lock);
}

static int hisi_thermal_get_temp(void *_sensor, int *temp)
{
	struct hisi_thermal_sensor *sensor = _sensor;
	struct hisi_thermal_data *data = sensor->thermal;

	int sensor_id = -1, i;
	long max_temp = 0;

	*temp = hisi_thermal_get_sensor_temp(data, sensor);

	sensor->sensor_temp = *temp;

	for (i = 0; i < HISI_MAX_SENSORS; i++) {
		if (!data->sensors[i].tzd)
			continue;

		if (data->sensors[i].sensor_temp >= max_temp) {
			max_temp = data->sensors[i].sensor_temp;
			sensor_id = i;
		}
	}

	/* If no sensor has been enabled, then skip to enable irq */
	if (sensor_id == -1)
		return 0;

	mutex_lock(&data->thermal_lock);
	data->irq_bind_sensor = sensor_id;
	mutex_unlock(&data->thermal_lock);

	dev_dbg(&data->pdev->dev, "id=%d, irq=%d, temp=%d, thres=%d\n",
		sensor->id, data->irq_enabled, *temp, sensor->thres_temp);
	/*
	 * Bind irq to sensor for two cases:
	 *   Reenable alarm IRQ if temperature below threshold;
	 *   if irq has been enabled, always set it;
	 */
	if (data->irq_enabled) {
		hisi_thermal_enable_bind_irq_sensor(data);
		return 0;
	}

	if (max_temp < sensor->thres_temp) {
		data->irq_enabled = true;
		hisi_thermal_enable_bind_irq_sensor(data);
		enable_irq(data->irq);
	}
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	return 0;
}

<<<<<<< HEAD
static int hisi_thermal_get_temp(void *__data, int *temp)
{
	struct hisi_thermal_data *data = __data;
	struct hisi_thermal_sensor *sensor = &data->sensor;

	*temp = data->get_temp(data);

	dev_dbg(&data->pdev->dev, "id=%d, temp=%d, thres=%d\n",
		sensor->id, *temp, sensor->thres_temp);

	return 0;
}

static const struct thermal_zone_of_device_ops hisi_of_thermal_ops = {
	.get_temp = hisi_thermal_get_temp,
};

static irqreturn_t hisi_thermal_alarm_irq_thread(int irq, void *dev)
{
	struct hisi_thermal_data *data = dev;
	struct hisi_thermal_sensor *sensor = &data->sensor;
	int temp = 0;

	data->irq_handler(data);

	hisi_thermal_get_temp(data, &temp);

	if (temp >= sensor->thres_temp) {
		dev_crit(&data->pdev->dev, "THERMAL ALARM: %d > %d\n",
			 temp, sensor->thres_temp);

		thermal_zone_device_update(data->sensor.tzd,
					   THERMAL_EVENT_UNSPECIFIED);

	} else {
		dev_crit(&data->pdev->dev, "THERMAL ALARM stopped: %d < %d\n",
			 temp, sensor->thres_temp);
=======
static struct thermal_zone_of_device_ops hisi_of_thermal_ops = {
	.get_temp = hisi_thermal_get_temp,
};

static irqreturn_t hisi_thermal_alarm_irq(int irq, void *dev)
{
	struct hisi_thermal_data *data = dev;

	disable_irq_nosync(irq);
	data->irq_enabled = false;

	return IRQ_WAKE_THREAD;
}

static irqreturn_t hisi_thermal_alarm_irq_thread(int irq, void *dev)
{
	struct hisi_thermal_data *data = dev;
	struct hisi_thermal_sensor *sensor;
	int i;

	mutex_lock(&data->thermal_lock);
	sensor = &data->sensors[data->irq_bind_sensor];

	dev_crit(&data->pdev->dev, "THERMAL ALARM: T > %d\n",
		 sensor->thres_temp);
	mutex_unlock(&data->thermal_lock);

	for (i = 0; i < HISI_MAX_SENSORS; i++) {
		if (!data->sensors[i].tzd)
			continue;

		thermal_zone_device_update(data->sensors[i].tzd,
					   THERMAL_EVENT_UNSPECIFIED);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	}

	return IRQ_HANDLED;
}

static int hisi_thermal_register_sensor(struct platform_device *pdev,
					struct hisi_thermal_data *data,
<<<<<<< HEAD
					struct hisi_thermal_sensor *sensor)
=======
					struct hisi_thermal_sensor *sensor,
					int index)
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
{
	int ret, i;
	const struct thermal_trip *trip;

<<<<<<< HEAD
	sensor->tzd = devm_thermal_zone_of_sensor_register(&pdev->dev,
							   sensor->id, data,
							   &hisi_of_thermal_ops);
=======
	sensor->id = index;
	sensor->thermal = data;

	sensor->tzd = devm_thermal_zone_of_sensor_register(&pdev->dev,
				sensor->id, sensor, &hisi_of_thermal_ops);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	if (IS_ERR(sensor->tzd)) {
		ret = PTR_ERR(sensor->tzd);
		sensor->tzd = NULL;
		dev_err(&pdev->dev, "failed to register sensor id %d: %d\n",
			sensor->id, ret);
		return ret;
	}

	trip = of_thermal_get_trip_points(sensor->tzd);

	for (i = 0; i < of_thermal_get_ntrips(sensor->tzd); i++) {
		if (trip[i].type == THERMAL_TRIP_PASSIVE) {
<<<<<<< HEAD
			sensor->thres_temp = trip[i].temperature;
=======
			sensor->thres_temp = hisi_thermal_round_temp(trip[i].temperature);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
			break;
		}
	}

	return 0;
}

static const struct of_device_id of_hisi_thermal_match[] = {
<<<<<<< HEAD
	{
		.compatible = "hisilicon,tsensor",
		.data = hi6220_thermal_probe
	},
	{
		.compatible = "hisilicon,hi3660-tsensor",
		.data = hi3660_thermal_probe
	},
=======
	{ .compatible = "hisilicon,tsensor" },
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	{ /* end */ }
};
MODULE_DEVICE_TABLE(of, of_hisi_thermal_match);

static void hisi_thermal_toggle_sensor(struct hisi_thermal_sensor *sensor,
				       bool on)
{
	struct thermal_zone_device *tzd = sensor->tzd;

	tzd->ops->set_mode(tzd,
		on ? THERMAL_DEVICE_ENABLED : THERMAL_DEVICE_DISABLED);
}

static int hisi_thermal_probe(struct platform_device *pdev)
{
	struct hisi_thermal_data *data;
<<<<<<< HEAD
	int const (*platform_probe)(struct hisi_thermal_data *);
	struct device *dev = &pdev->dev;
	int ret;

	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->pdev = pdev;
	platform_set_drvdata(pdev, data);

	platform_probe = of_device_get_match_data(dev);
	if (!platform_probe) {
		dev_err(dev, "failed to get probe func\n");
		return -EINVAL;
	}

	ret = platform_probe(data);
	if (ret)
		return ret;

	ret = hisi_thermal_register_sensor(pdev, data,
					   &data->sensor);
	if (ret) {
		dev_err(dev, "failed to register thermal sensor: %d\n", ret);
		return ret;
	}

	ret = data->enable_sensor(data);
	if (ret) {
		dev_err(dev, "Failed to setup the sensor: %d\n", ret);
		return ret;
	}

	if (data->irq) {
		ret = devm_request_threaded_irq(dev, data->irq, NULL,
				hisi_thermal_alarm_irq_thread,
				IRQF_ONESHOT, "hisi_thermal", data);
		if (ret < 0) {
			dev_err(dev, "failed to request alarm irq: %d\n", ret);
			return ret;
		}
	}

	hisi_thermal_toggle_sensor(&data->sensor, true);
=======
	struct resource *res;
	int i;
	int ret;

	data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	mutex_init(&data->thermal_lock);
	data->pdev = pdev;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	data->regs = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(data->regs)) {
		dev_err(&pdev->dev, "failed to get io address\n");
		return PTR_ERR(data->regs);
	}

	data->irq = platform_get_irq(pdev, 0);
	if (data->irq < 0)
		return data->irq;

	platform_set_drvdata(pdev, data);

	data->clk = devm_clk_get(&pdev->dev, "thermal_clk");
	if (IS_ERR(data->clk)) {
		ret = PTR_ERR(data->clk);
		if (ret != -EPROBE_DEFER)
			dev_err(&pdev->dev,
				"failed to get thermal clk: %d\n", ret);
		return ret;
	}

	/* enable clock for thermal */
	ret = clk_prepare_enable(data->clk);
	if (ret) {
		dev_err(&pdev->dev, "failed to enable thermal clk: %d\n", ret);
		return ret;
	}

	hisi_thermal_enable_bind_irq_sensor(data);
	data->irq_enabled = true;

	for (i = 0; i < HISI_MAX_SENSORS; ++i) {
		ret = hisi_thermal_register_sensor(pdev, data,
						   &data->sensors[i], i);
		if (ret)
			dev_err(&pdev->dev,
				"failed to register thermal sensor: %d\n", ret);
		else
			hisi_thermal_toggle_sensor(&data->sensors[i], true);
	}

	ret = devm_request_threaded_irq(&pdev->dev, data->irq,
					hisi_thermal_alarm_irq,
					hisi_thermal_alarm_irq_thread,
					0, "hisi_thermal", data);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed to request alarm irq: %d\n", ret);
		return ret;
	}

	enable_irq(data->irq);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	return 0;
}

static int hisi_thermal_remove(struct platform_device *pdev)
{
	struct hisi_thermal_data *data = platform_get_drvdata(pdev);
<<<<<<< HEAD
	struct hisi_thermal_sensor *sensor = &data->sensor;

	hisi_thermal_toggle_sensor(sensor, false);

	data->disable_sensor(data);
=======
	int i;

	for (i = 0; i < HISI_MAX_SENSORS; i++) {
		struct hisi_thermal_sensor *sensor = &data->sensors[i];

		if (!sensor->tzd)
			continue;

		hisi_thermal_toggle_sensor(sensor, false);
	}

	hisi_thermal_disable_sensor(data);
	clk_disable_unprepare(data->clk);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int hisi_thermal_suspend(struct device *dev)
{
	struct hisi_thermal_data *data = dev_get_drvdata(dev);

<<<<<<< HEAD
	data->disable_sensor(data);
=======
	hisi_thermal_disable_sensor(data);
	data->irq_enabled = false;

	clk_disable_unprepare(data->clk);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	return 0;
}

static int hisi_thermal_resume(struct device *dev)
{
	struct hisi_thermal_data *data = dev_get_drvdata(dev);
<<<<<<< HEAD

	return data->enable_sensor(data);
=======
	int ret;

	ret = clk_prepare_enable(data->clk);
	if (ret)
		return ret;

	data->irq_enabled = true;
	hisi_thermal_enable_bind_irq_sensor(data);

	return 0;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
}
#endif

static SIMPLE_DEV_PM_OPS(hisi_thermal_pm_ops,
			 hisi_thermal_suspend, hisi_thermal_resume);

static struct platform_driver hisi_thermal_driver = {
	.driver = {
		.name		= "hisi_thermal",
		.pm		= &hisi_thermal_pm_ops,
		.of_match_table = of_hisi_thermal_match,
	},
	.probe	= hisi_thermal_probe,
	.remove	= hisi_thermal_remove,
};

module_platform_driver(hisi_thermal_driver);

MODULE_AUTHOR("Xinwei Kong <kong.kongxinwei@hisilicon.com>");
MODULE_AUTHOR("Leo Yan <leo.yan@linaro.org>");
MODULE_DESCRIPTION("Hisilicon thermal driver");
MODULE_LICENSE("GPL v2");
