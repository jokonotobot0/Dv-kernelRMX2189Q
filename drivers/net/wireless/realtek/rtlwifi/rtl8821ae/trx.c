<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
/* Copyright(c) 2009-2010  Realtek Corporation.*/
=======
/******************************************************************************
 *
 * Copyright(c) 2009-2010  Realtek Corporation.
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
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 *****************************************************************************/
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

#include "../wifi.h"
#include "../pci.h"
#include "../base.h"
#include "../stats.h"
#include "reg.h"
#include "def.h"
<<<<<<< HEAD
=======
#include "phy.h"
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
#include "trx.h"
#include "led.h"
#include "dm.h"
#include "phy.h"
#include "fw.h"

static u8 _rtl8821ae_map_hwqueue_to_fwqueue(struct sk_buff *skb, u8 hw_queue)
{
	__le16 fc = rtl_get_fc(skb);

	if (unlikely(ieee80211_is_beacon(fc)))
		return QSLT_BEACON;
	if (ieee80211_is_mgmt(fc) || ieee80211_is_ctl(fc))
		return QSLT_MGNT;

	return skb->priority;
}

static u16 odm_cfo(s8 value)
{
	int ret_val;

	if (value < 0) {
		ret_val = 0 - value;
		ret_val = (ret_val << 1) + (ret_val >> 1);
		/* set bit12 as 1 for negative cfo */
		ret_val = ret_val | BIT(12);
	} else {
		ret_val = value;
		ret_val = (ret_val << 1) + (ret_val >> 1);
	}
	return ret_val;
}

static u8 _rtl8821ae_evm_dbm_jaguar(s8 value)
{
	s8 ret_val = value;

	/* -33dB~0dB to 33dB ~ 0dB*/
	if (ret_val == -128)
		ret_val = 127;
	else if (ret_val < 0)
		ret_val = 0 - ret_val;

	ret_val  = ret_val >> 1;
	return ret_val;
}

static void query_rxphystatus(struct ieee80211_hw *hw,
<<<<<<< HEAD
			      struct rtl_stats *pstatus, __le32 *pdesc,
=======
			      struct rtl_stats *pstatus, u8 *pdesc,
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
			      struct rx_fwinfo_8821ae *p_drvinfo,
			      bool bpacket_match_bssid,
			      bool bpacket_toself, bool packet_beacon)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct phy_status_rpt *p_phystrpt = (struct phy_status_rpt *)p_drvinfo;
	struct rtl_dm *rtldm = rtl_dm(rtl_priv(hw));
	struct rtl_phy *rtlphy = &rtlpriv->phy;
	s8 rx_pwr_all = 0, rx_pwr[4];
	u8 rf_rx_num = 0, evm, evmdbm, pwdb_all;
	u8 i, max_spatial_stream;
	u32 rssi, total_rssi = 0;
	bool is_cck = pstatus->is_cck;
	u8 lan_idx, vga_idx;

	/* Record it for next packet processing */
	pstatus->packet_matchbssid = bpacket_match_bssid;
	pstatus->packet_toself = bpacket_toself;
	pstatus->packet_beacon = packet_beacon;
	pstatus->rx_mimo_signalquality[0] = -1;
	pstatus->rx_mimo_signalquality[1] = -1;

	if (is_cck) {
		u8 cck_highpwr;
		u8 cck_agc_rpt;

		cck_agc_rpt = p_phystrpt->cfosho[0];

		/* (1)Hardware does not provide RSSI for CCK
<<<<<<< HEAD
		 * (2)PWDB, Average PWDB calculated by
=======
		 * (2)PWDB, Average PWDB cacluated by
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		 * hardware (for rate adaptive)
		 */
		cck_highpwr = (u8)rtlphy->cck_high_power;

		lan_idx = ((cck_agc_rpt & 0xE0) >> 5);
		vga_idx = (cck_agc_rpt & 0x1f);
		if (rtlpriv->rtlhal.hw_type == HARDWARE_TYPE_RTL8812AE) {
			switch (lan_idx) {
			case 7:
				if (vga_idx <= 27)
					/*VGA_idx = 27~2*/
					rx_pwr_all = -100 + 2*(27-vga_idx);
				else
					rx_pwr_all = -100;
				break;
			case 6:
				/*VGA_idx = 2~0*/
				rx_pwr_all = -48 + 2*(2-vga_idx);
				break;
			case 5:
				/*VGA_idx = 7~5*/
				rx_pwr_all = -42 + 2*(7-vga_idx);
				break;
			case 4:
				/*VGA_idx = 7~4*/
				rx_pwr_all = -36 + 2*(7-vga_idx);
				break;
			case 3:
				/*VGA_idx = 7~0*/
				rx_pwr_all = -24 + 2*(7-vga_idx);
				break;
			case 2:
				if (cck_highpwr)
					/*VGA_idx = 5~0*/
					rx_pwr_all = -12 + 2*(5-vga_idx);
				else
					rx_pwr_all = -6 + 2*(5-vga_idx);
				break;
			case 1:
				rx_pwr_all = 8-2*vga_idx;
				break;
			case 0:
				rx_pwr_all = 14-2*vga_idx;
				break;
			default:
				break;
			}
			rx_pwr_all += 6;
			pwdb_all = rtl_query_rxpwrpercentage(rx_pwr_all);
			if (!cck_highpwr) {
				if (pwdb_all >= 80)
					pwdb_all =
					  ((pwdb_all - 80)<<1) +
					 ((pwdb_all - 80)>>1) + 80;
				else if ((pwdb_all <= 78) && (pwdb_all >= 20))
					pwdb_all += 3;
				if (pwdb_all > 100)
					pwdb_all = 100;
			}
		} else { /* 8821 */
			s8 pout = -6;

			switch (lan_idx) {
			case 5:
				rx_pwr_all = pout - 32 - (2*vga_idx);
					break;
			case 4:
				rx_pwr_all = pout - 24 - (2*vga_idx);
					break;
			case 2:
				rx_pwr_all = pout - 11 - (2*vga_idx);
					break;
			case 1:
				rx_pwr_all = pout + 5 - (2*vga_idx);
					break;
			case 0:
				rx_pwr_all = pout + 21 - (2*vga_idx);
					break;
			}
			pwdb_all = rtl_query_rxpwrpercentage(rx_pwr_all);
		}

		pstatus->rx_pwdb_all = pwdb_all;
		pstatus->recvsignalpower = rx_pwr_all;

		/* (3) Get Signal Quality (EVM) */
		if (bpacket_match_bssid) {
			u8 sq;

			if (pstatus->rx_pwdb_all > 40) {
				sq = 100;
			} else {
				sq = p_phystrpt->pwdb_all;
				if (sq > 64)
					sq = 0;
				else if (sq < 20)
					sq = 100;
				else
					sq = ((64 - sq) * 100) / 44;
			}

			pstatus->signalquality = sq;
			pstatus->rx_mimo_signalquality[0] = sq;
			pstatus->rx_mimo_signalquality[1] = -1;
		}
	} else {
		/* (1)Get RSSI for HT rate */
		for (i = RF90_PATH_A; i < RF6052_MAX_PATH; i++) {
			/* we will judge RF RX path now. */
			if (rtlpriv->dm.rfpath_rxenable[i])
				rf_rx_num++;

			rx_pwr[i] = (p_phystrpt->gain_trsw[i] & 0x7f) - 110;

			/* Translate DBM to percentage. */
			rssi = rtl_query_rxpwrpercentage(rx_pwr[i]);
			total_rssi += rssi;

			/* Get Rx snr value in DB */
			pstatus->rx_snr[i] = p_phystrpt->rxsnr[i] / 2;
			rtlpriv->stats.rx_snr_db[i] = p_phystrpt->rxsnr[i] / 2;

			pstatus->cfo_short[i] = odm_cfo(p_phystrpt->cfosho[i]);
			pstatus->cfo_tail[i] = odm_cfo(p_phystrpt->cfotail[i]);
			/* Record Signal Strength for next packet */
			pstatus->rx_mimo_signalstrength[i] = (u8)rssi;
		}

<<<<<<< HEAD
		/* (2)PWDB, Average PWDB calculated by
=======
		/* (2)PWDB, Average PWDB cacluated by
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		 * hardware (for rate adaptive)
		 */
		rx_pwr_all = ((p_drvinfo->pwdb_all >> 1) & 0x7f) - 110;

		pwdb_all = rtl_query_rxpwrpercentage(rx_pwr_all);
		pstatus->rx_pwdb_all = pwdb_all;
		pstatus->rxpower = rx_pwr_all;
		pstatus->recvsignalpower = rx_pwr_all;

		/* (3)EVM of HT rate */
		if ((pstatus->is_ht && pstatus->rate >= DESC_RATEMCS8 &&
		     pstatus->rate <= DESC_RATEMCS15) ||
		    (pstatus->is_vht &&
		     pstatus->rate >= DESC_RATEVHT2SS_MCS0 &&
		     pstatus->rate <= DESC_RATEVHT2SS_MCS9))
			max_spatial_stream = 2;
		else
			max_spatial_stream = 1;

		for (i = 0; i < max_spatial_stream; i++) {
			evm = rtl_evm_db_to_percentage(p_phystrpt->rxevm[i]);
			evmdbm = _rtl8821ae_evm_dbm_jaguar(p_phystrpt->rxevm[i]);

			if (bpacket_match_bssid) {
				/* Fill value in RFD, Get the first
				 * spatial stream only
				 */
				if (i == 0)
					pstatus->signalquality = evm;
				pstatus->rx_mimo_signalquality[i] = evm;
				pstatus->rx_mimo_evm_dbm[i] = evmdbm;
			}
		}
		if (bpacket_match_bssid) {
			for (i = RF90_PATH_A; i <= RF90_PATH_B; i++)
				rtl_priv(hw)->dm.cfo_tail[i] =
					(s8)p_phystrpt->cfotail[i];

			rtl_priv(hw)->dm.packet_count++;
		}
	}

	/* UI BSS List signal strength(in percentage),
	 * make it good looking, from 0~100.
	 */
	if (is_cck)
		pstatus->signalstrength = (u8)(rtl_signal_scale_mapping(hw,
			pwdb_all));
	else if (rf_rx_num != 0)
		pstatus->signalstrength = (u8)(rtl_signal_scale_mapping(hw,
			total_rssi /= rf_rx_num));
	/*HW antenna diversity*/
	rtldm->fat_table.antsel_rx_keep_0 = p_phystrpt->antidx_anta;
	rtldm->fat_table.antsel_rx_keep_1 = p_phystrpt->antidx_antb;
}

