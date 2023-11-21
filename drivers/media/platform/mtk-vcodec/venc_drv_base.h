/*
 * Copyright (c) 2016 MediaTek Inc.
 * Author: Daniel Hsiao <daniel.hsiao@mediatek.com>
<<<<<<< HEAD
 *      Jungchang Tsao <jungchang.tsao@mediatek.com>
 *      Tiffany Lin <tiffany.lin@mediatek.com>
=======
 *	Jungchang Tsao <jungchang.tsao@mediatek.com>
 *	Tiffany Lin <tiffany.lin@mediatek.com>
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
 *
 * This program is free software; you can redistribute it and/or
 * modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _VENC_DRV_BASE_
#define _VENC_DRV_BASE_

#include "mtk_vcodec_drv.h"

#include "venc_drv_if.h"

struct venc_common_if {
	/**
	 * (*init)() - initialize driver
<<<<<<< HEAD
	 * @ctx:        [in] mtk v4l2 context
=======
	 * @ctx:	[in] mtk v4l2 context
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	 * @handle: [out] driver handle
	 */
	int (*init)(struct mtk_vcodec_ctx *ctx, unsigned long *handle);

	/**
	 * (*encode)() - trigger encode
	 * @handle: [in] driver handle
	 * @opt: [in] encode option
	 * @frm_buf: [in] frame buffer to store input frame
	 * @bs_buf: [in] bitstream buffer to store output bitstream
	 * @result: [out] encode result
	 */
	int (*encode)(unsigned long handle, enum venc_start_opt opt,
<<<<<<< HEAD
				  struct venc_frm_buf *frm_buf,
				  struct mtk_vcodec_mem *bs_buf,
				  struct venc_done_result *result);

	/**
	 * (*get_param)() - set driver's parameter
	 * @handle: [in] driver handle
	 * @type: [in] parameter type
	 * @in: [in] buffer to store the parameter
	 */
	int (*get_param)(unsigned long handle, enum venc_get_param_type type,
					 void *out);
=======
		      struct venc_frm_buf *frm_buf,
		      struct mtk_vcodec_mem *bs_buf,
		      struct venc_done_result *result);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	/**
	 * (*set_param)() - set driver's parameter
	 * @handle: [in] driver handle
	 * @type: [in] parameter type
	 * @in: [in] buffer to store the parameter
	 */
	int (*set_param)(unsigned long handle, enum venc_set_param_type type,
<<<<<<< HEAD
					 struct venc_enc_param *in);
=======
			 struct venc_enc_param *in);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	/**
	 * (*deinit)() - deinitialize driver.
	 * @handle: [in] driver handle
	 */
	int (*deinit)(unsigned long handle);
};

#endif
