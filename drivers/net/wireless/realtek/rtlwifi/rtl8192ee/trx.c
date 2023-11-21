<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
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
#include "phy.h"
#include "trx.h"
#include "led.h"
#include "dm.h"
#include "fw.h"

static u8 _rtl92ee_map_hwqueue_to_fwqueue(struct sk_buff *skb, u8 hw_queue)
{
	__le16 fc = rtl_get_fc(skb);

	if (unlikely(ieee80211_is_beacon(fc)))
		return QSLT_BEACON;
	if (ieee80211_is_mgmt(fc) || ieee80211_is_ctl(fc))
		return QSLT_MGNT;

	return skb->priority;
}

static void _rtl92ee_query_rxphystatus(struct ieee80211_hw *hw,
				       struct rtl_stats *pstatus, u8 *pdesc,
				       struct rx_fwinfo *p_drvinfo,
				       bool bpacket_match_bssid,
				       bool bpacket_toself,
				       bool packet_beacon)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct phy_status_rpt *p_phystrpt = (struct phy_status_rpt *)p_drvinfo;
<<<<<<< HEAD
	s8 rx_pwr_all, rx_pwr[4];
=======
	s8 rx_pwr_all = 0, rx_pwr[4];
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	u8 rf_rx_num = 0, evm, pwdb_all;
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
		/* CCK Driver info Structure is not the same as OFDM packet. */
		cck_agc_rpt = p_phystrpt->cck_agc_rpt_ofdm_cfosho_a;

		/* (1)Hardware does not provide RSSI for CCK
<<<<<<< HEAD
		 * (2)PWDB, Average PWDB calculated by
=======
		 * (2)PWDB, Average PWDB cacluated by
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		 * hardware (for rate adaptive)
		 */
		cck_highpwr = (u8)rtl_get_bbreg(hw, RFPGA0_XA_HSSIPARAMETER2,
						 BIT(9));

		lan_idx = ((cck_agc_rpt & 0xE0) >> 5);
		vga_idx = (cck_agc_rpt & 0x1f);
		switch (lan_idx) {
		case 7: /*VGA_idx = 27~2*/
				if (vga_idx <= 27)
					rx_pwr_all = -100 + 2 * (27 - vga_idx);
				else
					rx_pwr_all = -100;
				break;
		case 6: /*VGA_idx = 2~0*/
				rx_pwr_all = -48 + 2 * (2 - vga_idx);
				break;
		case 5: /*VGA_idx = 7~5*/
				rx_pwr_all = -42 + 2 * (7 - vga_idx);
				break;
		case 4: /*VGA_idx = 7~4*/
				rx_pwr_all = -36 + 2 * (7 - vga_idx);
				break;
		case 3: /*VGA_idx = 7~0*/
				rx_pwr_all = -24 + 2 * (7 - vga_idx);
				break;
		case 2: /*VGA_idx = 5~0*/
				if (cck_highpwr)
					rx_pwr_all = -12 + 2 * (5 - vga_idx);
				else
					rx_pwr_all = -6 + 2 * (5 - vga_idx);
				break;
		case 1:
				rx_pwr_all = 8 - 2 * vga_idx;
				break;
		case 0:
				rx_pwr_all = 14 - 2 * vga_idx;
				break;
		default:
<<<<<<< HEAD
				rx_pwr_all = 0;
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
				break;
		}
		rx_pwr_all += 16;
		pwdb_all = rtl_query_rxpwrpercentage(rx_pwr_all);

		if (!cck_highpwr) {
			if (pwdb_all >= 80)
				pwdb_all = ((pwdb_all - 80) << 1) +
					   ((pwdb_all - 80) >> 1) + 80;
			else if ((pwdb_all <= 78) && (pwdb_all >= 20))
				pwdb_all += 3;
			if (pwdb_all > 100)
				pwdb_all = 100;
		}

		pstatus->rx_pwdb_all = pwdb_all;
		pstatus->bt_rx_rssi_percentage = pwdb_all;
		pstatus->recvsignalpower = rx_pwr_all;

		/* (3) Get Signal Quality (EVM) */
		if (bpacket_match_bssid) {
			u8 sq, sq_rpt;

			if (pstatus->rx_pwdb_all > 40) {
				sq = 100;
			} else {
				sq_rpt = p_phystrpt->cck_sig_qual_ofdm_pwdb_all;
				if (sq_rpt > 64)
					sq = 0;
				else if (sq_rpt < 20)
					sq = 100;
				else
					sq = ((64 - sq_rpt) * 100) / 44;
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

			rx_pwr[i] = ((p_phystrpt->path_agc[i].gain & 0x3f) * 2)
				    - 110;

			pstatus->rx_pwr[i] = rx_pwr[i];
			/* Translate DBM to percentage. */
			rssi = rtl_query_rxpwrpercentage(rx_pwr[i]);
			total_rssi += rssi;

			pstatus->rx_mimo_signalstrength[i] = (u8)rssi;
		}

<<<<<<< HEAD
		/* (2)PWDB, Average PWDB calculated by
=======
		/* (2)PWDB, Average PWDB cacluated by
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		 * hardware (for rate adaptive)
		 */
		rx_pwr_all = ((p_phystrpt->cck_sig_qual_ofdm_pwdb_all >> 1)
			      & 0x7f) - 110;

		pwdb_all = rtl_query_rxpwrpercentage(rx_pwr_all);
		pstatus->rx_pwdb_all = pwdb_all;
		pstatus->bt_rx_rssi_percentage = pwdb_all;
		pstatus->rxpower = rx_pwr_all;
		pstatus->recvsignalpower = rx_pwr_all;

		/* (3)EVM of HT rate */
		if (pstatus->rate >= DESC_RATEMCS8 &&
		    pstatus->rate <= DESC_RATEMCS15)
			max_spatial_stream = 2;
		else
			max_spatial_stream = 1;

		for (i = 0; i < max_spatial_stream; i++) {
			evm = rtl_evm_db_to_percentage(
						p_phystrpt->stream_rxevm[i]);

			if (bpacket_match_bssid) {
				/* Fill value in RFD, Get the first
				 * spatial stream only
				 */
				if (i == 0)
					pstatus->signalquality = (u8)(evm &
								       0xff);
				pstatus->rx_mimo_signalquality[i] = (u8)(evm &
									  0xff);
			}
		}

		if (bpacket_match_bssid) {
			for (i = RF90_PATH_A; i <= RF90_PATH_B; i++)
				rtl_priv(hw)->dm.cfo_tail[i] =
					(int)p_phystrpt->path_cfotail[i];

			if (rtl_priv(hw)->dm.packet_count == 0xffffffff)
				rtl_priv(hw)->dm.packet_count = 0;
			else
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
}

static void _rtl92ee_translate_rx_signal_stuff(struct ieee80211_hw *hw,
					       struct sk_buff *skb,
					       struct rtl_stats *pstatus,
					       u8 *pdesc,
					       struct rx_fwinfo *p_drvinfo)
{
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	struct rtl_efuse *rtlefuse = rtl_efuse(rtl_priv(hw));
	struct ieee80211_hdr *hdr;
	u8 *tmp_buf;
	u8 *praddr;
	u8 *psaddr;
	__le16 fc;
	bool packet_matchbssid, packet_toself, packet_beacon;

	tmp_buf = skb->data + pstatus->rx_drvinfo_size +
		  pstatus->rx_bufshift + 24;

	hdr = (struct ieee80211_hdr *)tmp_buf;
	fc = hdr->frame_control;
	praddr = hdr->addr1;
	psaddr = ieee80211_get_SA(hdr);
	ether_addr_copy(pstatus->psaddr, psaddr);

	packet_matchbssid = (!ieee80211_is_ctl(fc) &&
			       (ether_addr_equal(mac->bssid,
						ieee80211_has_tods(fc) ?
						hdr->addr1 :
						ieee80211_has_fromds(fc) ?
						hdr->addr2 : hdr->addr3)) &&
				(!pstatus->hwerror) && (!pstatus->crc) &&
				(!pstatus->icv));

	packet_toself = packet_matchbssid &&
			 (ether_addr_equal(praddr, rtlefuse->dev_addr));

	if (ieee80211_is_beacon(fc))
		packet_beacon = true;
	else
		packet_beacon = false;

	if (packet_beacon && packet_matchbssid)
		rtl_priv(hw)->dm.dbginfo.num_qry_beacon_pkt++;

	if (packet_matchbssid && ieee80211_is_data_qos(hdr->frame_control) &&
	    !is_multicast_ether_addr(ieee80211_get_DA(hdr))) {
		struct ieee80211_qos_hdr *hdr_qos =
					    (struct ieee80211_qos_hdr *)tmp_buf;
		u16 tid = le16_to_cpu(hdr_qos->qos_ctrl) & 0xf;

		if (tid != 0 && tid != 3)
			rtl_priv(hw)->dm.dbginfo.num_non_be_pkt++;
	}

	_rtl92ee_query_rxphystatus(hw, pstatus, pdesc, p_drvinfo,
				   packet_matchbssid, packet_toself,
				   packet_beacon);
	rtl_process_phyinfo(hw, tmp_buf, pstatus);
}

static void _rtl92ee_insert_emcontent(struct rtl_tcb_desc *ptcb_desc,
<<<<<<< HEAD
				      u8 *virtualaddress8)
{
	u32 dwtmp;
	__le32 *virtualaddress = (__le32 *)virtualaddress8;

	memset(virtualaddress, 0, 8);

	set_earlymode_pktnum(virtualaddress, ptcb_desc->empkt_num);
=======
				      u8 *virtualaddress)
{
	u32 dwtmp = 0;

	memset(virtualaddress, 0, 8);

	SET_EARLYMODE_PKTNUM(virtualaddress, ptcb_desc->empkt_num);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	if (ptcb_desc->empkt_num == 1) {
		dwtmp = ptcb_desc->empkt_len[0];
	} else {
		dwtmp = ptcb_desc->empkt_len[0];
		dwtmp += ((dwtmp % 4) ? (4 - dwtmp % 4) : 0) + 4;
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
		dwtmp += ((dwtmp % 4) ? (4 - dwtmp % 4) : 0) + 4;
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
		dwtmp += ((dwtmp % 4) ? (4 - dwtmp % 4) : 0) + 4;
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
		dwtmp += ((dwtmp % 4) ? (4 - dwtmp % 4) : 0) + 4;
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
		dwtmp += ((dwtmp % 4) ? (4 - dwtmp % 4) : 0) + 4;
		dwtmp += ptcb_desc->empkt_len[9];
	}
<<<<<<< HEAD
	set_earlymode_len4(virtualaddress, dwtmp);
=======
	SET_EARLYMODE_LEN4(virtualaddress, dwtmp);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
}

bool rtl92ee_rx_query_desc(struct ieee80211_hw *hw,
			   struct rtl_stats *status,
			   struct ieee80211_rx_status *rx_status,
<<<<<<< HEAD
			   u8 *pdesc8, struct sk_buff *skb)
=======
			   u8 *pdesc, struct sk_buff *skb)
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rx_fwinfo *p_drvinfo;
	struct ieee80211_hdr *hdr;