static void translate_rx_signal_stuff(struct ieee80211_hw *hw,
				      struct sk_buff *skb,
<<<<<<< HEAD
				      struct rtl_stats *pstatus, __le32 *pdesc,
=======
				      struct rtl_stats *pstatus, u8 *pdesc,
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
				      struct rx_fwinfo_8821ae *p_drvinfo)
{
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	struct rtl_efuse *rtlefuse = rtl_efuse(rtl_priv(hw));
	struct ieee80211_hdr *hdr;
	u8 *tmp_buf;
	u8 *praddr;
	u8 *psaddr;
	__le16 fc;
<<<<<<< HEAD
=======
	u16 type;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	bool packet_matchbssid, packet_toself, packet_beacon;

	tmp_buf = skb->data + pstatus->rx_drvinfo_size + pstatus->rx_bufshift;

	hdr = (struct ieee80211_hdr *)tmp_buf;
	fc = hdr->frame_control;
<<<<<<< HEAD
=======
	type = WLAN_FC_GET_TYPE(hdr->frame_control);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	praddr = hdr->addr1;
	psaddr = ieee80211_get_SA(hdr);
	ether_addr_copy(pstatus->psaddr, psaddr);

	packet_matchbssid = (!ieee80211_is_ctl(fc) &&
			     (ether_addr_equal(mac->bssid,
					       ieee80211_has_tods(fc) ?
					       hdr->addr1 :
					       ieee80211_has_fromds(fc) ?
					       hdr->addr2 : hdr->addr3)) &&
			      (!pstatus->hwerror) &&
			      (!pstatus->crc) && (!pstatus->icv));

	packet_toself = packet_matchbssid &&
	    (ether_addr_equal(praddr, rtlefuse->dev_addr));

	if (ieee80211_is_beacon(hdr->frame_control))
		packet_beacon = true;
	else
		packet_beacon = false;

	if (packet_beacon && packet_matchbssid)
		rtl_priv(hw)->dm.dbginfo.num_qry_beacon_pkt++;

	if (packet_matchbssid &&
	    ieee80211_is_data_qos(hdr->frame_control) &&
	    !is_multicast_ether_addr(ieee80211_get_DA(hdr))) {
		struct ieee80211_qos_hdr *hdr_qos =
			(struct ieee80211_qos_hdr *)tmp_buf;
		u16 tid = le16_to_cpu(hdr_qos->qos_ctrl) & 0xf;

		if (tid != 0 && tid != 3)
			rtl_priv(hw)->dm.dbginfo.num_non_be_pkt++;
	}

	query_rxphystatus(hw, pstatus, pdesc, p_drvinfo,
			  packet_matchbssid, packet_toself,
			  packet_beacon);
	/*_rtl8821ae_smart_antenna(hw, pstatus); */
	rtl_process_phyinfo(hw, tmp_buf, pstatus);
}

<<<<<<< HEAD
static void rtl8821ae_insert_emcontent(struct rtl_tcb_desc *ptcb_desc,
				       __le32 *virtualaddress)
