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

long unsigned int *rx_base_addr; /* Virtual Base Address */
long unsigned int *tx_base_addr; /* Virtual Base Address */
unsigned remap_size; 
static irqreturn_t irqHandlerInit (int irq, void *dev_id)
{
        char message[1000];
        printk (KERN_NOTICE "IRQ %d interrupt fired\n", irq);
        //memset((void *)message, 0, 1000);

        //memcpy((void *)message, rx_base_addr, 1000 );

        printk(KERN_NOTICE "received string %x\n", rx_base_addr[0]);
        rx_base_addr[0] = 0x0;
        //memset(rx_base_addr, 0, remap_size);
        return IRQ_HANDLED;
}

/* 
 * Probe is called when the driver is about to be attached 
 */
int testdriver_probe(struct platform_device *pdev)
{
        int irq_num, ret;
        struct resource *res; /* Device Resource Structure */
        printk(KERN_INFO "Test Driver probed %s\n", __func__);

        res = platform_get_resource(pdev, IORESOURCE_MEM, 0); // get resource info
        remap_size = res->end - res->start + 1;               // get resource memory size
        rx_base_addr = ioremap(res->start, remap_size);  
        printk(KERN_INFO "rx_base_addr is %lu mapped at phys %lx and size is %d\n", (unsigned long)rx_base_addr,(unsigned long)res->start, remap_size);
       /* res = platform_get_resource(pdev, IORESOURCE_MEM, 1); // get resource info
        remap_size = res->end - res->start + 1;               // get resource memory size
        tx_base_addr = ioremap(res->start, remap_size);  
        printk(KERN_INFO "tx_base_addr is %lx mapped at phys %lx and size is %d\n", *tx_base_addr,res->start, remap_size); */

        //irq_num = platform_get_irq( pdev , 0 ) ;
        irq_num = 10;
        printk(KERN_INFO "Request IRQ for  %d\n", platform_get_irq( pdev , 0 ));
        ret = set_ipi_handler(irq_num, &irqHandlerInit, "IPI Handler TestDriver");
        // ret = request_irq(irq_num, &irqHandlerInit, IRQF_NO_SUSPEND, "testdriver_irq", NULL); 
        if (ret !=0 ) {
        printk(KERN_ERR "testdriver-irq: cannot register IRQ %d %d\n", irq_num, ret);
        return -1;
	}

	return 0;
}

/*
 * Remove is called when driver is about to be detached
 */
int testdriver_remove(struct platform_device *dev)
{
	printk(KERN_INFO "%s\n", __func__);

	return 0;
}

static const struct of_device_id testdriver_match_table[] = {
        { .compatible = "testdriver"},
        { },
};

MODULE_DEVICE_TABLE(of, testdriver_match_table);


/* Platform driver definition */
static struct platform_driver testdriver_driver = {
	.driver		= {
		.name	= "testdriver",
		.owner	= THIS_MODULE,
                .of_match_table = of_match_ptr(testdriver_match_table),
	},
	.probe		= testdriver_probe,
	.remove		= (testdriver_remove),
};

/*
 * Module init function
 */
static int __init testdriver_init(void)
{
    int ret;

    printk(KERN_INFO "%s\n", __func__);

    ret = platform_driver_register(&testdriver_driver);

	if(ret) {
		printk(KERN_ERR "Unable to register driver\n");
		return ret;
	}

	return 0;
}

/*
 * Module exit function
 */
static void __exit testdriver_exit(void)
{
	printk(KERN_INFO "%s\n", __func__);

    platform_driver_unregister(&testdriver_driver);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Amna Waseem");
MODULE_DESCRIPTION("Basic Linux Kernel module for a platform device");

module_init(testdriver_init);
module_exit(testdriver_exit);
