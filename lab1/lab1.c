#include <linux/vmalloc.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/crc32.h>
#include <asm-generic/io.h>
#define MODULE_NAME hallo

static void *iter(void *v) {
	/* ?? maybe to include $(KERNEL_SRC)/kernel/resource.h ???
	 * to use r_next function?
	 */
	struct resource *p = v;
	if (p->child != NULL)
		return p->child;
	while ((p->sibling == NULL) && (p->parent != NULL)) 
		p = p-> parent;
	return p->sibling;
}

static u32 find(void *v, char *name) {
	struct resource *p = v;
	while (p != NULL) {
		if (strcmp(name,p->name) == 0) {
			return crc32(0, phys_to_virt(p->start), 
					resource_size(p) );
		}
		p = iter(p);
	}
	return 0;
}

char *KERNEL_CODE = "Kernel code";

static int __init MODULE_NAME_init(void) {
 	u32 ans = find(&iomem_resource, "Kernel code");
	if (ans == 0) { 
		printk("%s not found", KERNEL_CODE);
	} else {
		printk("CRC32 of %s = %u\n", KERNEL_CODE, ans);
	}
	return 0;
}

static void __exit MODULE_NAME_exit(void) {
}

module_init(MODULE_NAME_init);
module_exit(MODULE_NAME_exit);