<<<<<<< HEAD
	__le32 *pdesc = (__le32 *)pdesc8;
	u32 phystatus = get_rx_desc_physt(pdesc);
	u8 wake_match;

	if (get_rx_status_desc_rpt_sel(pdesc) == 0)
		status->packet_report_type = NORMAL_RX;
	else
		status->packet_report_type = C2H_PACKET;
	status->length = (u16)get_rx_desc_pkt_len(pdesc);
	status->rx_drvinfo_size = (u8)get_rx_desc_drv_info_size(pdesc) *
				  RX_DRV_INFO_SIZE_UNIT;
	status->rx_bufshift = (u8)(get_rx_desc_shift(pdesc) & 0x03);
	status->icv = (u16)get_rx_desc_icv(pdesc);
	status->crc = (u16)get_rx_desc_crc32(pdesc);
	status->hwerror = (status->crc | status->icv);
	status->decrypted = !get_rx_desc_swdec(pdesc);
	status->rate = (u8)get_rx_desc_rxmcs(pdesc);
	status->isampdu = (bool)(get_rx_desc_paggr(pdesc) == 1);
	status->timestamp_low = get_rx_desc_tsfl(pdesc);
	status->is_cck = RTL92EE_RX_HAL_IS_CCK_RATE(status->rate);

	status->macid = get_rx_desc_macid(pdesc);
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
	u32 phystatus = GET_RX_DESC_PHYST(pdesc);

	if (GET_RX_STATUS_DESC_RPT_SEL(pdesc) == 0)
		status->packet_report_type = NORMAL_RX;
	else
		status->packet_report_type = C2H_PACKET;
	status->length = (u16)GET_RX_DESC_PKT_LEN(pdesc);
	status->rx_drvinfo_size = (u8)GET_RX_DESC_DRV_INFO_SIZE(pdesc) *
				  RX_DRV_INFO_SIZE_UNIT;
	status->rx_bufshift = (u8)(GET_RX_DESC_SHIFT(pdesc) & 0x03);
	status->icv = (u16)GET_RX_DESC_ICV(pdesc);
	status->crc = (u16)GET_RX_DESC_CRC32(pdesc);
	status->hwerror = (status->crc | status->icv);
	status->decrypted = !GET_RX_DESC_SWDEC(pdesc);
	status->rate = (u8)GET_RX_DESC_RXMCS(pdesc);
	status->isampdu = (bool)(GET_RX_DESC_PAGGR(pdesc) == 1);
	status->timestamp_low = GET_RX_DESC_TSFL(pdesc);
	status->is_cck = RTL92EE_RX_HAL_IS_CCK_RATE(status->rate);

	status->macid = GET_RX_DESC_MACID(pdesc);
	if (GET_RX_STATUS_DESC_MAGIC_MATCH(pdesc))
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

	hdr = (struct ieee80211_hdr *)(skb->data + status->rx_drvinfo_size +
				       status->rx_bufshift + 24);

	if (status->crc)
		rx_status->flag |= RX_FLAG_FAILED_FCS_CRC;

