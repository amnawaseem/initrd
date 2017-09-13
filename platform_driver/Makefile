export ARCH := arm64

MY_CFLAGS += -g -DDEBUG -static -static-libgcc -static-libstdc++
MY_LDFLAGS += -g -static
ccflags-y += ${MY_CFLAGS}
CC += ${MY_CFLAGS}
LDFLAGS += ${MY_LDFLAGS} 

export CROSS_COMPILE := aarch64-linux-gnu-
obj-m := testdriver.o
KDIR := /home/amna/Thesis/Linux_4.11/linux-4.11-rc2
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KDIR)  -I/usr/aarch64-linux-gnu/include/sys/ SUBDIRS=$(PWD) modules EXTRA_CFLAGS=" -g $(MY_CFLAGS) $(MY_LDFLAGS)"
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
