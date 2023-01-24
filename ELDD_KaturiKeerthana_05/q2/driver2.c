/***************************
Author : Katuri Keerthana
Course : PG-DESD
RollNo : 220950330005
Question 2
****************************/
/************************Headerfiles section*****************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/dev_t.h>
#include <linux/device.h>
#include <linux/types.h>


#include <linux/err.h>

#include <linux/slab.h>
#include <linux/uaccess.h>

#include <linux/ioctl.h>

#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/jiffies.h>

#include <linux/gpio.h>

/**************************Macros Definition***************************************/
#define GPIO_23 23
#define TIMEOUT 1000

#define WR __IOW('a','a',int32_t)
#define RD __IOR('a','b',int32_t)

/**************************Global variable declarations and initializations********/
static dev_t dev =0;
static struct cdev driver_cdev;

static struct timer_list driver_timer;
int count =0;
char *value;


/***************************File operations declaration***************************/
static struct fileoperations fops=
{
    .ownwer = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
    .unlocked_ioctl = ext_ioctl,
}

/****************************Function declarations*******************************/
static int device_open(struct inode *inode,struct file *file);
static int device_release(struct inode *inode,struct file *file);
static ssize_t device_read(struct file *filp,char __user *buf,size_t len,struct loff_t *loff);
static ssize_t device_write(struct file *filp,const char *buf,size_t len,struct loff_t *loff);
static int __init driver_init(void);
static void __exit driver_exit(void);

static unsigned long ext_ioctl(struct file *file,unsigned int cmd,unsigned long arg);
void timer_callback(&driver_timer,int *data);

/****************************Function definitions********************************/
static int device_open(struct inode *inode,struct file *file)
{
    printk("File opened successfully..!\n");
    return 0;
}
static int device_release(struct inode *inode,struct file *file)
{
    printk("File released successfully..!\n");
    return 0;
}
static ssize_t device_read(struct file *filp,char __user *buf,size_t len,struct loff_t *loff)
{
   
    printk("Reading from kernel\n");
    return 0;
}
static ssize_t device_write(struct file *filp,const char *buf,size_t len,struct loff_t *loff)
{
    printk("Writing to kernel\n");
    return 0;
}
void timer_callback(&driver_timer,int *data)
{
    printk("Inside timer\n");
    timer_setup(&driver_timer,set_gpio(GPIO_23),0);
    mod_timer(&driver_timer,jiffies + msecs_to_jiffies(TIMEOUT));

}
static unsigned long ext_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
{
    switch(cmd)
    {
         len =1;
        case RD:
                value = get_gpio(GPIO_23);
                 if(copy_to_user(buf,value,len))
                    {
                        printk(KERN_INFO("Reading error\n"));
                    }
                    printk("Read successfully...!");
        case WR:
                if(copy_from_user(value,buf,1))
                {
                    printk(KERN_INFO("Writing error\n"));
                }
                printk("Wrote successfully...!");
                printk("Value read = %c",value);
    }

}
static int __init driver_init(void)
{
/*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "driver2")) <0){
                pr_err("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

        /*Creating cdev structure*/
        cdev_init(&driver_cdev,&fops);

        /*Adding character device to the system*/
        if((cdev_add(&driver_cdev,dev,1)) < 0){
            pr_err("Cannot add the device to the system\n");
            goto r_class;
        }

        /*Creating struct class*/
        if(IS_ERR(dev_class = class_create(THIS_MODULE,"file_class"))){
            pr_err("Cannot create the struct class\n");
            goto r_class;
        }

        /*Creating device*/
        if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"driver2_device"))){
            pr_err("Cannot create the Device \n");
            goto r_device;
        }
        pr_info("Device Driver Insert...Done!!!\n");
        timer_setup(&driver_timer,set_gpio(GPIO_23),0);
        mod_timer(&driver_timer,jiffies + msecs_to_jiffies(TIMEOUT));
        

      return 0;

r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

/*Module exit function*/
static void __exit file_oper_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&driver_cdev);
        unregister_chrdev_region(dev, 1);
        pr_info("Device Driver Remove...Done!!!\n");
}

module init(driver_init);
module exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Katuri Keerthana");
MODULE_VERSION("1.7");