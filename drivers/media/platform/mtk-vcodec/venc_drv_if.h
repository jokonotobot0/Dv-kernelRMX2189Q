/*
 * Copyright (c) 2016 MediaTek Inc.
 * Author: Daniel Hsiao <daniel.hsiao@mediatek.com>
<<<<<<< HEAD
 *              Jungchang Tsao <jungchang.tsao@mediatek.com>
 *              Tiffany Lin <tiffany.lin@mediatek.com>
=======
 *		Jungchang Tsao <jungchang.tsao@mediatek.com>
 *		Tiffany Lin <tiffany.lin@mediatek.com>
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

#ifndef _VENC_DRV_IF_H_
#define _VENC_DRV_IF_H_

#include "mtk_vcodec_drv.h"
#include "mtk_vcodec_util.h"
<<<<<<< HEAD
#include "venc_vcu_if.h"
#include "venc_ipi_msg.h"

/*
 * struct venc_inst - encoder AP driver instance
 * @hw_base: encoder hardware register base
 * @work_bufs: working buffer
 * @pps_buf: buffer to store the pps bitstream
 * @work_buf_allocated: working buffer allocated flag
 * @frm_cnt: encoded frame count
 * @prepend_hdr: when the v4l2 layer send VENC_SET_PARAM_PREPEND_HEADER cmd
 *  through venc_set_param interface, it will set this flag and prepend the
 *  sps/pps in venc_encode function.
 * @vcu_inst: VCU instance to exchange information between AP and VCU
 * @vsi: driver structure allocated by VCU side and shared to AP side for
 *       control and info share
 * @ctx: context for v4l2 layer integration
 */