=======
static void _rtl8821ae_insert_emcontent(struct rtl_tcb_desc *ptcb_desc,
					u8 *virtualaddress)
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
{
	u32 dwtmp = 0;

	memset(virtualaddress, 0, 8);

<<<<<<< HEAD
	set_earlymode_pktnum(virtualaddress, ptcb_desc->empkt_num);
=======
	SET_EARLYMODE_PKTNUM(virtualaddress, ptcb_desc->empkt_num);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	if (ptcb_desc->empkt_num == 1) {
		dwtmp = ptcb_desc->empkt_len[0];
	} else {
		dwtmp = ptcb_desc->empkt_len[0];
		dwtmp += ((dwtmp % 4) ? (4 - dwtmp % 4) : 0)+4;
		dwtmp += ptcb_desc->empkt_len[1];
	}
<<<<<<< HEAD
	set_earlymode_len0(virtualaddress, dwtmp);
=======
	SET_EARLYMODE_LEN0(virtualaddress, dwtmp);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	if (ptcb_desc->empkt_num <= 3) {
		dwtmp = ptcb_desc->empkt_len[2];
	} else {
		dwtmp = ptcb_desc->empkt_len[2];
		dwtmp += ((dwtmp % 4) ? (4 - dwtmp % 4) : 0)+4;
		dwtmp += ptcb_desc->empkt_len[3];
	}
<<<<<<< HEAD
	set_earlymode_len1(virtualaddress, dwtmp);
=======
	SET_EARLYMODE_LEN1(virtualaddress, dwtmp);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	if (ptcb_desc->empkt_num <= 5) {
		dwtmp = ptcb_desc->empkt_len[4];
	} else {
		dwtmp = ptcb_desc->empkt_len[4];
		dwtmp += ((dwtmp % 4) ? (4 - dwtmp % 4) : 0)+4;
		dwtmp += ptcb_desc->empkt_len[5];
	}
<<<<<<< HEAD
	set_earlymode_len2_1(virtualaddress, dwtmp & 0xF);
	set_earlymode_len2_2(virtualaddress, dwtmp >> 4);
=======
	SET_EARLYMODE_LEN2_1(virtualaddress, dwtmp & 0xF);
	SET_EARLYMODE_LEN2_2(virtualaddress, dwtmp >> 4);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	if (ptcb_desc->empkt_num <= 7) {
		dwtmp = ptcb_desc->empkt_len[6];
	} else {
		dwtmp = ptcb_desc->empkt_len[6];
		dwtmp += ((dwtmp % 4) ? (4 - dwtmp % 4) : 0)+4;
		dwtmp += ptcb_desc->empkt_len[7];
	}
<<<<<<< HEAD
	set_earlymode_len3(virtualaddress, dwtmp);
=======
	SET_EARLYMODE_LEN3(virtualaddress, dwtmp);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	if (ptcb_desc->empkt_num <= 9) {
		dwtmp = ptcb_desc->empkt_len[8];
	} else {
		dwtmp = ptcb_desc->empkt_len[8];
		dwtmp += ((dwtmp % 4) ? (4 - dwtmp % 4) : 0)+4;
		dwtmp += ptcb_desc->empkt_len[9];
	}
<<<<<<< HEAD
	set_earlymode_len4(virtualaddress, dwtmp);
}

static bool rtl8821ae_get_rxdesc_is_ht(struct ieee80211_hw *hw, __le32 *pdesc)
=======
	SET_EARLYMODE_LEN4(virtualaddress, dwtmp);
}

static bool rtl8821ae_get_rxdesc_is_ht(struct ieee80211_hw *hw, u8 *pdesc)
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	u8 rx_rate = 0;

<<<<<<< HEAD
	rx_rate = get_rx_desc_rxmcs(pdesc);

	rtl_dbg(rtlpriv, COMP_RXDESC, DBG_LOUD, "rx_rate=0x%02x.\n", rx_rate);
=======
	rx_rate = GET_RX_DESC_RXMCS(pdesc);

	RT_TRACE(rtlpriv, COMP_RXDESC, DBG_LOUD, "rx_rate=0x%02x.\n", rx_rate);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	if ((rx_rate >= DESC_RATEMCS0) && (rx_rate <= DESC_RATEMCS15))
		return true;
	return false;
}

<<<<<<< HEAD
static bool rtl8821ae_get_rxdesc_is_vht(struct ieee80211_hw *hw, __le32 *pdesc)
=======
static bool rtl8821ae_get_rxdesc_is_vht(struct ieee80211_hw *hw, u8 *pdesc)
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	u8 rx_rate = 0;

<<<<<<< HEAD
	rx_rate = get_rx_desc_rxmcs(pdesc);

	rtl_dbg(rtlpriv, COMP_RXDESC, DBG_LOUD, "rx_rate=0x%02x.\n", rx_rate);
=======
	rx_rate = GET_RX_DESC_RXMCS(pdesc);

	RT_TRACE(rtlpriv, COMP_RXDESC, DBG_LOUD, "rx_rate=0x%02x.\n", rx_rate);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	if (rx_rate >= DESC_RATEVHT1SS_MCS0)
		return true;
	return false;
}

<<<<<<< HEAD
static u8 rtl8821ae_get_rx_vht_nss(struct ieee80211_hw *hw, __le32 *pdesc)
=======
static u8 rtl8821ae_get_rx_vht_nss(struct ieee80211_hw *hw, u8 *pdesc)
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
{
	u8 rx_rate = 0;
	u8 vht_nss = 0;

<<<<<<< HEAD
	rx_rate = get_rx_desc_rxmcs(pdesc);
=======
	rx_rate = GET_RX_DESC_RXMCS(pdesc);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	if ((rx_rate >= DESC_RATEVHT1SS_MCS0) &&
	    (rx_rate <= DESC_RATEVHT1SS_MCS9))
		vht_nss = 1;
	else if ((rx_rate >= DESC_RATEVHT2SS_MCS0) &&
		 (rx_rate <= DESC_RATEVHT2SS_MCS9))
		vht_nss = 2;

	return vht_nss;
}

bool rtl8821ae_rx_query_desc(struct ieee80211_hw *hw,
			     struct rtl_stats *status,
			     struct ieee80211_rx_status *rx_status,
<<<<<<< HEAD
			     u8 *pdesc8, struct sk_buff *skb)
=======
			     u8 *pdesc, struct sk_buff *skb)
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rx_fwinfo_8821ae *p_drvinfo;
	struct ieee80211_hdr *hdr;
<<<<<<< HEAD
	u8 wake_match;
	__le32 *pdesc = (__le32 *)pdesc8;
	u32 phystatus = get_rx_desc_physt(pdesc);

	status->length = (u16)get_rx_desc_pkt_len(pdesc);
	status->rx_drvinfo_size = (u8)get_rx_desc_drv_info_size(pdesc) *
	    RX_DRV_INFO_SIZE_UNIT;
	status->rx_bufshift = (u8)(get_rx_desc_shift(pdesc) & 0x03);
	status->icv = (u16)get_rx_desc_icv(pdesc);
	status->crc = (u16)get_rx_desc_crc32(pdesc);
	status->hwerror = (status->crc | status->icv);
	status->decrypted = !get_rx_desc_swdec(pdesc);
	status->rate = (u8)get_rx_desc_rxmcs(pdesc);
	status->shortpreamble = (u16)get_rx_desc_splcp(pdesc);
	status->isampdu = (bool)(get_rx_desc_paggr(pdesc) == 1);
	status->isfirst_ampdu = (bool)(get_rx_desc_paggr(pdesc) == 1);
	status->timestamp_low = get_rx_desc_tsfl(pdesc);
	status->rx_packet_bw = get_rx_desc_bw(pdesc);
	status->macid = get_rx_desc_macid(pdesc);
	status->is_short_gi = !(bool)get_rx_desc_splcp(pdesc);
=======

	u32 phystatus = GET_RX_DESC_PHYST(pdesc);

	status->length = (u16)GET_RX_DESC_PKT_LEN(pdesc);
	status->rx_drvinfo_size = (u8)GET_RX_DESC_DRV_INFO_SIZE(pdesc) *
	    RX_DRV_INFO_SIZE_UNIT;
	status->rx_bufshift = (u8)(GET_RX_DESC_SHIFT(pdesc) & 0x03);
	status->icv = (u16)GET_RX_DESC_ICV(pdesc);
	status->crc = (u16)GET_RX_DESC_CRC32(pdesc);
	status->hwerror = (status->crc | status->icv);
	status->decrypted = !GET_RX_DESC_SWDEC(pdesc);
	status->rate = (u8)GET_RX_DESC_RXMCS(pdesc);
	status->shortpreamble = (u16)GET_RX_DESC_SPLCP(pdesc);
	status->isampdu = (bool)(GET_RX_DESC_PAGGR(pdesc) == 1);
	status->isfirst_ampdu = (bool)(GET_RX_DESC_PAGGR(pdesc) == 1);
	status->timestamp_low = GET_RX_DESC_TSFL(pdesc);
	status->rx_packet_bw = GET_RX_DESC_BW(pdesc);
	status->macid = GET_RX_DESC_MACID(pdesc);
	status->is_short_gi = !(bool)GET_RX_DESC_SPLCP(pdesc);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	status->is_ht = rtl8821ae_get_rxdesc_is_ht(hw, pdesc);
	status->is_vht = rtl8821ae_get_rxdesc_is_vht(hw, pdesc);
	status->vht_nss = rtl8821ae_get_rx_vht_nss(hw, pdesc);
	status->is_cck = RTL8821AE_RX_HAL_IS_CCK_RATE(status->rate);

