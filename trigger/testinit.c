#include <linux/module.h>	
#include <linux/kernel.h>	
#include <linux/platform_device.h>
#include <linux/init.h>             // Macros used to mark up functions e.g., __init __exit
#include <linux/module.h>           // Core header for loading LKMs into the kernel
#include <linux/kernel.h>           // Contains types, macros, functions for the kernel
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>    
#include <asm/page.h>
#include <asm/pgalloc.h>
#include <asm/pgtable.h>
#include <asm/io.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/gfp.h>

void *rx_base_addr; /* Virtual Base Address */
unsigned long *tx_base_addr; /* Virtual Base Address */
#define MAP_FAILED (void *)-1
unsigned int val =2;
char message[1000];
char buf[6]="555555";
unsigned remap_size; 
struct resource *res; /* Device Resource Structure */
extern struct mm_struct init_mm;
void *virt_addr;
/* 
 * Probe is called when the driver is about to be attached 
 */
int trigger_probe(struct platform_device *pdev)
{
////////////////////////////////////////////////////////////////////////////

      /* unsigned long page_address;
       virt_addr = (void *) get_zeroed_page(GFP_XEN);
       page_address = page_to_phys(virt_to_page(virt_addr)); 
       memcpy(virt_addr, buf, 6); */
       val = 2;
       asm volatile("mov x0, #0x9999\n\tmov x1, %0\n\thvc #0" :: "r" (val) : "x0", "x1");
      // printk(KERN_INFO "virtual address  is %p at address physical %lu\n", virt_addr, page_address);
////////////////////////////////////////////////////////////////////////////
     

       /* void *virt_addr;
        struct page *page;
        spinlock_t *ptl;
        pte_t *pte;
        unsigned long  phys_addr;
        res = platform_get_resource(pdev, IORESOURCE_MEM, 0); // get resource info
        remap_size = res->end - res->start + 1;               // get resource memory size
        phys_addr = res->start;
        rx_base_addr = ioremap(res->start, remap_size); 
        unsigned long cast = rx_base_addr; 
        printk(KERN_INFO "%lu - %p and size is %d\n", cast, rx_base_addr,res->start, remap_size);
       /* res = platform_get_resource(pdev, IORESOURCE_MEM, 1); // get resource info
        remap_size = res->end - res->start + 1;               // get resource memory size
        tx_base_addr = ioremap(res->start, remap_size);  
        printk(KERN_INFO "tx_base_addr is %lx mapped at phys %lx and size is %d\n", *tx_base_addr,res->start, remap_size); */
        
        //virt_addr = (void *) get_zeroed_page(GFP_KERNEL);
        
        //writel( 0xabab, (volatile void __iomem *)rx_base_addr);
        /*void *p0 = do_mremap(cast, 0, 4096, 2, (unsigned long) virt_addr);
        if ( p0 == MAP_FAILED ) {
           printk(KERN_INFO "mremap: mremap failed");
           return -1;
        }

         printk(KERN_INFO "p0 is %c at address %p\n", ((char *)p0)[0], p0);
         printk(KERN_INFO "old value is %x at address %p\n", readl((volatile void __iomem *)rx_base_addr), rx_base_addr); */

       /* ioremap_page_range((unsigned long)(virt_addr),(unsigned long)(virt_addr) + 4096, phys_addr,
				  pgprot_noncached(PAGE_SHARED));
        writel( 0xabab, (volatile void __iomem *)virt_addr); */
       /* page = pfn_to_page((unsigned long)(virt_addr) >> PAGE_SHIFT);
        pte = get_locked_pte(&init_mm, phys_addr, &ptl);
        set_pte_at(&init_mm, phys_addr, pte, mk_pte(page, pgprot_noncached(PAGE_SHARED)));
        spin_unlock(ptl);
        flush_tlb_all();
        writel( 0xabab, (volatile void __iomem *)page);
        //memcpy(page, buf, 6);
    asm volatile("mov x0, #0x9999\n\tmov x1, %0\n\thvc #0" :: "r" (val) : "x0", "x1"); */
    return 0;
}

/*
 * Remove is called when driver is about to be detached
 */
int trigger_remove(struct platform_device *dev)
{
	printk(KERN_INFO "%s\n", __func__);
        free_page((unsigned long)virt_addr);
	return 0;
}

static const struct of_device_id trigger_match_table[] = {
        { .compatible = "trigger"},
        { },
};

/* Platform driver definition */
static struct platform_driver trigger_driver = {
	.driver		= {
		.name	= "trigger",
		.owner	= THIS_MODULE,
                .of_match_table = of_match_ptr(trigger_match_table),
	},
	.probe		= trigger_probe,
	.remove		= trigger_remove,
};



MODULE_DEVICE_TABLE(of, trigger_match_table);

static int __init trigger_init(void){

    int ret;
    printk(KERN_INFO "Trigger: Hello  from the Trigger Module!\n");
    ret = platform_driver_register(&trigger_driver);

    if(ret) {
            printk(KERN_ERR "Unable to register driver\n");
            return ret;
    }

    return 0;
}

static void __exit trigger_exit(void){
   printk(KERN_INFO "Trigger: Goodbye from the Trigger!\n");
   platform_driver_unregister(&trigger_driver);
}


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Amna Waseem");
MODULE_DESCRIPTION("Basic Linux Kernel module for a triggering platform device");


module_init(trigger_init);
module_exit(trigger_exit);
MODULE_INFO(intree, "Y");