struct venc_inst {
	void __iomem *hw_base;
	struct mtk_vcodec_mem pps_buf;
	bool work_buf_allocated;
	unsigned int frm_cnt;
	unsigned int prepend_hdr;
	struct venc_vcu_inst vcu_inst;
	struct venc_vsi *vsi;
	struct mtk_vcodec_ctx *ctx;
=======

/*
 * enum venc_yuv_fmt - The type of input yuv format
 * (VPU related: If you change the order, you must also update the VPU codes.)
 * @VENC_YUV_FORMAT_I420: I420 YUV format
 * @VENC_YUV_FORMAT_YV12: YV12 YUV format
 * @VENC_YUV_FORMAT_NV12: NV12 YUV format
 * @VENC_YUV_FORMAT_NV21: NV21 YUV format
 */
enum venc_yuv_fmt {
	VENC_YUV_FORMAT_I420 = 3,
	VENC_YUV_FORMAT_YV12 = 5,
	VENC_YUV_FORMAT_NV12 = 6,
	VENC_YUV_FORMAT_NV21 = 7,
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
};

/*
 * enum venc_start_opt - encode frame option used in venc_if_encode()
 * @VENC_START_OPT_ENCODE_SEQUENCE_HEADER: encode SPS/PPS for H264
 * @VENC_START_OPT_ENCODE_FRAME: encode normal frame
<<<<<<< HEAD
 * @VENC_START_OPT_ENCODE_FRAME_FINAL: encode last frame for oal codec
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
 */
enum venc_start_opt {
	VENC_START_OPT_ENCODE_SEQUENCE_HEADER,
	VENC_START_OPT_ENCODE_FRAME,
<<<<<<< HEAD
	VENC_START_OPT_ENCODE_FRAME_FINAL
};

/*
 * struct venc_done_result - This is return information used in venc_if_encode()
 * @bs_size: output bitstream size
 * @is_key_frm: output is key frame or not
 */
struct venc_done_result {
	__u32 bs_size;
	__u32 is_key_frm;
	__u64 bs_va;
	__u64 frm_va;
};

/*
 * struct venc_resolution_change
 * @width: width resolution change to
 * @height: height resolution change to
 * @resolutionchange : if resolution change
 */
struct venc_resolution_change {
	__u32 width;
	__u32 height;
	__u32 framerate;
	__u32 resolutionchange;
=======
};

/*
 * enum venc_set_param_type - The type of set parameter used in
 *						      venc_if_set_param()
 * (VPU related: If you change the order, you must also update the VPU codes.)
 * @VENC_SET_PARAM_ENC: set encoder parameters
 * @VENC_SET_PARAM_FORCE_INTRA: force an intra frame
 * @VENC_SET_PARAM_ADJUST_BITRATE: adjust bitrate (in bps)
 * @VENC_SET_PARAM_ADJUST_FRAMERATE: set frame rate
 * @VENC_SET_PARAM_GOP_SIZE: set IDR interval
 * @VENC_SET_PARAM_INTRA_PERIOD: set I frame interval
 * @VENC_SET_PARAM_SKIP_FRAME: set H264 skip one frame
 * @VENC_SET_PARAM_PREPEND_HEADER: set H264 prepend SPS/PPS before IDR
 * @VENC_SET_PARAM_TS_MODE: set VP8 temporal scalability mode
 */
enum venc_set_param_type {
	VENC_SET_PARAM_ENC,
	VENC_SET_PARAM_FORCE_INTRA,
	VENC_SET_PARAM_ADJUST_BITRATE,
	VENC_SET_PARAM_ADJUST_FRAMERATE,
	VENC_SET_PARAM_GOP_SIZE,
	VENC_SET_PARAM_INTRA_PERIOD,
	VENC_SET_PARAM_SKIP_FRAME,
	VENC_SET_PARAM_PREPEND_HEADER,
	VENC_SET_PARAM_TS_MODE,
};

/*
 * struct venc_enc_prm - encoder settings for VENC_SET_PARAM_ENC used in
 *					  venc_if_set_param()
 * @input_fourcc: input yuv format
 * @h264_profile: V4L2 defined H.264 profile
 * @h264_level: V4L2 defined H.264 level
 * @width: image width
 * @height: image height
 * @buf_width: buffer width
 * @buf_height: buffer height
 * @frm_rate: frame rate in fps
 * @intra_period: intra frame period
 * @bitrate: target bitrate in bps
 * @gop_size: group of picture size
 */
struct venc_enc_param {
	enum venc_yuv_fmt input_yuv_fmt;
	unsigned int h264_profile;
	unsigned int h264_level;
	unsigned int width;
	unsigned int height;
	unsigned int buf_width;
	unsigned int buf_height;
	unsigned int frm_rate;
	unsigned int intra_period;
	unsigned int bitrate;
	unsigned int gop_size;
};

/*
 * struct venc_frm_buf - frame buffer information used in venc_if_encode()
 * @fb_addr: plane frame buffer addresses
 */
struct venc_frm_buf {
	struct mtk_vcodec_mem fb_addr[MTK_VCODEC_MAX_PLANES];
};

/*
 * struct venc_done_result - This is return information used in venc_if_encode()
 * @bs_size: output bitstream size
 * @is_key_frm: output is key frame or not
 */
struct venc_done_result {
	unsigned int bs_size;
	bool is_key_frm;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
};

/*
 * venc_if_init - Create the driver handle
 * @ctx: device context
 * @fourcc: encoder input format
 * Return: 0 if creating handle successfully, otherwise it is failed.
 */
int venc_if_init(struct mtk_vcodec_ctx *ctx, unsigned int fourcc);

/*
 * venc_if_deinit - Release the driver handle
 * @ctx: device context
 * Return: 0 if releasing handle successfully, otherwise it is failed.
 */
int venc_if_deinit(struct mtk_vcodec_ctx *ctx);

<<<<<<< HEAD
/**
 * venc_if_get_param() - get driver's parameter
 * @ctx : [in] v4l2 context
 * @type    : [in] input parameter type
 * @out : [out] buffer to store query result
 */
int venc_if_get_param(struct mtk_vcodec_ctx *ctx, enum venc_get_param_type type,
					  void *out);

=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
/*
 * venc_if_set_param - Set parameter to driver
 * @ctx: device context
 * @type: parameter type
 * @in: input parameter
 * Return: 0 if setting param successfully, otherwise it is failed.
 */
int venc_if_set_param(struct mtk_vcodec_ctx *ctx,
<<<<<<< HEAD
					  enum venc_set_param_type type,
					  struct venc_enc_param *in);
=======
		      enum venc_set_param_type type,
		      struct venc_enc_param *in);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

/*
 * venc_if_encode - Encode one frame
 * @ctx: device context
 * @opt: encode frame option
 * @frm_buf: input frame buffer information
 * @bs_buf: output bitstream buffer infomraiton
 * @result: encode result
 * Return: 0 if encoding frame successfully, otherwise it is failed.
 */
int venc_if_encode(struct mtk_vcodec_ctx *ctx,
<<<<<<< HEAD
				   enum venc_start_opt opt,
				   struct venc_frm_buf *frm_buf,
				   struct mtk_vcodec_mem *bs_buf,
				   struct venc_done_result *result);


void venc_encode_prepare(void *ctx_prepare, unsigned long *flags);
void venc_encode_unprepare(void *ctx_unprepare, unsigned long *flags);
=======
		   enum venc_start_opt opt,
		   struct venc_frm_buf *frm_buf,
		   struct mtk_vcodec_mem *bs_buf,
		   struct venc_done_result *result);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

#endif /* _VENC_DRV_IF_H_ */