<<<<<<< HEAD
	rtl_dbg(rtlpriv, COMP_RXDESC, DBG_LOUD,
		"rx_packet_bw=%s,is_ht %d, is_vht %d, vht_nss=%d,is_short_gi %d.\n",
		(status->rx_packet_bw == 2) ? "80M" :
		(status->rx_packet_bw == 1) ? "40M" : "20M",
		status->is_ht, status->is_vht, status->vht_nss,
		status->is_short_gi);

	if (get_rx_status_desc_rpt_sel(pdesc))
=======
	RT_TRACE(rtlpriv, COMP_RXDESC, DBG_LOUD,
		 "rx_packet_bw=%s,is_ht %d, is_vht %d, vht_nss=%d,is_short_gi %d.\n",
		 (status->rx_packet_bw == 2) ? "80M" :
		 (status->rx_packet_bw == 1) ? "40M" : "20M",
		 status->is_ht, status->is_vht, status->vht_nss,
		 status->is_short_gi);

	if (GET_RX_STATUS_DESC_RPT_SEL(pdesc))
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		status->packet_report_type = C2H_PACKET;
	else
		status->packet_report_type = NORMAL_RX;

<<<<<<< HEAD
	if (get_rx_status_desc_pattern_match(pdesc))
		wake_match = BIT(2);
	else if (get_rx_status_desc_magic_match(pdesc))
		wake_match = BIT(1);
	else if (get_rx_status_desc_unicast_match(pdesc))
		wake_match = BIT(0);
	else
		wake_match = 0;

	if (wake_match)
		rtl_dbg(rtlpriv, COMP_RXDESC, DBG_LOUD,
			"GGGGGGGGGGGGGet Wakeup Packet!! WakeMatch=%d\n",
			wake_match);
=======
	if (GET_RX_STATUS_DESC_PATTERN_MATCH(pdesc))
		status->wake_match = BIT(2);
	else if (GET_RX_STATUS_DESC_MAGIC_MATCH(pdesc))
		status->wake_match = BIT(1);
	else if (GET_RX_STATUS_DESC_UNICAST_MATCH(pdesc))
		status->wake_match = BIT(0);
	else
		status->wake_match = 0;

	if (status->wake_match)
		RT_TRACE(rtlpriv, COMP_RXDESC, DBG_LOUD,
			 "GGGGGGGGGGGGGet Wakeup Packet!! WakeMatch=%d\n",
			 status->wake_match);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	rx_status->freq = hw->conf.chandef.chan->center_freq;
	rx_status->band = hw->conf.chandef.chan->band;

	hdr = (struct ieee80211_hdr *)(skb->data +
	      status->rx_drvinfo_size + status->rx_bufshift);

	if (status->crc)
		rx_status->flag |= RX_FLAG_FAILED_FCS_CRC;

	if (status->rx_packet_bw == HT_CHANNEL_WIDTH_20_40)
<<<<<<< HEAD
		rx_status->bw = RATE_INFO_BW_40;
	else if (status->rx_packet_bw == HT_CHANNEL_WIDTH_80)
		rx_status->bw = RATE_INFO_BW_80;
	if (status->is_ht)
		rx_status->encoding = RX_ENC_HT;
	if (status->is_vht)
		rx_status->encoding = RX_ENC_VHT;

	if (status->is_short_gi)
		rx_status->enc_flags |= RX_ENC_FLAG_SHORT_GI;

	rx_status->nss = status->vht_nss;
=======
		rx_status->flag |= RX_FLAG_40MHZ;
	else if (status->rx_packet_bw == HT_CHANNEL_WIDTH_80)
		rx_status->vht_flag |= RX_VHT_FLAG_80MHZ;
	if (status->is_ht)
		rx_status->flag |= RX_FLAG_HT;
	if (status->is_vht)
		rx_status->flag |= RX_FLAG_VHT;

	if (status->is_short_gi)
		rx_status->flag |= RX_FLAG_SHORT_GI;

	rx_status->vht_nss = status->vht_nss;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	rx_status->flag |= RX_FLAG_MACTIME_START;

	/* hw will set status->decrypted true, if it finds the
	 * frame is open data frame or mgmt frame.
	 * So hw will not decryption robust managment frame
	 * for IEEE80211w but still set status->decrypted
	 * true, so here we should set it back to undecrypted
	 * for IEEE80211w frame, and mac80211 sw will help
	 * to decrypt it
	 */
	if (status->decrypted) {
		if ((!_ieee80211_is_robust_mgmt_frame(hdr)) &&
		    (ieee80211_has_protected(hdr->frame_control)))
			rx_status->flag |= RX_FLAG_DECRYPTED;
		else
			rx_status->flag &= ~RX_FLAG_DECRYPTED;
	}

	/* rate_idx: index of data rate into band's
	 * supported rates or MCS index if HT rates
	 * are use (RX_FLAG_HT)
	 */
	rx_status->rate_idx = rtlwifi_rate_mapping(hw, status->is_ht,
						   status->is_vht,
						   status->rate);

	rx_status->mactime = status->timestamp_low;
	if (phystatus) {
		p_drvinfo = (struct rx_fwinfo_8821ae *)(skb->data +
			    status->rx_bufshift);

		translate_rx_signal_stuff(hw, skb, status, pdesc, p_drvinfo);
	}
	rx_status->signal = status->recvsignalpower + 10;
	if (status->packet_report_type == TX_REPORT2) {
		status->macid_valid_entry[0] =
<<<<<<< HEAD
		  get_rx_rpt2_desc_macid_valid_1(pdesc);
		status->macid_valid_entry[1] =
		  get_rx_rpt2_desc_macid_valid_2(pdesc);
=======
		  GET_RX_RPT2_DESC_MACID_VALID_1(pdesc);
		status->macid_valid_entry[1] =
		  GET_RX_RPT2_DESC_MACID_VALID_2(pdesc);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	}
	return true;
}

static u8 rtl8821ae_bw_mapping(struct ieee80211_hw *hw,
			       struct rtl_tcb_desc *ptcb_desc)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_phy *rtlphy = &rtlpriv->phy;
	u8 bw_setting_of_desc = 0;

<<<<<<< HEAD
	rtl_dbg(rtlpriv, COMP_SEND, DBG_TRACE,
		"%s, current_chan_bw %d, packet_bw %d\n",
		__func__,
		rtlphy->current_chan_bw, ptcb_desc->packet_bw);
=======
	RT_TRACE(rtlpriv, COMP_SEND, DBG_TRACE,
		 "rtl8821ae_bw_mapping, current_chan_bw %d, packet_bw %d\n",
		 rtlphy->current_chan_bw, ptcb_desc->packet_bw);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	if (rtlphy->current_chan_bw == HT_CHANNEL_WIDTH_80) {
		if (ptcb_desc->packet_bw == HT_CHANNEL_WIDTH_80)
			bw_setting_of_desc = 2;
		else if (ptcb_desc->packet_bw == HT_CHANNEL_WIDTH_20_40)
			bw_setting_of_desc = 1;
		else
			bw_setting_of_desc = 0;
	} else if (rtlphy->current_chan_bw == HT_CHANNEL_WIDTH_20_40) {
		if ((ptcb_desc->packet_bw == HT_CHANNEL_WIDTH_20_40) ||
		    (ptcb_desc->packet_bw == HT_CHANNEL_WIDTH_80))
			bw_setting_of_desc = 1;
		else
			bw_setting_of_desc = 0;
	} else {
		bw_setting_of_desc = 0;
	}
	return bw_setting_of_desc;
}

