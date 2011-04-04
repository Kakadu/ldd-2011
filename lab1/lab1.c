#include <linux/vmalloc.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/crc32.h>
#include <asm-generic/io.h>

static void *iter(void *v) 
{
	struct resource *p = v;
	if (p->child != NULL)
		return p->child;
	while ((p->sibling == NULL) && (p->parent != NULL)) 
		p = p->parent;
	return p->sibling;
}

static bool find(u32 *ans, void *v, char *name) 
{
	struct resource *p = v;
	while (p != NULL) {
		if (strcmp(name,p->name) == 0) {
			*ans = crc32(0xFFFFFFFF, phys_to_virt(p->start), 
					 	 resource_size(p) );
			return true;
		}
		p = iter(p);
	}
	return false;
}

#define KERNEL_CODE "Kernel code"

static int __init lab1_init(void) 
{		
 	u32 ans = 0;
	if (find(&ans, &iomem_resource, KERNEL_CODE)) {
		printk("CRC32 of %s = %u\n", KERNEL_CODE, ans);
	} else {
		printk("%s not found", KERNEL_CODE);
	}
	return 0;
}

static void __exit lab1_exit(void) 
{
}

module_init(lab1_init);
module_exit(lab1_exit);

