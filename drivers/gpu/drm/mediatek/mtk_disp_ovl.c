/*
 * Copyright (c) 2015 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <drm/drmP.h>
#include <linux/clk.h>
#include <linux/component.h>
<<<<<<< HEAD
#include <linux/iommu.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
=======
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

#include "mtk_drm_crtc.h"
#include "mtk_drm_ddp_comp.h"

#define DISP_REG_OVL_INTEN			0x0004
#define OVL_FME_CPL_INT					BIT(1)
#define DISP_REG_OVL_INTSTA			0x0008
#define DISP_REG_OVL_EN				0x000c
#define DISP_REG_OVL_RST			0x0014
#define DISP_REG_OVL_ROI_SIZE			0x0020
#define DISP_REG_OVL_ROI_BGCLR			0x0028
#define DISP_REG_OVL_SRC_CON			0x002c
#define DISP_REG_OVL_CON(n)			(0x0030 + 0x20 * (n))
#define DISP_REG_OVL_SRC_SIZE(n)		(0x0038 + 0x20 * (n))
#define DISP_REG_OVL_OFFSET(n)			(0x003c + 0x20 * (n))
#define DISP_REG_OVL_PITCH(n)			(0x0044 + 0x20 * (n))
#define DISP_REG_OVL_RDMA_CTRL(n)		(0x00c0 + 0x20 * (n))
#define DISP_REG_OVL_RDMA_GMC(n)		(0x00c8 + 0x20 * (n))
<<<<<<< HEAD
#define DISP_REG_OVL_ADDR_MT2701		0x0040
#define DISP_REG_OVL_ADDR_MT8173		0x0f40
#define DISP_REG_OVL_ADDR(module, n)	((module)->data->addr + 0x20 * (n))
=======
#define DISP_REG_OVL_ADDR(n)			(0x0f40 + 0x20 * (n))
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

#define	OVL_RDMA_MEM_GMC	0x40402020

#define OVL_CON_BYTE_SWAP	BIT(24)
<<<<<<< HEAD
#define OVL_CON_MTX_YUV_TO_RGB	(6UL << 16)
#define OVL_CON_CLRFMT_RGB	(1UL << 12)
#define OVL_CON_CLRFMT_RGBA8888	(2 << 12)
#define OVL_CON_CLRFMT_ARGB8888	(3 << 12)
#define OVL_CON_CLRFMT_RGB565(module)	(((module)->data->fmt_rgb565_is_0 \
					== true) ? 0UL : OVL_CON_CLRFMT_RGB)
#define OVL_CON_CLRFMT_RGB888(module)	(((module)->data->fmt_rgb565_is_0 \
					== true) ? OVL_CON_CLRFMT_RGB : 0UL)
#define OVL_CON_CLRFMT_UYVY(module)	((module)->data->fmt_uyvy)
#define OVL_CON_CLRFMT_YUYV(module)	((module)->data->fmt_yuyv)
#define	OVL_CON_AEN		BIT(8)
#define	OVL_CON_ALPHA		0xff

struct mtk_disp_ovl_data {
	unsigned int addr;
	bool fmt_rgb565_is_0;
	unsigned int fmt_uyvy;
	unsigned int fmt_yuyv;
};

=======
#define OVL_CON_CLRFMT_RGB565	(0 << 12)
#define OVL_CON_CLRFMT_RGB888	(1 << 12)
#define OVL_CON_CLRFMT_RGBA8888	(2 << 12)
#define OVL_CON_CLRFMT_ARGB8888	(3 << 12)
#define	OVL_CON_AEN		BIT(8)
#define	OVL_CON_ALPHA		0xff

>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
/**
 * struct mtk_disp_ovl - DISP_OVL driver structure
 * @ddp_comp - structure containing type enum and hardware resources
 * @crtc - associated crtc to report vblank events to
 */
struct mtk_disp_ovl {
	struct mtk_ddp_comp		ddp_comp;
	struct drm_crtc			*crtc;
<<<<<<< HEAD
	const struct mtk_disp_ovl_data	*data;
};

static inline struct mtk_disp_ovl *comp_to_ovl(struct mtk_ddp_comp *comp)
{
	return container_of(comp, struct mtk_disp_ovl, ddp_comp);
}

=======
};