static u8 rtl8821ae_sc_mapping(struct ieee80211_hw *hw,
			       struct rtl_tcb_desc *ptcb_desc)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_phy *rtlphy = &rtlpriv->phy;
	struct rtl_mac *mac = rtl_mac(rtlpriv);
	u8 sc_setting_of_desc = 0;

	if (rtlphy->current_chan_bw == HT_CHANNEL_WIDTH_80) {
		if (ptcb_desc->packet_bw == HT_CHANNEL_WIDTH_80) {
			sc_setting_of_desc = VHT_DATA_SC_DONOT_CARE;
		} else if (ptcb_desc->packet_bw == HT_CHANNEL_WIDTH_20_40) {
			if (mac->cur_80_prime_sc ==
			    HAL_PRIME_CHNL_OFFSET_LOWER)
				sc_setting_of_desc =
					VHT_DATA_SC_40_LOWER_OF_80MHZ;
			else if (mac->cur_80_prime_sc ==
				 HAL_PRIME_CHNL_OFFSET_UPPER)
				sc_setting_of_desc =
					VHT_DATA_SC_40_UPPER_OF_80MHZ;
			else
<<<<<<< HEAD
				rtl_dbg(rtlpriv, COMP_SEND, DBG_LOUD,
					"%s: Not Correct Primary40MHz Setting\n",
					__func__);
=======
				RT_TRACE(rtlpriv, COMP_SEND, DBG_LOUD,
					 "rtl8821ae_sc_mapping: Not Correct Primary40MHz Setting\n");
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		} else {
			if ((mac->cur_40_prime_sc ==
			     HAL_PRIME_CHNL_OFFSET_LOWER) &&
			    (mac->cur_80_prime_sc  ==
			     HAL_PRIME_CHNL_OFFSET_LOWER))
				sc_setting_of_desc =
					VHT_DATA_SC_20_LOWEST_OF_80MHZ;
			else if ((mac->cur_40_prime_sc ==
				  HAL_PRIME_CHNL_OFFSET_UPPER) &&
				 (mac->cur_80_prime_sc ==
				  HAL_PRIME_CHNL_OFFSET_LOWER))
				sc_setting_of_desc =
					VHT_DATA_SC_20_LOWER_OF_80MHZ;
			else if ((mac->cur_40_prime_sc ==
				  HAL_PRIME_CHNL_OFFSET_LOWER) &&
				 (mac->cur_80_prime_sc ==
				  HAL_PRIME_CHNL_OFFSET_UPPER))
				sc_setting_of_desc =
					VHT_DATA_SC_20_UPPER_OF_80MHZ;
			else if ((mac->cur_40_prime_sc ==
				  HAL_PRIME_CHNL_OFFSET_UPPER) &&
				 (mac->cur_80_prime_sc ==
				  HAL_PRIME_CHNL_OFFSET_UPPER))
				sc_setting_of_desc =
					VHT_DATA_SC_20_UPPERST_OF_80MHZ;
			else
<<<<<<< HEAD
				rtl_dbg(rtlpriv, COMP_SEND, DBG_LOUD,
					"%s: Not Correct Primary40MHz Setting\n",
					__func__);
=======
				RT_TRACE(rtlpriv, COMP_SEND, DBG_LOUD,
					 "rtl8821ae_sc_mapping: Not Correct Primary40MHz Setting\n");
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		}
	} else if (rtlphy->current_chan_bw == HT_CHANNEL_WIDTH_20_40) {
		if (ptcb_desc->packet_bw == HT_CHANNEL_WIDTH_20_40) {
			sc_setting_of_desc = VHT_DATA_SC_DONOT_CARE;
		} else if (ptcb_desc->packet_bw == HT_CHANNEL_WIDTH_20) {
			if (mac->cur_40_prime_sc ==
				HAL_PRIME_CHNL_OFFSET_UPPER) {
				sc_setting_of_desc =
					VHT_DATA_SC_20_UPPER_OF_80MHZ;
			} else if (mac->cur_40_prime_sc ==
				HAL_PRIME_CHNL_OFFSET_LOWER){
				sc_setting_of_desc =
					VHT_DATA_SC_20_LOWER_OF_80MHZ;
			} else {
				sc_setting_of_desc = VHT_DATA_SC_DONOT_CARE;
			}
		}
	} else {
		sc_setting_of_desc = VHT_DATA_SC_DONOT_CARE;
	}

	return sc_setting_of_desc;
}

void rtl8821ae_tx_fill_desc(struct ieee80211_hw *hw,
<<<<<<< HEAD
			    struct ieee80211_hdr *hdr, u8 *pdesc8, u8 *txbd,
=======
			    struct ieee80211_hdr *hdr, u8 *pdesc_tx, u8 *txbd,
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
			    struct ieee80211_tx_info *info,
			    struct ieee80211_sta *sta,
			    struct sk_buff *skb,
			    u8 hw_queue, struct rtl_tcb_desc *ptcb_desc)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	struct rtl_pci *rtlpci = rtl_pcidev(rtl_pcipriv(hw));
	struct rtl_hal *rtlhal = rtl_hal(rtlpriv);
<<<<<<< HEAD
	struct rtlwifi_tx_info *tx_info = rtl_tx_skb_cb_info(skb);
=======
	u8 *pdesc = (u8 *)pdesc_tx;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	u16 seq_number;
	__le16 fc = hdr->frame_control;
	unsigned int buf_len = 0;
	unsigned int skb_len = skb->len;
	u8 fw_qsel = _rtl8821ae_map_hwqueue_to_fwqueue(skb, hw_queue);
	bool firstseg = ((hdr->seq_ctrl &
			  cpu_to_le16(IEEE80211_SCTL_FRAG)) == 0);
	bool lastseg = ((hdr->frame_control &
			 cpu_to_le16(IEEE80211_FCTL_MOREFRAGS)) == 0);
	dma_addr_t mapping;
	u8 short_gi = 0;
<<<<<<< HEAD
	bool tmp_bool;
	__le32 *pdesc = (__le32 *)pdesc8;
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	seq_number = (le16_to_cpu(hdr->seq_ctrl) & IEEE80211_SCTL_SEQ) >> 4;
	rtl_get_tcb_desc(hw, info, sta, skb, ptcb_desc);
	/* reserve 8 byte for AMPDU early mode */
	if (rtlhal->earlymode_enable) {
		skb_push(skb, EM_HDR_LEN);
		memset(skb->data, 0, EM_HDR_LEN);
	}
	buf_len = skb->len;
<<<<<<< HEAD
	mapping = dma_map_single(&rtlpci->pdev->dev, skb->data, skb->len,
				 DMA_TO_DEVICE);
	if (dma_mapping_error(&rtlpci->pdev->dev, mapping)) {
		rtl_dbg(rtlpriv, COMP_SEND, DBG_TRACE,
			"DMA mapping error\n");
		return;
	}
	clear_pci_tx_desc_content(pdesc, sizeof(struct tx_desc_8821ae));
