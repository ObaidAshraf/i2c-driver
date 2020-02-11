obj-m = i2c.o
EXTRA_CFLAGS = -fno-pie
MAKEFLAGS = -Wno-error

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
