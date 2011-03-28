KDIR := ~/linux-source-2.6.32

all:
	$(MAKE) -C $(KDIR) M=`pwd` modules

clean:
	rm *.o log *mod.c *.ko modules.order Module.symvers -f


