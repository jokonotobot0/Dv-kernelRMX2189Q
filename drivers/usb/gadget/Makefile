#
# USB peripheral controller drivers
#
subdir-ccflags-$(CONFIG_USB_GADGET_DEBUG)	:= -DDEBUG
subdir-ccflags-$(CONFIG_USB_GADGET_VERBOSE)	+= -DVERBOSE_DEBUG
<<<<<<< HEAD
MTK_PLATFORM := $(subst ",,$(CONFIG_MTK_PLATFORM))
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/usb_boost
ifdef CONFIG_MTPROF
ccflags-y += -Idrivers/misc/mediatek/mtprof/
endif

ccflags-y	+= -I$(srctree)/drivers/usb/gadget/udc -I$(srctree)/drivers/usb/gadget/function
ccflags-y += -I$(srctree)/drivers/misc/mediatek/c2k_ccci/
ccflags-y += -I$(srctree)/drivers/usb/gadget/
=======
ccflags-y				+= -I$(srctree)/drivers/usb/gadget/udc
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc

obj-$(CONFIG_USB_LIBCOMPOSITE)	+= libcomposite.o
libcomposite-y			:= usbstring.o config.o epautoconf.o
libcomposite-y			+= composite.o functions.o configfs.o u_f.o
<<<<<<< HEAD
libcomposite-$(CONFIG_USB_CONFIGFS_UEVENT)			+= meta.o

g_android-y			:= android.o

obj-$(CONFIG_USB_GADGET)	+= udc/ function/ legacy/

obj-$(CONFIG_USB_G_ANDROID)	+= g_android.o
=======

obj-$(CONFIG_USB_GADGET)	+= udc/ function/ legacy/
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
