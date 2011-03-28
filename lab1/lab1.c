#include <linux/vmalloc.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/crc32.h>
#define MODULE_NAME hallo

static void *iter(void *v) {
	// ??? how to include $(KERNEL_SRC)/kernel/resource.h ???
	// to use r_next function?
	struct resource *p = v;
	if (p->child != NULL)
		return p->child;
	while ((p->sibling == NULL) && (p->parent != NULL)) 
		p = p-> parent;
	return p->sibling;
}
extern u32 crc32_le(u32,unsigned char const*, size_t);

static u32 find(void* v, char *name) {
	struct resource *p = v;
	while (p != NULL) {
		printk("name = %s\n", p->name);
		if (strcmp(name,p->name) == 0) {
			printk("PROFIT!\nstart=%d\nend=%d\n",p->start,p->end);				// CRC32 crash system.	
			return crc32(0,p->start, resource_size(p) );
		}
		p = iter(p);
	}
	return 0;
}

char *KERNEL_CODE = "Kernel code";

static int __init MODULE_NAME_init(void) {
	printk(KERN_INFO "hello\n");
	printk("%p\n", &iomem_resource);
 	u32 ans = find(&iomem_resource, "Kernel code");
	if (ans == 0) { 
		printk("%s not found", KERNEL_CODE);
	} else {
		printk("CRC32 of %s = %d",KERNEL_CODE,ans);
	}
	return 0;
}

static void __exit MODULE_NAME_exit(void) {
	printk(KERN_INFO "bye");
}

module_init(MODULE_NAME_init);
module_exit(MODULE_NAME_exit);