<<<<<<< HEAD
	if (status->rx_is40mhzpacket)
		rx_status->bw = RATE_INFO_BW_40;

	if (status->is_ht)
		rx_status->encoding = RX_ENC_HT;
=======
	if (status->rx_is40Mhzpacket)
		rx_status->flag |= RX_FLAG_40MHZ;

	if (status->is_ht)
		rx_status->flag |= RX_FLAG_HT;
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
	 * Notice: this is diff with windows define
	 */
	rx_status->rate_idx = rtlwifi_rate_mapping(hw, status->is_ht,
						   false, status->rate);

	rx_status->mactime = status->timestamp_low;
	if (phystatus) {
		p_drvinfo = (struct rx_fwinfo *)(skb->data +
						 status->rx_bufshift + 24);

<<<<<<< HEAD
		_rtl92ee_translate_rx_signal_stuff(hw, skb, status, pdesc8,
=======
		_rtl92ee_translate_rx_signal_stuff(hw, skb, status, pdesc,
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
						   p_drvinfo);
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

/*in Windows, this == Rx_92EE_Interrupt*/
<<<<<<< HEAD
void rtl92ee_rx_check_dma_ok(struct ieee80211_hw *hw, u8 *header_desc8,
=======
void rtl92ee_rx_check_dma_ok(struct ieee80211_hw *hw, u8 *header_desc,
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
			     u8 queue_index)
{
	u8 first_seg = 0;
	u8 last_seg = 0;
	u16 total_len = 0;
	u16 read_cnt = 0;
<<<<<<< HEAD
	__le32 *header_desc = (__le32 *)header_desc8;
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	if (header_desc == NULL)
		return;

<<<<<<< HEAD
	total_len = (u16)get_rx_buffer_desc_total_length(header_desc);

	first_seg = (u8)get_rx_buffer_desc_fs(header_desc);

	last_seg = (u8)get_rx_buffer_desc_ls(header_desc);

	while (total_len == 0 && first_seg == 0 && last_seg == 0) {
		read_cnt++;
		total_len = (u16)get_rx_buffer_desc_total_length(header_desc);
		first_seg = (u8)get_rx_buffer_desc_fs(header_desc);
		last_seg = (u8)get_rx_buffer_desc_ls(header_desc);
=======
	total_len = (u16)GET_RX_BUFFER_DESC_TOTAL_LENGTH(header_desc);

	first_seg = (u8)GET_RX_BUFFER_DESC_FS(header_desc);

	last_seg = (u8)GET_RX_BUFFER_DESC_LS(header_desc);

	while (total_len == 0 && first_seg == 0 && last_seg == 0) {
		read_cnt++;
		total_len = (u16)GET_RX_BUFFER_DESC_TOTAL_LENGTH(header_desc);
		first_seg = (u8)GET_RX_BUFFER_DESC_FS(header_desc);
		last_seg = (u8)GET_RX_BUFFER_DESC_LS(header_desc);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

		if (read_cnt > 20)
			break;
	}
}

u16 rtl92ee_rx_desc_buff_remained_cnt(struct ieee80211_hw *hw, u8 queue_index)
{
	struct rtl_pci *rtlpci = rtl_pcidev(rtl_pcipriv(hw));
	struct rtl_priv *rtlpriv = rtl_priv(hw);
<<<<<<< HEAD
	u16 read_point, write_point, remind_cnt;
	u32 tmp_4byte;
=======
	u16 read_point = 0, write_point = 0, remind_cnt = 0;
	u32 tmp_4byte = 0;
	static u16 last_read_point;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	static bool start_rx;

	tmp_4byte = rtl_read_dword(rtlpriv, REG_RXQ_TXBD_IDX);
	read_point = (u16)((tmp_4byte>>16) & 0x7ff);
	write_point = (u16)(tmp_4byte & 0x7ff);

	if (write_point != rtlpci->rx_ring[queue_index].next_rx_rp) {
<<<<<<< HEAD
		rtl_dbg(rtlpriv, COMP_RXDESC, DBG_DMESG,
			"!!!write point is 0x%x, reg 0x3B4 value is 0x%x\n",
			write_point, tmp_4byte);
=======
		RT_TRACE(rtlpriv, COMP_RXDESC, DBG_DMESG,
			 "!!!write point is 0x%x, reg 0x3B4 value is 0x%x\n",
			  write_point, tmp_4byte);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		tmp_4byte = rtl_read_dword(rtlpriv, REG_RXQ_TXBD_IDX);
		read_point = (u16)((tmp_4byte>>16) & 0x7ff);
		write_point = (u16)(tmp_4byte & 0x7ff);
	}

	if (read_point > 0)
		start_rx = true;
	if (!start_rx)
		return 0;

<<<<<<< HEAD
	remind_cnt = calc_fifo_space(read_point, write_point,
				     RTL_PCI_MAX_RX_COUNT);
=======
	remind_cnt = calc_fifo_space(read_point, write_point);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	if (remind_cnt == 0)
		return 0;

	rtlpci->rx_ring[queue_index].next_rx_rp = write_point;

<<<<<<< HEAD
=======
	last_read_point = read_point;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	return remind_cnt;
}

static u16 get_desc_addr_fr_q_idx(u16 queue_index)
{
<<<<<<< HEAD
	u16 desc_address;
=======
	u16 desc_address = REG_BEQ_TXBD_IDX;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	switch (queue_index) {
	case BK_QUEUE:
		desc_address = REG_BKQ_TXBD_IDX;
		break;
	case BE_QUEUE:
		desc_address = REG_BEQ_TXBD_IDX;
		break;
	case VI_QUEUE:
		desc_address = REG_VIQ_TXBD_IDX;
		break;
	case VO_QUEUE:
		desc_address = REG_VOQ_TXBD_IDX;
		break;
	case BEACON_QUEUE:
		desc_address = REG_BEQ_TXBD_IDX;
		break;
	case TXCMD_QUEUE:
		desc_address = REG_BEQ_TXBD_IDX;
		break;
	case MGNT_QUEUE:
		desc_address = REG_MGQ_TXBD_IDX;
		break;
	case HIGH_QUEUE:
		desc_address = REG_HI0Q_TXBD_IDX;
		break;
	case HCCA_QUEUE:
		desc_address = REG_BEQ_TXBD_IDX;
		break;
	default:
<<<<<<< HEAD
		desc_address = REG_BEQ_TXBD_IDX;
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		break;
	}
	return desc_address;
}

u16 rtl92ee_get_available_desc(struct ieee80211_hw *hw, u8 q_idx)
{
<<<<<<< HEAD
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	u16 point_diff = 0;
	u16 current_tx_read_point, current_tx_write_point;
=======
	struct rtl_pci *rtlpci = rtl_pcidev(rtl_pcipriv(hw));
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	u16 point_diff = 0;
	u16 current_tx_read_point = 0, current_tx_write_point = 0;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	u32 tmp_4byte;

	tmp_4byte = rtl_read_dword(rtlpriv,
				   get_desc_addr_fr_q_idx(q_idx));
	current_tx_read_point = (u16)((tmp_4byte >> 16) & 0x0fff);
	current_tx_write_point = (u16)((tmp_4byte) & 0x0fff);

	point_diff = calc_fifo_space(current_tx_read_point,
<<<<<<< HEAD
				     current_tx_write_point,
				     TX_DESC_NUM_92E);

=======
				     current_tx_write_point);

	rtlpci->tx_ring[q_idx].avl_desc = point_diff;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	return point_diff;
}

void rtl92ee_pre_fill_tx_bd_desc(struct ieee80211_hw *hw,
<<<<<<< HEAD
				 u8 *tx_bd_desc8, u8 *desc8, u8 queue_index,
=======
				 u8 *tx_bd_desc, u8 *desc, u8 queue_index,
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
				 struct sk_buff *skb, dma_addr_t addr)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_pci *rtlpci = rtl_pcidev(rtl_pcipriv(hw));
	u32 pkt_len = skb->len;
	u16 desc_size = 40; /*tx desc size*/
	u32 psblen = 0;
<<<<<<< HEAD
	u16 tx_page_size;
	u32 total_packet_size;
	u16 current_bd_desc;
	u8 i;
	u16 real_desc_size = 0x28;
	u16	append_early_mode_size = 0;
	u8 segmentnum = 1 << (RTL8192EE_SEG_NUM + 1);
	dma_addr_t desc_dma_addr;
	bool dma64 = rtlpriv->cfg->mod_params->dma64;
	__le32 *desc = (__le32 *)desc8;
	__le32 *tx_bd_desc = (__le32 *)tx_bd_desc8;
=======
	u16 tx_page_size = 0;
	u32 total_packet_size = 0;
	u16 current_bd_desc;
	u8 i = 0;
	u16 real_desc_size = 0x28;
	u16	append_early_mode_size = 0;
#if (RTL8192EE_SEG_NUM == 0)
	u8 segmentnum = 2;
#elif (RTL8192EE_SEG_NUM == 1)
	u8 segmentnum = 4;
#elif (RTL8192EE_SEG_NUM == 2)
	u8 segmentnum = 8;
#endif
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	tx_page_size = 2;
	current_bd_desc = rtlpci->tx_ring[queue_index].cur_tx_wp;

	total_packet_size = desc_size+pkt_len;

	if (rtlpriv->rtlhal.earlymode_enable)	{
		if (queue_index < BEACON_QUEUE) {
			append_early_mode_size = 8;
			total_packet_size += append_early_mode_size;
		}
	}

	if (tx_page_size > 0) {
		psblen = (pkt_len + real_desc_size + append_early_mode_size) /
			 (tx_page_size * 128);

		if (psblen * (tx_page_size * 128) < total_packet_size)
			psblen += 1;
	}

<<<<<<< HEAD
	/* tx desc addr */
	desc_dma_addr = rtlpci->tx_ring[queue_index].dma +
			(current_bd_desc * TX_DESC_SIZE);

	/* Reset */
	set_tx_buff_desc_len_0(tx_bd_desc, 0);
	set_tx_buff_desc_psb(tx_bd_desc, 0);
	set_tx_buff_desc_own(tx_bd_desc, 0);

	for (i = 1; i < segmentnum; i++) {
		set_txbuffer_desc_len_with_offset(tx_bd_desc, i, 0);
		set_txbuffer_desc_amsdu_with_offset(tx_bd_desc, i, 0);
		set_txbuffer_desc_add_low_with_offset(tx_bd_desc, i, 0);
		set_txbuffer_desc_add_high_with_offset(tx_bd_desc, i, 0, dma64);
	}

	/* Clear all status */
	clear_pci_tx_desc_content(desc, TX_DESC_SIZE);
=======
	/* Reset */
	SET_TX_BUFF_DESC_LEN_0(tx_bd_desc, 0);
	SET_TX_BUFF_DESC_PSB(tx_bd_desc, 0);
	SET_TX_BUFF_DESC_OWN(tx_bd_desc, 0);

	for (i = 1; i < segmentnum; i++) {
		SET_TXBUFFER_DESC_LEN_WITH_OFFSET(tx_bd_desc, i, 0);
		SET_TXBUFFER_DESC_AMSDU_WITH_OFFSET(tx_bd_desc, i, 0);
		SET_TXBUFFER_DESC_ADD_LOW_WITH_OFFSET(tx_bd_desc, i, 0);
#if (DMA_IS_64BIT == 1)
		SET_TXBUFFER_DESC_ADD_HIGT_WITH_OFFSET(tx_bd_desc, i, 0);
#endif
	}
	SET_TX_BUFF_DESC_LEN_1(tx_bd_desc, 0);
	SET_TX_BUFF_DESC_AMSDU_1(tx_bd_desc, 0);

	SET_TX_BUFF_DESC_LEN_2(tx_bd_desc, 0);
	SET_TX_BUFF_DESC_AMSDU_2(tx_bd_desc, 0);
	SET_TX_BUFF_DESC_LEN_3(tx_bd_desc, 0);
	SET_TX_BUFF_DESC_AMSDU_3(tx_bd_desc, 0);
	/* Clear all status */
	CLEAR_PCI_TX_DESC_CONTENT(desc, TX_DESC_SIZE);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	if (rtlpriv->rtlhal.earlymode_enable) {
		if (queue_index < BEACON_QUEUE) {
			/* This if needs braces */
<<<<<<< HEAD
			set_tx_buff_desc_len_0(tx_bd_desc, desc_size + 8);
		} else {
			set_tx_buff_desc_len_0(tx_bd_desc, desc_size);
		}
	} else {
		set_tx_buff_desc_len_0(tx_bd_desc, desc_size);
	}
	set_tx_buff_desc_psb(tx_bd_desc, psblen);
	set_tx_buff_desc_addr_low_0(tx_bd_desc, desc_dma_addr);
	set_tx_buff_desc_addr_high_0(tx_bd_desc, ((u64)desc_dma_addr >> 32),
				     dma64);

	set_txbuffer_desc_len_with_offset(tx_bd_desc, 1, pkt_len);
	/* don't using extendsion mode. */
	set_txbuffer_desc_amsdu_with_offset(tx_bd_desc, 1, 0);
	set_txbuffer_desc_add_low_with_offset(tx_bd_desc, 1, addr);
	set_txbuffer_desc_add_high_with_offset(tx_bd_desc, 1,
					       ((u64)addr >> 32), dma64);

	set_tx_desc_pkt_size(desc, (u16)(pkt_len));
	set_tx_desc_tx_buffer_size(desc, (u16)(pkt_len));
}

void rtl92ee_tx_fill_desc(struct ieee80211_hw *hw,
			  struct ieee80211_hdr *hdr, u8 *pdesc8,
=======
			SET_TX_BUFF_DESC_LEN_0(tx_bd_desc, desc_size + 8);
		} else {
			SET_TX_BUFF_DESC_LEN_0(tx_bd_desc, desc_size);
		}
	} else {
		SET_TX_BUFF_DESC_LEN_0(tx_bd_desc, desc_size);
	}
	SET_TX_BUFF_DESC_PSB(tx_bd_desc, psblen);
	SET_TX_BUFF_DESC_ADDR_LOW_0(tx_bd_desc,
				    rtlpci->tx_ring[queue_index].dma +
				    (current_bd_desc * TX_DESC_SIZE));

	SET_TXBUFFER_DESC_LEN_WITH_OFFSET(tx_bd_desc, 1, pkt_len);
	/* don't using extendsion mode. */
	SET_TXBUFFER_DESC_AMSDU_WITH_OFFSET(tx_bd_desc, 1, 0);
	SET_TXBUFFER_DESC_ADD_LOW_WITH_OFFSET(tx_bd_desc, 1, addr);

	SET_TX_DESC_PKT_SIZE(desc, (u16)(pkt_len));
	SET_TX_DESC_TX_BUFFER_SIZE(desc, (u16)(pkt_len));
}