=======
	mapping = pci_map_single(rtlpci->pdev, skb->data, skb->len,
				 PCI_DMA_TODEVICE);
	if (pci_dma_mapping_error(rtlpci->pdev, mapping)) {
		RT_TRACE(rtlpriv, COMP_SEND, DBG_TRACE,
			 "DMA mapping error\n");
		return;
	}
	CLEAR_PCI_TX_DESC_CONTENT(pdesc, sizeof(struct tx_desc_8821ae));
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	if (ieee80211_is_nullfunc(fc) || ieee80211_is_ctl(fc)) {
		firstseg = true;
		lastseg = true;
	}
	if (firstseg) {
		if (rtlhal->earlymode_enable) {
<<<<<<< HEAD
			set_tx_desc_pkt_offset(pdesc, 1);
			set_tx_desc_offset(pdesc, USB_HWDESC_HEADER_LEN +
					   EM_HDR_LEN);
			if (ptcb_desc->empkt_num) {
				rtl_dbg(rtlpriv, COMP_SEND, DBG_TRACE,
					"Insert 8 byte.pTcb->EMPktNum:%d\n",
					ptcb_desc->empkt_num);
				rtl8821ae_insert_emcontent(ptcb_desc,
							   (__le32 *)skb->data);
			}
		} else {
			set_tx_desc_offset(pdesc, USB_HWDESC_HEADER_LEN);
		}


		/* ptcb_desc->use_driver_rate = true; */
		set_tx_desc_tx_rate(pdesc, ptcb_desc->hw_rate);
=======
			SET_TX_DESC_PKT_OFFSET(pdesc, 1);
			SET_TX_DESC_OFFSET(pdesc, USB_HWDESC_HEADER_LEN +
					   EM_HDR_LEN);
			if (ptcb_desc->empkt_num) {
				RT_TRACE(rtlpriv, COMP_SEND, DBG_TRACE,
					 "Insert 8 byte.pTcb->EMPktNum:%d\n",
					  ptcb_desc->empkt_num);
				_rtl8821ae_insert_emcontent(ptcb_desc,
					 (u8 *)(skb->data));
			}
		} else {
			SET_TX_DESC_OFFSET(pdesc, USB_HWDESC_HEADER_LEN);
		}

		/* ptcb_desc->use_driver_rate = true; */
		SET_TX_DESC_TX_RATE(pdesc, ptcb_desc->hw_rate);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		if (ptcb_desc->hw_rate > DESC_RATEMCS0)
			short_gi = (ptcb_desc->use_shortgi) ? 1 : 0;
		else
			short_gi = (ptcb_desc->use_shortpreamble) ? 1 : 0;

<<<<<<< HEAD
		set_tx_desc_data_shortgi(pdesc, short_gi);

		if (info->flags & IEEE80211_TX_CTL_AMPDU) {
			set_tx_desc_agg_enable(pdesc, 1);
			set_tx_desc_max_agg_num(pdesc, 0x1f);
		}
		set_tx_desc_seq(pdesc, seq_number);
		set_tx_desc_rts_enable(pdesc,
				       ((ptcb_desc->rts_enable &&
					!ptcb_desc->cts_enable) ? 1 : 0));
		set_tx_desc_hw_rts_enable(pdesc, 0);
		set_tx_desc_cts2self(pdesc, ((ptcb_desc->cts_enable) ? 1 : 0));

		set_tx_desc_rts_rate(pdesc, ptcb_desc->rts_rate);
		set_tx_desc_rts_sc(pdesc, ptcb_desc->rts_sc);
		tmp_bool = ((ptcb_desc->rts_rate <= DESC_RATE54M) ?
			    (ptcb_desc->rts_use_shortpreamble ? 1 : 0) :
			    (ptcb_desc->rts_use_shortgi ? 1 : 0));
		set_tx_desc_rts_short(pdesc, tmp_bool);

		if (ptcb_desc->tx_enable_sw_calc_duration)
			set_tx_desc_nav_use_hdr(pdesc, 1);

		set_tx_desc_data_bw(pdesc,
				    rtl8821ae_bw_mapping(hw, ptcb_desc));

		set_tx_desc_tx_sub_carrier(pdesc,
					   rtl8821ae_sc_mapping(hw, ptcb_desc));

		set_tx_desc_linip(pdesc, 0);
		set_tx_desc_pkt_size(pdesc, (u16)skb_len);
		if (sta) {
			u8 ampdu_density = sta->deflink.ht_cap.ampdu_density;

			set_tx_desc_ampdu_density(pdesc, ampdu_density);
=======
		SET_TX_DESC_DATA_SHORTGI(pdesc, short_gi);

		if (info->flags & IEEE80211_TX_CTL_AMPDU) {
			SET_TX_DESC_AGG_ENABLE(pdesc, 1);
			SET_TX_DESC_MAX_AGG_NUM(pdesc, 0x1f);
		}
		SET_TX_DESC_SEQ(pdesc, seq_number);
		SET_TX_DESC_RTS_ENABLE(pdesc, ((ptcb_desc->rts_enable &&
					!ptcb_desc->cts_enable) ? 1 : 0));
		SET_TX_DESC_HW_RTS_ENABLE(pdesc, 0);
		SET_TX_DESC_CTS2SELF(pdesc, ((ptcb_desc->cts_enable) ? 1 : 0));

		SET_TX_DESC_RTS_RATE(pdesc, ptcb_desc->rts_rate);
		SET_TX_DESC_RTS_SC(pdesc, ptcb_desc->rts_sc);
		SET_TX_DESC_RTS_SHORT(pdesc,
			((ptcb_desc->rts_rate <= DESC_RATE54M) ?
			(ptcb_desc->rts_use_shortpreamble ? 1 : 0) :
			(ptcb_desc->rts_use_shortgi ? 1 : 0)));

		if (ptcb_desc->tx_enable_sw_calc_duration)
			SET_TX_DESC_NAV_USE_HDR(pdesc, 1);

		SET_TX_DESC_DATA_BW(pdesc,
			rtl8821ae_bw_mapping(hw, ptcb_desc));

		SET_TX_DESC_TX_SUB_CARRIER(pdesc,
			rtl8821ae_sc_mapping(hw, ptcb_desc));

		SET_TX_DESC_LINIP(pdesc, 0);
		SET_TX_DESC_PKT_SIZE(pdesc, (u16)skb_len);
		if (sta) {
			u8 ampdu_density = sta->ht_cap.ampdu_density;

			SET_TX_DESC_AMPDU_DENSITY(pdesc, ampdu_density);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		}
		if (info->control.hw_key) {
			struct ieee80211_key_conf *keyconf =
				info->control.hw_key;
			switch (keyconf->cipher) {
			case WLAN_CIPHER_SUITE_WEP40:
			case WLAN_CIPHER_SUITE_WEP104:
			case WLAN_CIPHER_SUITE_TKIP:
<<<<<<< HEAD
				set_tx_desc_sec_type(pdesc, 0x1);
				break;
			case WLAN_CIPHER_SUITE_CCMP:
				set_tx_desc_sec_type(pdesc, 0x3);
				break;
			default:
				set_tx_desc_sec_type(pdesc, 0x0);
=======
				SET_TX_DESC_SEC_TYPE(pdesc, 0x1);
				break;
			case WLAN_CIPHER_SUITE_CCMP:
				SET_TX_DESC_SEC_TYPE(pdesc, 0x3);
				break;
			default:
				SET_TX_DESC_SEC_TYPE(pdesc, 0x0);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
				break;
			}
		}

<<<<<<< HEAD
		set_tx_desc_queue_sel(pdesc, fw_qsel);
		set_tx_desc_data_rate_fb_limit(pdesc, 0x1F);
		set_tx_desc_rts_rate_fb_limit(pdesc, 0xF);
		set_tx_desc_disable_fb(pdesc, ptcb_desc->disable_ratefallback ?
				       1 : 0);
		set_tx_desc_use_rate(pdesc, ptcb_desc->use_driver_rate ? 1 : 0);

		if (ieee80211_is_data_qos(fc)) {
			if (mac->rdg_en) {
				rtl_dbg(rtlpriv, COMP_SEND, DBG_TRACE,
					"Enable RDG function.\n");
				set_tx_desc_rdg_enable(pdesc, 1);
				set_tx_desc_htc(pdesc, 1);
			}
		}
		/* tx report */
		rtl_set_tx_report(ptcb_desc, pdesc8, hw, tx_info);
	}

	set_tx_desc_first_seg(pdesc, (firstseg ? 1 : 0));
	set_tx_desc_last_seg(pdesc, (lastseg ? 1 : 0));
	set_tx_desc_tx_buffer_size(pdesc, buf_len);
	set_tx_desc_tx_buffer_address(pdesc, mapping);
	/* if (rtlpriv->dm.useramask) { */
	if (1) {
		set_tx_desc_rate_id(pdesc, ptcb_desc->ratr_index);
		set_tx_desc_macid(pdesc, ptcb_desc->mac_id);
	} else {
		set_tx_desc_rate_id(pdesc, 0xC + ptcb_desc->ratr_index);
		set_tx_desc_macid(pdesc, ptcb_desc->mac_id);
	}
	if (!ieee80211_is_data_qos(fc))  {
		set_tx_desc_hwseq_en(pdesc, 1);
		set_tx_desc_hwseq_sel(pdesc, 0);
	}
	set_tx_desc_more_frag(pdesc, (lastseg ? 0 : 1));
	if (is_multicast_ether_addr(ieee80211_get_DA(hdr)) ||
	    is_broadcast_ether_addr(ieee80211_get_DA(hdr))) {
		set_tx_desc_bmc(pdesc, 1);
	}

	rtl8821ae_dm_set_tx_ant_by_tx_info(hw, pdesc8, ptcb_desc->mac_id);
	rtl_dbg(rtlpriv, COMP_SEND, DBG_TRACE, "\n");
}