>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
static irqreturn_t mtk_disp_ovl_irq_handler(int irq, void *dev_id)
{
	struct mtk_disp_ovl *priv = dev_id;
	struct mtk_ddp_comp *ovl = &priv->ddp_comp;

	/* Clear frame completion interrupt */
	writel(0x0, ovl->regs + DISP_REG_OVL_INTSTA);

	if (!priv->crtc)
		return IRQ_NONE;

	mtk_crtc_ddp_irq(priv->crtc, ovl);

	return IRQ_HANDLED;
}

static void mtk_ovl_enable_vblank(struct mtk_ddp_comp *comp,
				  struct drm_crtc *crtc)
{
<<<<<<< HEAD
	struct mtk_disp_ovl *ovl = comp_to_ovl(comp);

	ovl->crtc = crtc;
=======
	struct mtk_disp_ovl *priv = container_of(comp, struct mtk_disp_ovl,
						 ddp_comp);

	priv->crtc = crtc;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	writel(0x0, comp->regs + DISP_REG_OVL_INTSTA);
	writel_relaxed(OVL_FME_CPL_INT, comp->regs + DISP_REG_OVL_INTEN);
}

static void mtk_ovl_disable_vblank(struct mtk_ddp_comp *comp)
{
<<<<<<< HEAD
	struct mtk_disp_ovl *ovl = comp_to_ovl(comp);

	ovl->crtc = NULL;
=======
	struct mtk_disp_ovl *priv = container_of(comp, struct mtk_disp_ovl,
						 ddp_comp);

	priv->crtc = NULL;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	writel_relaxed(0x0, comp->regs + DISP_REG_OVL_INTEN);
}

static void mtk_ovl_start(struct mtk_ddp_comp *comp)
{
<<<<<<< HEAD
	int ret;

	ret = pm_runtime_get_sync(comp->dev);
	if (ret < 0)
		DRM_ERROR("Failed to enable power domain: %d\n", ret);

=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	writel_relaxed(0x1, comp->regs + DISP_REG_OVL_EN);
}

static void mtk_ovl_stop(struct mtk_ddp_comp *comp)
{
<<<<<<< HEAD
	int ret;

	ret = pm_runtime_put(comp->dev);
	if (ret < 0)
		DRM_ERROR("Failed to disable power domain: %d\n", ret);

=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	writel_relaxed(0x0, comp->regs + DISP_REG_OVL_EN);
}

static void mtk_ovl_config(struct mtk_ddp_comp *comp, unsigned int w,
			   unsigned int h, unsigned int vrefresh,
			   unsigned int bpc)
{
	if (w != 0 && h != 0)
		writel_relaxed(h << 16 | w, comp->regs + DISP_REG_OVL_ROI_SIZE);
	writel_relaxed(0x0, comp->regs + DISP_REG_OVL_ROI_BGCLR);

	writel(0x1, comp->regs + DISP_REG_OVL_RST);
	writel(0x0, comp->regs + DISP_REG_OVL_RST);
}

static void mtk_ovl_layer_on(struct mtk_ddp_comp *comp, unsigned int idx)
{
	unsigned int reg;

	writel(0x1, comp->regs + DISP_REG_OVL_RDMA_CTRL(idx));
	writel(OVL_RDMA_MEM_GMC, comp->regs + DISP_REG_OVL_RDMA_GMC(idx));

	reg = readl(comp->regs + DISP_REG_OVL_SRC_CON);
	reg = reg | BIT(idx);
	writel(reg, comp->regs + DISP_REG_OVL_SRC_CON);
}

static void mtk_ovl_layer_off(struct mtk_ddp_comp *comp, unsigned int idx)
{
	unsigned int reg;

	reg = readl(comp->regs + DISP_REG_OVL_SRC_CON);
	reg = reg & ~BIT(idx);
	writel(reg, comp->regs + DISP_REG_OVL_SRC_CON);

	writel(0x0, comp->regs + DISP_REG_OVL_RDMA_CTRL(idx));
}

