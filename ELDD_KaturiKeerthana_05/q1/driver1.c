/***************************
Author : Katuri Keerthana
Course : PG-DESD
RollNo : 220950330005
Question 1
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

#include <linux/waitqueue.h>
#include <linux/semaphore.h>

/**************************Macros Definition***************************************/
#define MEMSIZE 50

/**************************Global variable declarations and initializations********/
static dev_t dev =0;
static struct cdev driver_cdev;

char *kernel_bufffer[MEMSIZE];

uint32_t read_count = 0;
uint32_t write_count = 0;
static struct task_struct *wait_thread;
wait_queue_head_t wait_queue_wq;
int wait_queue_flag = 0;

int opresult=5;
struct semaphore *sema;

/***************************File operations declaration***************************/
static struct fileoperations fops=
{
    .ownwer = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
}

/****************************Function declarations*******************************/
static int device_open(struct inode *inode,struct file *file);
static int device_release(struct inode *inode,struct file *file);
static ssize_t device_read(struct file *filp,char __user *buf,size_t len,struct loff_t *loff);
static ssize_t device_write(struct file *filp,const char *buf,size_t len,struct loff_t *loff);
static int __init driver_init(void);
static void __exit driver_exit(void);

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
    if(copy_to_user(buf,value,MEMSIZE))
    {
        printk(KERN_INFO("Reading error\n"));
    }
    wait_queue_flag =1;
    wake_up_interruptible(&wait_queue_wq);
    up(&sema);
    return MEMSIZE;
}
static ssize_t device_write(struct file *filp,const char *buf,size_t len,struct loff_t *loff)
{
    down(&sema);
    printk("Reading from kernel\n");
    if(copy_from_user(value,buf,1))
    {
        printk(KERN_INFO("Writing error\n"));
    }
    wait_queue_flag =3;
    wake_up_interruptible(&wait_queue_wq);
    return len;
}

static int wait_function(void *unused)
{
        
        while(1) 
        {
            pr_info("Waiting For Event...\n");
            wait_event_interruptible(wait_queue_wq, wait_queue_flag != 0 );
            if(wait_queue_flag == 2)
            {
                pr_info("Event Came From Exit Function\n");
                return 0;
            }
		
            if(wait_queue_flag == 1)
            {
                pr_info("Event Came From Read Function - %d\n", ++read_count);
		
	        }
		    if(wait_queue_flag == 3)
		    {
                pr_info("Event Came From write Function - %d\n", ++write_count);
                wait_queue_flag = 0;
		    }
                wait_queue_flag = 0;
        }
        return 0;
}

static int __init driver_init(void)
{
    /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 2, "driver1")) <0){
                pr_err("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

        /*Creating cdev structure*/
        cdev_init(&driver_cdev,&fops);

        /*Adding character device to the system*/
        if((cdev_add(&driver_cdev,dev,2)) < 0){
            pr_err("Cannot add the device to the system\n");
            goto r_class;
        }
    // Device files can be created from cmd 
    // mknode -m 0777 /dev/device1 MajorNo. MinorNo.
    // mknode -m 0777 /dev/device2 MajorNo. MinorNo.
        pr_info("Device Driver Insert...Done!!!\n");

        kernel_bufffer = kmalloc(MEMSIZE,"GFP_KERNEL");

        sema_init(&sema,1);
        //Initialize wait queue
        init_waitqueue_head(&wait_queue_wq);
 
        //Create the kernel thread with name 'WaitThread'
        wait_thread = kthread_create(wait_function, NULL, "WaitThread");
        if (wait_thread) {
                pr_info("Thread Created successfully\n");
                wake_up_process(wait_thread);
        } else
                pr_info("Thread creation failed\n");
            

      return 0;

r_class:
        unregister_chrdev_region(dev,2);
        return -1;
}

/*Module exit function*/
static void __exit driver_exit(void)
{
        kfree(kernel_bufffer);
		wait_queue_flag =2;
        cdev_del(&driver_cdev);
        unregister_chrdev_region(dev, 2);
        pr_info("Device Driver Remove...Done!!!\n");
}
module init(driver_init);
module exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Katuri Keerthana");
MODULE_VERSION("1.7");