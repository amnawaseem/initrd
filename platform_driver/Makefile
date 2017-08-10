export ARCH := arm64
export CROSS_COMPILE := aarch64-linux-gnu-
obj-m := testdriver.o
KDIR := /home/amna/Thesis/Linux_4.11/linux-4.11-rc2
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