void rtl92ee_tx_fill_desc(struct ieee80211_hw *hw,
			  struct ieee80211_hdr *hdr, u8 *pdesc_tx,
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
			  u8 *pbd_desc_tx,
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
	u16 seq_number;
	__le16 fc = hdr->frame_control;
=======
	u8 *pdesc = (u8 *)pdesc_tx;
	u16 seq_number;
	__le16 fc = hdr->frame_control;
	unsigned int buf_len = 0;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	u8 fw_qsel = _rtl92ee_map_hwqueue_to_fwqueue(skb, hw_queue);
	bool firstseg = ((hdr->seq_ctrl &
			    cpu_to_le16(IEEE80211_SCTL_FRAG)) == 0);
	bool lastseg = ((hdr->frame_control &
			   cpu_to_le16(IEEE80211_FCTL_MOREFRAGS)) == 0);
	dma_addr_t mapping;
	u8 bw_40 = 0;
<<<<<<< HEAD
	__le32 *pdesc = (__le32 *)pdesc8;
=======
	u8 short_gi = 0;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	if (mac->opmode == NL80211_IFTYPE_STATION) {
		bw_40 = mac->bw_40;
	} else if (mac->opmode == NL80211_IFTYPE_AP ||
		   mac->opmode == NL80211_IFTYPE_ADHOC) {
		if (sta)
<<<<<<< HEAD
			bw_40 = sta->deflink.ht_cap.cap &
=======
			bw_40 = sta->ht_cap.cap &
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
				IEEE80211_HT_CAP_SUP_WIDTH_20_40;
	}
	seq_number = (le16_to_cpu(hdr->seq_ctrl) & IEEE80211_SCTL_SEQ) >> 4;
	rtl_get_tcb_desc(hw, info, sta, skb, ptcb_desc);
	/* reserve 8 byte for AMPDU early mode */
	if (rtlhal->earlymode_enable) {
		skb_push(skb, EM_HDR_LEN);
		memset(skb->data, 0, EM_HDR_LEN);
	}
<<<<<<< HEAD
	mapping = dma_map_single(&rtlpci->pdev->dev, skb->data, skb->len,
				 DMA_TO_DEVICE);
	if (dma_mapping_error(&rtlpci->pdev->dev, mapping)) {
		rtl_dbg(rtlpriv, COMP_SEND, DBG_TRACE,
			"DMA mapping error\n");
=======
	buf_len = skb->len;
	mapping = pci_map_single(rtlpci->pdev, skb->data, skb->len,
				 PCI_DMA_TODEVICE);
	if (pci_dma_mapping_error(rtlpci->pdev, mapping)) {
		RT_TRACE(rtlpriv, COMP_SEND, DBG_TRACE,
			 "DMA mapping error\n");
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		return;
	}

	if (pbd_desc_tx != NULL)
<<<<<<< HEAD
		rtl92ee_pre_fill_tx_bd_desc(hw, pbd_desc_tx, pdesc8, hw_queue,
=======
		rtl92ee_pre_fill_tx_bd_desc(hw, pbd_desc_tx, pdesc, hw_queue,
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
					    skb, mapping);

	if (ieee80211_is_nullfunc(fc) || ieee80211_is_ctl(fc)) {
		firstseg = true;
		lastseg = true;
	}
	if (firstseg) {
		if (rtlhal->earlymode_enable) {
<<<<<<< HEAD
			set_tx_desc_pkt_offset(pdesc, 1);
			set_tx_desc_offset(pdesc,
					   USB_HWDESC_HEADER_LEN + EM_HDR_LEN);
			if (ptcb_desc->empkt_num) {
				rtl_dbg(rtlpriv, COMP_SEND, DBG_TRACE,
					"Insert 8 byte.pTcb->EMPktNum:%d\n",
					ptcb_desc->empkt_num);
=======
			SET_TX_DESC_PKT_OFFSET(pdesc, 1);
			SET_TX_DESC_OFFSET(pdesc,
					   USB_HWDESC_HEADER_LEN + EM_HDR_LEN);
			if (ptcb_desc->empkt_num) {
				RT_TRACE(rtlpriv, COMP_SEND, DBG_TRACE,
					 "Insert 8 byte.pTcb->EMPktNum:%d\n",
					  ptcb_desc->empkt_num);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
				_rtl92ee_insert_emcontent(ptcb_desc,
							  (u8 *)(skb->data));
			}
		} else {
<<<<<<< HEAD
			set_tx_desc_offset(pdesc, USB_HWDESC_HEADER_LEN);
		}


		set_tx_desc_tx_rate(pdesc, ptcb_desc->hw_rate);
=======
			SET_TX_DESC_OFFSET(pdesc, USB_HWDESC_HEADER_LEN);
		}

		SET_TX_DESC_TX_RATE(pdesc, ptcb_desc->hw_rate);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

		if (ieee80211_is_mgmt(fc)) {
			ptcb_desc->use_driver_rate = true;
		} else {
			if (rtlpriv->ra.is_special_data) {
				ptcb_desc->use_driver_rate = true;
<<<<<<< HEAD
				set_tx_desc_tx_rate(pdesc, DESC_RATE11M);
=======
				SET_TX_DESC_TX_RATE(pdesc, DESC_RATE11M);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
			} else {
				ptcb_desc->use_driver_rate = false;
			}
		}

<<<<<<< HEAD
		if (info->flags & IEEE80211_TX_CTL_AMPDU) {
			set_tx_desc_agg_enable(pdesc, 1);
			set_tx_desc_max_agg_num(pdesc, 0x14);
		}
		set_tx_desc_seq(pdesc, seq_number);
		set_tx_desc_rts_enable(pdesc,
				       ((ptcb_desc->rts_enable &&
					 !ptcb_desc->cts_enable) ? 1 : 0));
		set_tx_desc_hw_rts_enable(pdesc, 0);
		set_tx_desc_cts2self(pdesc,
				     ((ptcb_desc->cts_enable) ? 1 : 0));

		set_tx_desc_rts_rate(pdesc, ptcb_desc->rts_rate);
		set_tx_desc_rts_sc(pdesc, ptcb_desc->rts_sc);
		set_tx_desc_rts_short(pdesc,
=======
		if (ptcb_desc->hw_rate > DESC_RATEMCS0)
			short_gi = (ptcb_desc->use_shortgi) ? 1 : 0;
		else
			short_gi = (ptcb_desc->use_shortpreamble) ? 1 : 0;

		if (info->flags & IEEE80211_TX_CTL_AMPDU) {
			SET_TX_DESC_AGG_ENABLE(pdesc, 1);
			SET_TX_DESC_MAX_AGG_NUM(pdesc, 0x14);
		}
		SET_TX_DESC_SEQ(pdesc, seq_number);
		SET_TX_DESC_RTS_ENABLE(pdesc,
				       ((ptcb_desc->rts_enable &&
					 !ptcb_desc->cts_enable) ? 1 : 0));
		SET_TX_DESC_HW_RTS_ENABLE(pdesc, 0);
		SET_TX_DESC_CTS2SELF(pdesc,
				     ((ptcb_desc->cts_enable) ? 1 : 0));

		SET_TX_DESC_RTS_RATE(pdesc, ptcb_desc->rts_rate);
		SET_TX_DESC_RTS_SC(pdesc, ptcb_desc->rts_sc);
		SET_TX_DESC_RTS_SHORT(pdesc,
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
				((ptcb_desc->rts_rate <= DESC_RATE54M) ?
				 (ptcb_desc->rts_use_shortpreamble ? 1 : 0) :
				 (ptcb_desc->rts_use_shortgi ? 1 : 0)));

		if (ptcb_desc->tx_enable_sw_calc_duration)
<<<<<<< HEAD
			set_tx_desc_nav_use_hdr(pdesc, 1);

		if (bw_40) {
			if (ptcb_desc->packet_bw == HT_CHANNEL_WIDTH_20_40) {
				set_tx_desc_data_bw(pdesc, 1);
				set_tx_desc_tx_sub_carrier(pdesc, 3);
			} else {
				set_tx_desc_data_bw(pdesc, 0);
				set_tx_desc_tx_sub_carrier(pdesc,
							   mac->cur_40_prime_sc);
			}
		} else {
			set_tx_desc_data_bw(pdesc, 0);
			set_tx_desc_tx_sub_carrier(pdesc, 0);
		}

		set_tx_desc_linip(pdesc, 0);
		if (sta) {
			u8 ampdu_density = sta->deflink.ht_cap.ampdu_density;

			set_tx_desc_ampdu_density(pdesc, ampdu_density);
=======
			SET_TX_DESC_NAV_USE_HDR(pdesc, 1);

		if (bw_40) {
			if (ptcb_desc->packet_bw == HT_CHANNEL_WIDTH_20_40) {
				SET_TX_DESC_DATA_BW(pdesc, 1);
				SET_TX_DESC_TX_SUB_CARRIER(pdesc, 3);
			} else {
				SET_TX_DESC_DATA_BW(pdesc, 0);
				SET_TX_DESC_TX_SUB_CARRIER(pdesc,
							   mac->cur_40_prime_sc);
			}
		} else {
			SET_TX_DESC_DATA_BW(pdesc, 0);
			SET_TX_DESC_TX_SUB_CARRIER(pdesc, 0);
		}

		SET_TX_DESC_LINIP(pdesc, 0);
		if (sta) {
			u8 ampdu_density = sta->ht_cap.ampdu_density;

			SET_TX_DESC_AMPDU_DENSITY(pdesc, ampdu_density);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		}
		if (info->control.hw_key) {
			struct ieee80211_key_conf *key = info->control.hw_key;

			switch (key->cipher) {
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
		set_tx_desc_disable_fb(pdesc,
				       ptcb_desc->disable_ratefallback ? 1 : 0);
		set_tx_desc_use_rate(pdesc, ptcb_desc->use_driver_rate ? 1 : 0);

		/*set_tx_desc_pwr_status(pdesc, pwr_status);*/
=======
		SET_TX_DESC_QUEUE_SEL(pdesc, fw_qsel);
		SET_TX_DESC_DATA_RATE_FB_LIMIT(pdesc, 0x1F);
		SET_TX_DESC_RTS_RATE_FB_LIMIT(pdesc, 0xF);
		SET_TX_DESC_DISABLE_FB(pdesc,
				       ptcb_desc->disable_ratefallback ? 1 : 0);
		SET_TX_DESC_USE_RATE(pdesc, ptcb_desc->use_driver_rate ? 1 : 0);

		/*SET_TX_DESC_PWR_STATUS(pdesc, pwr_status);*/
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		/* Set TxRate and RTSRate in TxDesc  */
		/* This prevent Tx initial rate of new-coming packets */
		/* from being overwritten by retried  packet rate.*/
		if (!ptcb_desc->use_driver_rate) {
<<<<<<< HEAD
			/*set_tx_desc_rts_rate(pdesc, 0x08); */
			/* set_tx_desc_tx_rate(pdesc, 0x0b); */
		}
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
	set_tx_desc_tx_buffer_address(pdesc, mapping);
	if (rtlpriv->dm.useramask) {
		set_tx_desc_rate_id(pdesc, ptcb_desc->ratr_index);
		set_tx_desc_macid(pdesc, ptcb_desc->mac_id);
	} else {
		set_tx_desc_rate_id(pdesc, 0xC + ptcb_desc->ratr_index);
		set_tx_desc_macid(pdesc, ptcb_desc->ratr_index);
	}

	set_tx_desc_more_frag(pdesc, (lastseg ? 0 : 1));
	if (is_multicast_ether_addr(ieee80211_get_DA(hdr)) ||
	    is_broadcast_ether_addr(ieee80211_get_DA(hdr))) {
		set_tx_desc_bmc(pdesc, 1);
	}
	rtl_dbg(rtlpriv, COMP_SEND, DBG_TRACE, "\n");
}

void rtl92ee_tx_fill_cmddesc(struct ieee80211_hw *hw,
			     u8 *pdesc8, bool firstseg,
=======
			/*SET_TX_DESC_RTS_RATE(pdesc, 0x08); */
			/* SET_TX_DESC_TX_RATE(pdesc, 0x0b); */
		}
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
	SET_TX_DESC_TX_BUFFER_ADDRESS(pdesc, mapping);
	if (rtlpriv->dm.useramask) {
		SET_TX_DESC_RATE_ID(pdesc, ptcb_desc->ratr_index);
		SET_TX_DESC_MACID(pdesc, ptcb_desc->mac_id);
	} else {
		SET_TX_DESC_RATE_ID(pdesc, 0xC + ptcb_desc->ratr_index);
		SET_TX_DESC_MACID(pdesc, ptcb_desc->ratr_index);
	}

	SET_TX_DESC_MORE_FRAG(pdesc, (lastseg ? 0 : 1));
	if (is_multicast_ether_addr(ieee80211_get_DA(hdr)) ||
	    is_broadcast_ether_addr(ieee80211_get_DA(hdr))) {
		SET_TX_DESC_BMC(pdesc, 1);
	}
	RT_TRACE(rtlpriv, COMP_SEND, DBG_TRACE, "\n");
}

void rtl92ee_tx_fill_cmddesc(struct ieee80211_hw *hw,
			     u8 *pdesc, bool firstseg,
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
			     bool lastseg, struct sk_buff *skb)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_pci *rtlpci = rtl_pcidev(rtl_pcipriv(hw));
	u8 fw_queue = QSLT_BEACON;
<<<<<<< HEAD
	dma_addr_t mapping = dma_map_single(&rtlpci->pdev->dev, skb->data,
					    skb->len, DMA_TO_DEVICE);
	u8 txdesc_len = 40;
	__le32 *pdesc = (__le32 *)pdesc8;

	if (dma_mapping_error(&rtlpci->pdev->dev, mapping)) {
		rtl_dbg(rtlpriv, COMP_SEND, DBG_TRACE,
			"DMA mapping error\n");
		return;
	}
	clear_pci_tx_desc_content(pdesc, txdesc_len);

	if (firstseg)
		set_tx_desc_offset(pdesc, txdesc_len);

	set_tx_desc_tx_rate(pdesc, DESC_RATE1M);

	set_tx_desc_seq(pdesc, 0);

	set_tx_desc_linip(pdesc, 0);

	set_tx_desc_queue_sel(pdesc, fw_queue);

	set_tx_desc_first_seg(pdesc, 1);
	set_tx_desc_last_seg(pdesc, 1);

	set_tx_desc_tx_buffer_size(pdesc, (u16)(skb->len));

	set_tx_desc_tx_buffer_address(pdesc, mapping);

	set_tx_desc_rate_id(pdesc, 7);
	set_tx_desc_macid(pdesc, 0);

	set_tx_desc_own(pdesc, 1);

	set_tx_desc_pkt_size(pdesc, (u16)(skb->len));

	set_tx_desc_first_seg(pdesc, 1);
	set_tx_desc_last_seg(pdesc, 1);

	set_tx_desc_offset(pdesc, 40);

	set_tx_desc_use_rate(pdesc, 1);
=======
	dma_addr_t mapping = pci_map_single(rtlpci->pdev,
					    skb->data, skb->len,
					    PCI_DMA_TODEVICE);
	u8 txdesc_len = 40;

	if (pci_dma_mapping_error(rtlpci->pdev, mapping)) {
		RT_TRACE(rtlpriv, COMP_SEND, DBG_TRACE,
			 "DMA mapping error\n");
		return;
	}
	CLEAR_PCI_TX_DESC_CONTENT(pdesc, txdesc_len);

	if (firstseg)
		SET_TX_DESC_OFFSET(pdesc, txdesc_len);

	SET_TX_DESC_TX_RATE(pdesc, DESC_RATE1M);

	SET_TX_DESC_SEQ(pdesc, 0);

	SET_TX_DESC_LINIP(pdesc, 0);

	SET_TX_DESC_QUEUE_SEL(pdesc, fw_queue);

	SET_TX_DESC_FIRST_SEG(pdesc, 1);
	SET_TX_DESC_LAST_SEG(pdesc, 1);

	SET_TX_DESC_TX_BUFFER_SIZE(pdesc, (u16)(skb->len));

	SET_TX_DESC_TX_BUFFER_ADDRESS(pdesc, mapping);

	SET_TX_DESC_RATE_ID(pdesc, 7);
	SET_TX_DESC_MACID(pdesc, 0);

	SET_TX_DESC_OWN(pdesc, 1);

	SET_TX_DESC_PKT_SIZE((u8 *)pdesc, (u16)(skb->len));

	SET_TX_DESC_FIRST_SEG(pdesc, 1);
	SET_TX_DESC_LAST_SEG(pdesc, 1);

	SET_TX_DESC_OFFSET(pdesc, 40);

	SET_TX_DESC_USE_RATE(pdesc, 1);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	RT_PRINT_DATA(rtlpriv, COMP_CMD, DBG_LOUD,
		      "H2C Tx Cmd Content\n", pdesc, txdesc_len);
}

<<<<<<< HEAD
void rtl92ee_set_desc(struct ieee80211_hw *hw, u8 *pdesc8, bool istx,
		      u8 desc_name, u8 *val)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	u8 q_idx = *val;
	bool dma64 = rtlpriv->cfg->mod_params->dma64;
	__le32 *pdesc = (__le32 *)pdesc8;
=======
void rtl92ee_set_desc(struct ieee80211_hw *hw, u8 *pdesc, bool istx,
		      u8 desc_name, u8 *val)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	u16 cur_tx_rp = 0;
	u16 cur_tx_wp = 0;
	static u16 last_txw_point;
	static bool over_run;
	u32 tmp = 0;
	u8 q_idx = *val;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	if (istx) {
		switch (desc_name) {
		case HW_DESC_TX_NEXTDESC_ADDR:
<<<<<<< HEAD
			set_tx_desc_next_desc_address(pdesc, *(u32 *)val);
=======
			SET_TX_DESC_NEXT_DESC_ADDRESS(pdesc, *(u32 *)val);
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
			break;
		case HW_DESC_OWN:{
			struct rtl_pci *rtlpci = rtl_pcidev(rtl_pcipriv(hw));
			struct rtl8192_tx_ring *ring = &rtlpci->tx_ring[q_idx];
			u16 max_tx_desc = ring->entries;

			if (q_idx == BEACON_QUEUE) {
				ring->cur_tx_wp = 0;
				ring->cur_tx_rp = 0;
<<<<<<< HEAD
				set_tx_buff_desc_own(pdesc, 1);
				return;
			}

			/* make sure tx desc is available by caller */
			ring->cur_tx_wp = ((ring->cur_tx_wp + 1) % max_tx_desc);

			rtl_write_word(rtlpriv,
				       get_desc_addr_fr_q_idx(q_idx),
				       ring->cur_tx_wp);
=======
				SET_TX_BUFF_DESC_OWN(pdesc, 1);
				return;
			}

			ring->cur_tx_wp = ((ring->cur_tx_wp + 1) % max_tx_desc);

			if (over_run) {
				ring->cur_tx_wp = 0;
				over_run = false;
			}
			if (ring->avl_desc > 1) {
				ring->avl_desc--;

				rtl_write_word(rtlpriv,
					       get_desc_addr_fr_q_idx(q_idx),
					       ring->cur_tx_wp);

				if (q_idx == 1)
					last_txw_point = cur_tx_wp;
			}

			if (ring->avl_desc < (max_tx_desc - 15)) {
				u16 point_diff = 0;

				tmp =
				  rtl_read_dword(rtlpriv,
						 get_desc_addr_fr_q_idx(q_idx));
				cur_tx_rp = (u16)((tmp >> 16) & 0x0fff);
				cur_tx_wp = (u16)(tmp & 0x0fff);

				ring->cur_tx_wp = cur_tx_wp;
				ring->cur_tx_rp = cur_tx_rp;
				point_diff = ((cur_tx_rp > cur_tx_wp) ?
					      (cur_tx_rp - cur_tx_wp) :
					      (TX_DESC_NUM_92E - 1 -
					       cur_tx_wp + cur_tx_rp));

				ring->avl_desc = point_diff;
			}
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
		}
		break;
		}
	} else {
		switch (desc_name) {
		case HW_DESC_RX_PREPARE:
<<<<<<< HEAD
			set_rx_buffer_desc_ls(pdesc, 0);
			set_rx_buffer_desc_fs(pdesc, 0);
			set_rx_buffer_desc_total_length(pdesc, 0);

			set_rx_buffer_desc_data_length(pdesc,
						       MAX_RECEIVE_BUFFER_SIZE +
						       RX_DESC_SIZE);

			set_rx_buffer_physical_low(pdesc, (*(dma_addr_t *)val) &
						   DMA_BIT_MASK(32));
			set_rx_buffer_physical_high(pdesc,
						    ((u64)(*(dma_addr_t *)val)
						    >> 32),
						    dma64);
			break;
		case HW_DESC_RXERO:
			set_rx_desc_eor(pdesc, 1);
			break;
		default:
			WARN_ONCE(true,
				  "rtl8192ee: ERR rxdesc :%d not processed\n",
				  desc_name);
=======
			SET_RX_BUFFER_DESC_LS(pdesc, 0);
			SET_RX_BUFFER_DESC_FS(pdesc, 0);
			SET_RX_BUFFER_DESC_TOTAL_LENGTH(pdesc, 0);

			SET_RX_BUFFER_DESC_DATA_LENGTH(pdesc,
						       MAX_RECEIVE_BUFFER_SIZE +
						       RX_DESC_SIZE);

			SET_RX_BUFFER_PHYSICAL_LOW(pdesc, *(u32 *)val);
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
u64 rtl92ee_get_desc(struct ieee80211_hw *hw,
		     u8 *pdesc8, bool istx, u8 desc_name)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	u64 ret = 0;
	bool dma64 = rtlpriv->cfg->mod_params->dma64;
	__le32 *pdesc = (__le32 *)pdesc8;
=======
u32 rtl92ee_get_desc(u8 *pdesc, bool istx, u8 desc_name)
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
			ret = get_txbuffer_desc_addr_low(pdesc, 1);
			ret |= (u64)get_txbuffer_desc_addr_high(pdesc, 1,
								dma64) << 32;
			break;
		default:
			WARN_ONCE(true,
				  "rtl8192ee: ERR txdesc :%d not processed\n",
				  desc_name);
=======
			ret = GET_TX_DESC_OWN(pdesc);
			break;
		case HW_DESC_TXBUFF_ADDR:
			ret = GET_TXBUFFER_DESC_ADDR_LOW(pdesc, 1);
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
				  "rtl8192ee: ERR rxdesc :%d not processed\n",
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

bool rtl92ee_is_tx_desc_closed(struct ieee80211_hw *hw, u8 hw_queue, u16 index)
{
	struct rtl_pci *rtlpci = rtl_pcidev(rtl_pcipriv(hw));
	struct rtl_priv *rtlpriv = rtl_priv(hw);
<<<<<<< HEAD
	u16 read_point, write_point;
	bool ret = false;
	struct rtl8192_tx_ring *ring = &rtlpci->tx_ring[hw_queue];

	{
		u16 cur_tx_rp;
		u32 tmpu32;
=======
	u16 read_point, write_point, available_desc_num;
	bool ret = false;
	static u8 stop_report_cnt;
	struct rtl8192_tx_ring *ring = &rtlpci->tx_ring[hw_queue];

	{
		u16 point_diff = 0;
		u16 cur_tx_rp, cur_tx_wp;
		u32 tmpu32 = 0;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

		tmpu32 =
		  rtl_read_dword(rtlpriv,
				 get_desc_addr_fr_q_idx(hw_queue));
		cur_tx_rp = (u16)((tmpu32 >> 16) & 0x0fff);
<<<<<<< HEAD

		/* don't need to update ring->cur_tx_wp */
		ring->cur_tx_rp = cur_tx_rp;
=======
		cur_tx_wp = (u16)(tmpu32 & 0x0fff);

		ring->cur_tx_wp = cur_tx_wp;
		ring->cur_tx_rp = cur_tx_rp;
		point_diff = ((cur_tx_rp > cur_tx_wp) ?
			      (cur_tx_rp - cur_tx_wp) :
			      (TX_DESC_NUM_92E - cur_tx_wp + cur_tx_rp));

		ring->avl_desc = point_diff;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	}

	read_point = ring->cur_tx_rp;
	write_point = ring->cur_tx_wp;
<<<<<<< HEAD
=======
	available_desc_num = ring->avl_desc;
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

	if (write_point > read_point) {
		if (index < write_point && index >= read_point)
			ret = false;
		else
			ret = true;
	} else if (write_point < read_point) {
		if (index > write_point && index < read_point)
			ret = true;
		else
			ret = false;
	} else {
		if (index != read_point)
			ret = true;
	}

	if (hw_queue == BEACON_QUEUE)
		ret = true;

	if (rtlpriv->rtlhal.driver_is_goingto_unload ||
	    rtlpriv->psc.rfoff_reason > RF_CHANGE_BY_PS)
		ret = true;

<<<<<<< HEAD
=======
	if (hw_queue < BEACON_QUEUE) {
		if (!ret)
			stop_report_cnt++;
		else
			stop_report_cnt = 0;
	}

>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
	return ret;
}

void rtl92ee_tx_polling(struct ieee80211_hw *hw, u8 hw_queue)
{
}
<<<<<<< HEAD
=======

u32 rtl92ee_rx_command_packet(struct ieee80211_hw *hw,
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
		rtl92ee_c2h_packet_handler(hw, skb->data, (u8)skb->len);
		result = 1;
		break;
	default:
		RT_TRACE(rtlpriv, COMP_RECV, DBG_TRACE,
			 "Unknown packet type %d\n", status->packet_report_type);
		break;
	}

	return result;
}
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
