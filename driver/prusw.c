#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/mutex.h>

#define DEVICE_NAME "prusw"
#define CLASS_NAME "prusw"

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Bartlomiej Nowak");
MODULE_DESCRIPTION("Character device driver for AM335x pru-stopwatch firmware");
MODULE_VERSION("0.1");

static int major_number;
static char message[256] = { 0 };
static short message_sz;
static int open_count = 0;
static struct class* prusw_class = NULL;
static struct device* prusw_device = NULL;
static DEFINE_MUTEX(prusw_mutex);

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

// Module lifecycle

static int __init prusw_init(void)
{
    printk(KERN_INFO "prusw: Initializing\n");
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0)
    {
        printk(KERN_ALERT "prusw: Failed to register a major number\n");
        return major_number;
    }
    printk(KERN_INFO "prusw: Registered with major number: %d\n", major_number);

    prusw_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(prusw_class))
    {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "prusw: Failed to register device class\n");
        return PTR_ERR(prusw_class); // Return an error on a pointer
    }
    printk(KERN_INFO "prusw: Device class registered\n");

    prusw_device = device_create(
        prusw_class,
        NULL,
        MKDEV(major_number, 0),
        NULL,
        DEVICE_NAME
    );
    if (IS_ERR(prusw_device))
    {
        class_unregister(prusw_class);
        class_destroy(prusw_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "prusw: Failed to create the device\n");
        return PTR_ERR(prusw_device);
    }
    printk(KERN_INFO "prusw: Device class created\n");

	mutex_init(&prusw_mutex);

    return 0;
}

static void __exit prusw_exit(void)
{
    device_destroy(prusw_class, MKDEV(major_number, 0));
    class_unregister(prusw_class);
    class_destroy(prusw_class);
    unregister_chrdev(major_number, DEVICE_NAME);
	mutex_destroy(&prusw_mutex);
    printk(KERN_INFO "prusw: Exiting\n");
}

// File operations

static int dev_open(struct inode *inodep, struct file *filep)
{
	if (!mutex_trylock(&prusw_mutex))
	{
		printk(KERN_ALERT "prusw: Device in use by another process");
		return -EBUSY;
	}
    open_count++;
    printk(KERN_INFO "prusw: Device opened %d times\n", open_count);
    return 0;
}

static ssize_t dev_read(
    struct file *filep,
    char *buffer,
    size_t len,
    loff_t *offset
){
    int errcount = 0;
    errcount = copy_to_user(buffer, message, message_sz);
    if (errcount==0)
    {
        printk(KERN_INFO "prusw: Sent %d characters\n", message_sz);
        return (message_sz=0);
    }
    else
    {
        printk(KERN_INFO "prusw: Failed to send %d characters\n", errcount);
        return -EFAULT;
    }
}

static ssize_t dev_write(
    struct file *filep,
    const char *buffer,
    size_t len,
    loff_t *offset
){
    if (copy_from_user(message, buffer, sizeof(char)*len) != 0) 
    {
        return -EFAULT;
    }
    strncpy(message, buffer, len > 255 ? 255 : len);
    printk(KERN_INFO "prusw: Received %zu characters\n", len);
    return len;
}

static int dev_release(struct inode *inodep, struct file *filep)
{
	mutex_unlock(&prusw_mutex);
    printk(KERN_INFO "prusw: Device successfully closed\n");
    return 0;
}

module_init(prusw_init);
module_exit(prusw_exit);