void rtl8821ae_tx_fill_cmddesc(struct ieee80211_hw *hw,
			       u8 *pdesc8, bool firstseg,
=======
		SET_TX_DESC_QUEUE_SEL(pdesc, fw_qsel);
		SET_TX_DESC_DATA_RATE_FB_LIMIT(pdesc, 0x1F);
		SET_TX_DESC_RTS_RATE_FB_LIMIT(pdesc, 0xF);
		SET_TX_DESC_DISABLE_FB(pdesc, ptcb_desc->disable_ratefallback ?
				       1 : 0);
		SET_TX_DESC_USE_RATE(pdesc, ptcb_desc->use_driver_rate ? 1 : 0);

		if (ieee80211_is_data_qos(fc)) {
			if (mac->rdg_en) {
				RT_TRACE(rtlpriv, COMP_SEND, DBG_TRACE,
					 "Enable RDG function.\n");
				SET_TX_DESC_RDG_ENABLE(pdesc, 1);
				SET_TX_DESC_HTC(pdesc, 1);
			}
		}
	}

	SET_TX_DESC_FIRST_SEG(pdesc, (firstseg ? 1 : 0));
	SET_TX_DESC_LAST_SEG(pdesc, (lastseg ? 1 : 0));
	SET_TX_DESC_TX_BUFFER_SIZE(pdesc, (u16)buf_len);
	SET_TX_DESC_TX_BUFFER_ADDRESS(pdesc, mapping);
	/* if (rtlpriv->dm.useramask) { */
	if (1) {
		SET_TX_DESC_RATE_ID(pdesc, ptcb_desc->ratr_index);
		SET_TX_DESC_MACID(pdesc, ptcb_desc->mac_id);
	} else {
		SET_TX_DESC_RATE_ID(pdesc, 0xC + ptcb_desc->ratr_index);
		SET_TX_DESC_MACID(pdesc, ptcb_desc->mac_id);
	}
	if (!ieee80211_is_data_qos(fc))  {
		SET_TX_DESC_HWSEQ_EN(pdesc, 1);
		SET_TX_DESC_HWSEQ_SEL(pdesc, 0);
	}
	SET_TX_DESC_MORE_FRAG(pdesc, (lastseg ? 0 : 1));
	if (is_multicast_ether_addr(ieee80211_get_DA(hdr)) ||
	    is_broadcast_ether_addr(ieee80211_get_DA(hdr))) {
		SET_TX_DESC_BMC(pdesc, 1);
	}

	rtl8821ae_dm_set_tx_ant_by_tx_info(hw, pdesc, ptcb_desc->mac_id);
	RT_TRACE(rtlpriv, COMP_SEND, DBG_TRACE, "\n");
}

void rtl8821ae_tx_fill_cmddesc(struct ieee80211_hw *hw,
			       u8 *pdesc, bool firstseg,
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
			       bool lastseg, struct sk_buff *skb)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_pci *rtlpci = rtl_pcidev(rtl_pcipriv(hw));
	u8 fw_queue = QSLT_BEACON;
<<<<<<< HEAD
	__le32 *pdesc = (__le32 *)pdesc8;

	dma_addr_t mapping = dma_map_single(&rtlpci->pdev->dev, skb->data,
					    skb->len, DMA_TO_DEVICE);

	if (dma_mapping_error(&rtlpci->pdev->dev, mapping)) {
		rtl_dbg(rtlpriv, COMP_SEND, DBG_TRACE,
			"DMA mapping error\n");
		return;
	}
	clear_pci_tx_desc_content(pdesc, TX_DESC_SIZE);

	set_tx_desc_first_seg(pdesc, 1);
	set_tx_desc_last_seg(pdesc, 1);

	set_tx_desc_pkt_size(pdesc, (u16)(skb->len));

	set_tx_desc_offset(pdesc, USB_HWDESC_HEADER_LEN);

	set_tx_desc_use_rate(pdesc, 1);
	set_tx_desc_tx_rate(pdesc, DESC_RATE1M);
	set_tx_desc_disable_fb(pdesc, 1);

	set_tx_desc_data_bw(pdesc, 0);

	set_tx_desc_hwseq_en(pdesc, 1);

	set_tx_desc_queue_sel(pdesc, fw_queue);

	set_tx_desc_tx_buffer_size(pdesc, skb->len);

	set_tx_desc_tx_buffer_address(pdesc, mapping);

	set_tx_desc_macid(pdesc, 0);

	set_tx_desc_own(pdesc, 1);

	RT_PRINT_DATA(rtlpriv, COMP_CMD, DBG_LOUD,
		      "H2C Tx Cmd Content\n",
		      pdesc8, TX_DESC_SIZE);
}

