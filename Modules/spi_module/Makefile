TARGET = spi_module
OBJS = spi_module.o
MDIR = drivers/misc

ifneq ($(KERNELRELEASE),)
# call from kernel build system
obj-m :=$(OBJS)

else
CURRENT = imx_4.1.15_1.2.0_ga
EXTRA_CFLAGS = -DEXPORT_SYMTAB -march=arm -I/Devel/NOVAsom_SDK/Xcompiler/ArmCompiler/usr/include
KERNELDIR ?= /Devel/NOVAsom_SDK/Kernel/linux-$(CURRENT)
PWD       := $(shell pwd)
DEST = /lib/modules/$(CURRENT)/kernel/$(MDIR)

Debug:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) LDDINC=$(PWD) modules

endif

cleanDebug:
cleanRelease:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions
