<<<<<<< HEAD
/* SPDX-License-Identifier: GPL-2.0 */
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
#ifndef RTL8180_RTL8225_H
#define RTL8180_RTL8225_H

#define RTL8225_ANAPARAM_ON	0xa0000b59
#define RTL8225_ANAPARAM2_ON	0x860dec11
#define RTL8225_ANAPARAM_OFF	0xa00beb59
#define RTL8225_ANAPARAM2_OFF	0x840dec11

const struct rtl818x_rf_ops * rtl8180_detect_rf(struct ieee80211_hw *);

static inline void rtl8225_write_phy_ofdm(struct ieee80211_hw *dev,
					  u8 addr, u8 data)
{
	rtl8180_write_phy(dev, addr, data);
}

static inline void rtl8225_write_phy_cck(struct ieee80211_hw *dev,
					 u8 addr, u8 data)
{
	rtl8180_write_phy(dev, addr, data | 0x10000);
}

#endif /* RTL8180_RTL8225_H */
