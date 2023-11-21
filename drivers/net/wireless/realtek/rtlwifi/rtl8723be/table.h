<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright(c) 2009-2014  Realtek Corporation.*/
=======
/******************************************************************************
 *
 * Copyright(c) 2009-2014  Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 * Realtek Corporation, No. 2, Innovation Road II, Hsinchu Science Park,
 * Hsinchu 300, Taiwan.
 *
 * Created on  2010/ 5/18,  1:41
 *
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 *****************************************************************************/
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

#ifndef __RTL8723BE_TABLE__H_
#define __RTL8723BE_TABLE__H_

#include <linux/types.h>
<<<<<<< HEAD
extern u32 RTL8723BEPHY_REG_1TARRAYLEN;
extern u32 RTL8723BEPHY_REG_1TARRAY[];
extern u32 RTL8723BEPHY_REG_ARRAY_PGLEN;
extern u32 RTL8723BEPHY_REG_ARRAY_PG[];
extern u32 RTL8723BE_RADIOA_1TARRAYLEN;
extern u32 RTL8723BE_RADIOA_1TARRAY[];
extern u32 RTL8723BEMAC_1T_ARRAYLEN;
extern u32 RTL8723BEMAC_1T_ARRAY[];
extern u32 RTL8723BEAGCTAB_1TARRAYLEN;
=======
#define  RTL8723BEPHY_REG_1TARRAYLEN	388
extern u32 RTL8723BEPHY_REG_1TARRAY[];
#define RTL8723BEPHY_REG_ARRAY_PGLEN	36
extern u32 RTL8723BEPHY_REG_ARRAY_PG[];
#define	RTL8723BE_RADIOA_1TARRAYLEN	206
extern u32 RTL8723BE_RADIOA_1TARRAY[];
#define RTL8723BEMAC_1T_ARRAYLEN	196
extern u32 RTL8723BEMAC_1T_ARRAY[];
#define RTL8723BEAGCTAB_1TARRAYLEN	260
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
extern u32 RTL8723BEAGCTAB_1TARRAY[];

#endif