<<<<<<< HEAD
static unsigned int ovl_fmt_convert(struct mtk_disp_ovl *ovl, unsigned int fmt)
=======
static unsigned int ovl_fmt_convert(unsigned int fmt)
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
{
	switch (fmt) {
	default:
	case DRM_FORMAT_RGB565:
<<<<<<< HEAD
		return OVL_CON_CLRFMT_RGB565(ovl);
	case DRM_FORMAT_BGR565:
		return (unsigned int)OVL_CON_CLRFMT_RGB565(ovl) |
				     OVL_CON_BYTE_SWAP;
	case DRM_FORMAT_RGB888:
		return OVL_CON_CLRFMT_RGB888(ovl);
	case DRM_FORMAT_BGR888:
		return (unsigned int)OVL_CON_CLRFMT_RGB888(ovl) |
				     OVL_CON_BYTE_SWAP;
=======
		return OVL_CON_CLRFMT_RGB565;
	case DRM_FORMAT_BGR565:
		return OVL_CON_CLRFMT_RGB565 | OVL_CON_BYTE_SWAP;
	case DRM_FORMAT_RGB888:
		return OVL_CON_CLRFMT_RGB888;
	case DRM_FORMAT_BGR888:
		return OVL_CON_CLRFMT_RGB888 | OVL_CON_BYTE_SWAP;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	case DRM_FORMAT_RGBX8888:
	case DRM_FORMAT_RGBA8888:
		return OVL_CON_CLRFMT_ARGB8888;
	case DRM_FORMAT_BGRX8888:
	case DRM_FORMAT_BGRA8888:
		return OVL_CON_CLRFMT_ARGB8888 | OVL_CON_BYTE_SWAP;
	case DRM_FORMAT_XRGB8888:
	case DRM_FORMAT_ARGB8888:
		return OVL_CON_CLRFMT_RGBA8888;
	case DRM_FORMAT_XBGR8888:
	case DRM_FORMAT_ABGR8888:
		return OVL_CON_CLRFMT_RGBA8888 | OVL_CON_BYTE_SWAP;
<<<<<<< HEAD
	case DRM_FORMAT_UYVY:
		return OVL_CON_CLRFMT_UYVY(ovl) | OVL_CON_MTX_YUV_TO_RGB;
	case DRM_FORMAT_YUYV:
		return OVL_CON_CLRFMT_YUYV(ovl) | OVL_CON_MTX_YUV_TO_RGB;
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	}
}

static void mtk_ovl_layer_config(struct mtk_ddp_comp *comp, unsigned int idx,
				 struct mtk_plane_state *state)
{
<<<<<<< HEAD
	struct mtk_disp_ovl *ovl = comp_to_ovl(comp);
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	struct mtk_plane_pending_state *pending = &state->pending;
	unsigned int addr = pending->addr;
	unsigned int pitch = pending->pitch & 0xffff;
	unsigned int fmt = pending->format;
	unsigned int offset = (pending->y << 16) | pending->x;
	unsigned int src_size = (pending->height << 16) | pending->width;
	unsigned int con;

	if (!pending->enable)
		mtk_ovl_layer_off(comp, idx);

<<<<<<< HEAD
	con = ovl_fmt_convert(ovl, fmt);
=======
	con = ovl_fmt_convert(fmt);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	if (idx != 0)
		con |= OVL_CON_AEN | OVL_CON_ALPHA;

	writel_relaxed(con, comp->regs + DISP_REG_OVL_CON(idx));
	writel_relaxed(pitch, comp->regs + DISP_REG_OVL_PITCH(idx));
	writel_relaxed(src_size, comp->regs + DISP_REG_OVL_SRC_SIZE(idx));
	writel_relaxed(offset, comp->regs + DISP_REG_OVL_OFFSET(idx));
<<<<<<< HEAD
	writel_relaxed(addr, comp->regs + DISP_REG_OVL_ADDR(ovl, idx));
=======
	writel_relaxed(addr, comp->regs + DISP_REG_OVL_ADDR(idx));
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	if (pending->enable)
		mtk_ovl_layer_on(comp, idx);
}

static const struct mtk_ddp_comp_funcs mtk_disp_ovl_funcs = {
	.config = mtk_ovl_config,
	.start = mtk_ovl_start,
	.stop = mtk_ovl_stop,
	.enable_vblank = mtk_ovl_enable_vblank,
	.disable_vblank = mtk_ovl_disable_vblank,
	.layer_on = mtk_ovl_layer_on,
	.layer_off = mtk_ovl_layer_off,
	.layer_config = mtk_ovl_layer_config,
};

static int mtk_disp_ovl_bind(struct device *dev, struct device *master,
			     void *data)
{
	struct mtk_disp_ovl *priv = dev_get_drvdata(dev);
	struct drm_device *drm_dev = data;
	int ret;

	ret = mtk_ddp_comp_register(drm_dev, &priv->ddp_comp);
	if (ret < 0) {
		dev_err(dev, "Failed to register component %s: %d\n",
			dev->of_node->full_name, ret);
		return ret;
	}

	return 0;
}

