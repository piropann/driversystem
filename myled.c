#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>

#define led0 25
#define led1 24

MODULE_AUTHOR("Ryuichi Ueda and Hirofumi Saito");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;
static unsigned int led[2] = {led0 , led1};

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
　　char c;   
　　int i;
　　if(copy_from_user(&c,buf,sizeof(char)))
	return -EFAULT;

　　if(c == '0'){
        for(i =1; i<=5; i++){
            gpio_base[10] = 1 << led0;
            gpio_base[7] = 1 << led1;
            msleep(3000);
            gpio_base[7] = 1 << led0;
            gpio_base[10] = 1 << led1;
            msleep(3000);
        }
    }
    else if(c == '1'){
        gpio_base[10] = 1 << 25;
        gpio_base[10] = 1 << 24;
    }
    return 1; 
}

static struct file_operations led_fops = {
        .owner = THIS_MODULE,
        .write = led_write,
};

static int __init init_mod(void)
{
	int retval,l;
	retval = alloc_chrdev_region(&dev, 0, 1, "myled");
	if(retval < 0){
		printk(KERN_ERR "alloc_chrdev_region failed.\n");
		return retval;
	}
	printk(KERN_INFO "%s is loaded. major:%d\n", __FILE__, MAJOR(dev));
	cdev_init(&cdv, &led_fops);
        retval = cdev_add(&cdv, dev, 1);
        if(retval < 0){
            printk(KERN_ERR "cdev_add failed. major:%d, minor:%d",MAJOR(dev),MINOR(dev));
            return retval;
	}

	cls = class_create(THIS_MODULE,"myled");  
         if(IS_ERR(cls)){
            printk(KERN_ERR "class_create failed.");
            return PTR_ERR(cls);
        }
	device_create(cls, NULL, dev, NULL, "myled%d",MINOR(dev));

	gpio_base = ioremap_nocache(0xfe200000, 0xA0);

    //const u32 led0 = 25;
    //const u32 led1 = 24;
	//int led[1] = {led0, led1};
        for(l=0; l<2; l++){
            const u32 index = led[l]/10;
            const u32 shift = (led[l]%10)*3;//15bit
            const u32 mask = ~(0x7 << shift);//11111111111111000111111111111111
            gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);
        }
	return 0;
}

static void __exit cleanup_mod(void)
{
	cdev_del(&cdv);
	device_destroy(cls, dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. major:%d\n", __FILE__, MAJOR(dev));
}

module_init(init_mod);
module_exit(cleanup_mod);
