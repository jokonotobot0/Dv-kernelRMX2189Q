#
# Makefile for the kernel mmc device drivers.
#

<<<<<<< HEAD
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include
=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
subdir-ccflags-$(CONFIG_MMC_DEBUG) := -DDEBUG

obj-$(CONFIG_MMC)		+= core/
obj-$(CONFIG_MMC)		+= card/
obj-$(subst m,y,$(CONFIG_MMC))	+= host/