void rtl8821ae_set_desc(struct ieee80211_hw *hw, u8 *pdesc8,
			bool istx, u8 desc_name, u8 *val)
{
	__le32 *pdesc = (__le32 *)pdesc8;

	if (istx) {
		switch (desc_name) {
		case HW_DESC_OWN:
			set_tx_desc_own(pdesc, 1);
			break;
		case HW_DESC_TX_NEXTDESC_ADDR:
			set_tx_desc_next_desc_address(pdesc, *(u32 *)val);
			break;
		default:
			WARN_ONCE(true,
				  "rtl8821ae: ERR txdesc :%d not processed\n",
				  desc_name);
=======

	dma_addr_t mapping = pci_map_single(rtlpci->pdev,
					    skb->data, skb->len,
					    PCI_DMA_TODEVICE);

	if (pci_dma_mapping_error(rtlpci->pdev, mapping)) {
		RT_TRACE(rtlpriv, COMP_SEND, DBG_TRACE,
			 "DMA mapping error\n");
		return;
	}
	CLEAR_PCI_TX_DESC_CONTENT(pdesc, TX_DESC_SIZE);

	SET_TX_DESC_FIRST_SEG(pdesc, 1);
	SET_TX_DESC_LAST_SEG(pdesc, 1);

	SET_TX_DESC_PKT_SIZE((u8 *)pdesc, (u16)(skb->len));

	SET_TX_DESC_OFFSET(pdesc, USB_HWDESC_HEADER_LEN);

	SET_TX_DESC_USE_RATE(pdesc, 1);
	SET_TX_DESC_TX_RATE(pdesc, DESC_RATE1M);
	SET_TX_DESC_DISABLE_FB(pdesc, 1);

	SET_TX_DESC_DATA_BW(pdesc, 0);

	SET_TX_DESC_HWSEQ_EN(pdesc, 1);

	SET_TX_DESC_QUEUE_SEL(pdesc, fw_queue);

	SET_TX_DESC_TX_BUFFER_SIZE(pdesc, (u16)(skb->len));

	SET_TX_DESC_TX_BUFFER_ADDRESS(pdesc, mapping);

	SET_TX_DESC_MACID(pdesc, 0);

	SET_TX_DESC_OWN(pdesc, 1);

	RT_PRINT_DATA(rtlpriv, COMP_CMD, DBG_LOUD,
		      "H2C Tx Cmd Content\n",
		      pdesc, TX_DESC_SIZE);
}

void rtl8821ae_set_desc(struct ieee80211_hw *hw, u8 *pdesc,
			bool istx, u8 desc_name, u8 *val)
{
	if (istx) {
		switch (desc_name) {
		case HW_DESC_OWN:
			SET_TX_DESC_OWN(pdesc, 1);
			break;
		case HW_DESC_TX_NEXTDESC_ADDR:
			SET_TX_DESC_NEXT_DESC_ADDRESS(pdesc, *(u32 *)val);
			break;
		default:
			RT_ASSERT(false,
				  "ERR txdesc :%d not process\n", desc_name);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
			break;
		}
	} else {
		switch (desc_name) {
		case HW_DESC_RXOWN:
<<<<<<< HEAD
			set_rx_desc_own(pdesc, 1);
			break;
		case HW_DESC_RXBUFF_ADDR:
			set_rx_desc_buff_addr(pdesc, *(u32 *)val);
			break;
		case HW_DESC_RXPKT_LEN:
			set_rx_desc_pkt_len(pdesc, *(u32 *)val);
			break;
		case HW_DESC_RXERO:
			set_rx_desc_eor(pdesc, 1);
			break;
		default:
			WARN_ONCE(true,
				  "rtl8821ae: ERR rxdesc :%d not processed\n",
				  desc_name);
=======
			SET_RX_DESC_OWN(pdesc, 1);
			break;
		case HW_DESC_RXBUFF_ADDR:
			SET_RX_DESC_BUFF_ADDR(pdesc, *(u32 *)val);
			break;
		case HW_DESC_RXPKT_LEN:
			SET_RX_DESC_PKT_LEN(pdesc, *(u32 *)val);
			break;
		case HW_DESC_RXERO:
			SET_RX_DESC_EOR(pdesc, 1);
			break;
		default:
			RT_ASSERT(false,
				  "ERR rxdesc :%d not process\n", desc_name);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
			break;
		}
	}
}

<<<<<<< HEAD
u64 rtl8821ae_get_desc(struct ieee80211_hw *hw,
		       u8 *pdesc8, bool istx, u8 desc_name)
{
	u32 ret = 0;
	__le32 *pdesc = (__le32 *)pdesc8;
=======
u32 rtl8821ae_get_desc(u8 *pdesc, bool istx, u8 desc_name)
{
	u32 ret = 0;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	if (istx) {
		switch (desc_name) {
		case HW_DESC_OWN:
<<<<<<< HEAD
			ret = get_tx_desc_own(pdesc);
			break;
		case HW_DESC_TXBUFF_ADDR:
			ret = get_tx_desc_tx_buffer_address(pdesc);
			break;
		default:
			WARN_ONCE(true,
				  "rtl8821ae: ERR txdesc :%d not processed\n",
				  desc_name);
=======
			ret = GET_TX_DESC_OWN(pdesc);
			break;
		case HW_DESC_TXBUFF_ADDR:
			ret = GET_TX_DESC_TX_BUFFER_ADDRESS(pdesc);
			break;
		default:
			RT_ASSERT(false,
				  "ERR txdesc :%d not process\n", desc_name);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
			break;
		}
	} else {
		switch (desc_name) {
		case HW_DESC_OWN:
<<<<<<< HEAD
			ret = get_rx_desc_own(pdesc);
			break;
		case HW_DESC_RXPKT_LEN:
			ret = get_rx_desc_pkt_len(pdesc);
			break;
		case HW_DESC_RXBUFF_ADDR:
			ret = get_rx_desc_buff_addr(pdesc);
			break;
		default:
			WARN_ONCE(true,
				  "rtl8821ae: ERR rxdesc :%d not processed\n",
				  desc_name);
=======
			ret = GET_RX_DESC_OWN(pdesc);
			break;
		case HW_DESC_RXPKT_LEN:
			ret = GET_RX_DESC_PKT_LEN(pdesc);
			break;
		case HW_DESC_RXBUFF_ADDR:
			ret = GET_RX_DESC_BUFF_ADDR(pdesc);
			break;
		default:
			RT_ASSERT(false,
				  "ERR rxdesc :%d not process\n", desc_name);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
			break;
		}
	}
	return ret;
}

bool rtl8821ae_is_tx_desc_closed(struct ieee80211_hw *hw,
				 u8 hw_queue, u16 index)
{
	struct rtl_pci *rtlpci = rtl_pcidev(rtl_pcipriv(hw));
	struct rtl8192_tx_ring *ring = &rtlpci->tx_ring[hw_queue];
	u8 *entry = (u8 *)(&ring->desc[ring->idx]);
<<<<<<< HEAD
	u8 own = (u8)rtl8821ae_get_desc(hw, entry, true, HW_DESC_OWN);
=======
	u8 own = (u8)rtl8821ae_get_desc(entry, true, HW_DESC_OWN);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	/**
	 *beacon packet will only use the first
	 *descriptor defautly,and the own may not
	 *be cleared by the hardware
	 */
	if (own)
		return false;
	return true;
}

void rtl8821ae_tx_polling(struct ieee80211_hw *hw, u8 hw_queue)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);

	if (hw_queue == BEACON_QUEUE) {
		rtl_write_word(rtlpriv, REG_PCIE_CTRL_REG, BIT(4));
	} else {
		rtl_write_word(rtlpriv, REG_PCIE_CTRL_REG,
			       BIT(0) << (hw_queue));
	}
}
<<<<<<< HEAD
=======

u32 rtl8821ae_rx_command_packet(struct ieee80211_hw *hw,
				const struct rtl_stats *status,
				struct sk_buff *skb)
{
	u32 result = 0;
	struct rtl_priv *rtlpriv = rtl_priv(hw);

	switch (status->packet_report_type) {
	case NORMAL_RX:
		result = 0;
		break;
	case C2H_PACKET:
		rtl8821ae_c2h_packet_handler(hw, skb->data, (u8)skb->len);
		result = 1;
		RT_TRACE(rtlpriv, COMP_RECV, DBG_LOUD,
			 "skb->len=%d\n\n", skb->len);
		break;
	default:
		RT_TRACE(rtlpriv, COMP_RECV, DBG_LOUD,
			 "No this packet type!!\n");
		break;
	}

	return result;
}
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