static void mtk_disp_ovl_unbind(struct device *dev, struct device *master,
				void *data)
{
	struct mtk_disp_ovl *priv = dev_get_drvdata(dev);
	struct drm_device *drm_dev = data;

	mtk_ddp_comp_unregister(drm_dev, &priv->ddp_comp);
}

static const struct component_ops mtk_disp_ovl_component_ops = {
	.bind	= mtk_disp_ovl_bind,
	.unbind = mtk_disp_ovl_unbind,
};

static int mtk_disp_ovl_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct mtk_disp_ovl *priv;
<<<<<<< HEAD
	enum mtk_ddp_comp_id comp_id;
	int irq;
	int ret;
	struct iommu_domain *iommu;

	iommu = iommu_get_domain_for_dev(dev);
	if (iommu == NULL) {
		dev_info(dev, "Waiting iommu driver ready...\n");
		return -EPROBE_DEFER;
	}
=======
	int comp_id;
	int irq;
	int ret;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;

	comp_id = mtk_ddp_comp_get_id(dev->of_node, MTK_DISP_OVL);
<<<<<<< HEAD
	if ((int)comp_id < 0) {
=======
	if (comp_id < 0) {
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		dev_err(dev, "Failed to identify by alias: %d\n", comp_id);
		return comp_id;
	}

	ret = mtk_ddp_comp_init(dev, dev->of_node, &priv->ddp_comp, comp_id,
				&mtk_disp_ovl_funcs);
	if (ret) {
		dev_err(dev, "Failed to initialize component: %d\n", ret);
		return ret;
	}

<<<<<<< HEAD
	priv->data = of_device_get_match_data(dev);

=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	platform_set_drvdata(pdev, priv);

	ret = devm_request_irq(dev, irq, mtk_disp_ovl_irq_handler,
			       IRQF_TRIGGER_NONE, dev_name(dev), priv);
	if (ret < 0) {
		dev_err(dev, "Failed to request irq %d: %d\n", irq, ret);
		return ret;
	}

<<<<<<< HEAD
	pm_runtime_enable(dev);

	ret = component_add(dev, &mtk_disp_ovl_component_ops);
	if (ret != 0) {
		dev_err(dev, "Failed to add component: %d\n", ret);
		pm_runtime_disable(dev);
	}
=======
	ret = component_add(dev, &mtk_disp_ovl_component_ops);
	if (ret)
		dev_err(dev, "Failed to add component: %d\n", ret);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	return ret;
}

static int mtk_disp_ovl_remove(struct platform_device *pdev)
{
	component_del(&pdev->dev, &mtk_disp_ovl_component_ops);

<<<<<<< HEAD
	pm_runtime_disable(&pdev->dev);
	return 0;
}

static const struct mtk_disp_ovl_data mt2701_ovl_driver_data = {
	.addr = DISP_REG_OVL_ADDR_MT2701,
	.fmt_rgb565_is_0 = false,
	.fmt_uyvy = 9U << 12,
	.fmt_yuyv = 8U << 12,
};

static const struct mtk_disp_ovl_data mt8173_ovl_driver_data = {
	.addr = DISP_REG_OVL_ADDR_MT8173,
	.fmt_rgb565_is_0 = true,
	.fmt_uyvy = 4U << 12,
	.fmt_yuyv = 5U << 12,
};

static const struct of_device_id mtk_disp_ovl_driver_dt_match[] = {
	{ .compatible = "mediatek,mt2701-disp-ovl",
	  .data = &mt2701_ovl_driver_data},
	{ .compatible = "mediatek,mt8173-disp-ovl",
	  .data = &mt8173_ovl_driver_data},
=======
	return 0;
}

static const struct of_device_id mtk_disp_ovl_driver_dt_match[] = {
	{ .compatible = "mediatek,mt8173-disp-ovl", },
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	{},
};
MODULE_DEVICE_TABLE(of, mtk_disp_ovl_driver_dt_match);

struct platform_driver mtk_disp_ovl_driver = {
	.probe		= mtk_disp_ovl_probe,
	.remove		= mtk_disp_ovl_remove,
	.driver		= {
		.name	= "mediatek-disp-ovl",
		.owner	= THIS_MODULE,
		.of_match_table = mtk_disp_ovl_driver_dt_match,
	},
};
