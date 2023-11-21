#
# Makefile for USB Core files and filesystem
#

<<<<<<< HEAD
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/usb_boost

=======
>>>>>>> 59e6b98dfb018c1d2f6293d84f5d1b82386049bc
usbcore-y := usb.o hub.o hcd.o urb.o message.o driver.o
usbcore-y += config.o file.o buffer.o sysfs.o endpoint.o
usbcore-y += devio.o notify.o generic.o quirks.o devices.o
usbcore-y += port.o

usbcore-$(CONFIG_OF)		+= of.o
usbcore-$(CONFIG_PCI)		+= hcd-pci.o
usbcore-$(CONFIG_ACPI)		+= usb-acpi.o

obj-$(CONFIG_USB)		+= usbcore.o

obj-$(CONFIG_USB_LEDS_TRIGGER_USBPORT)	+= ledtrig-usbport.o
