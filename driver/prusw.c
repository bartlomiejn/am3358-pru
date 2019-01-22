#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/rpmsg.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/io.h>

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Bartlomiej Nowak");
MODULE_DESCRIPTION("Driver for AM335x pru-stopwatch firmware");
MODULE_VERSION("0.1");

#define CLASS_NAME      "prusw"
#define DEVICE1_NAME    "prusw1"
#define DEVICE2_NAME    "prusw2"
#define DEVICE1_MINOR   0
#define DEVICE2_MINOR   1

#define PRU_MEM_ADDR            0x4A300000
#define PRU_MEM_LEN             0x80000
#define PRU_SHAREDMEM_OFFSET    0x10000
#define PRU_SHAREDMEM_LEN       0x3000      // 12KB

static int major_number;
static struct class* prusw_class = NULL;
static struct device* prusw_device1 = NULL;
static struct device* prusw_device2 = NULL;
static DEFINE_MUTEX(prusw_mutex);

void *pru_mem;
void *pru_shared_mem;

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
static void __exit prusw_exit(void);

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
    major_number = register_chrdev(0, DEVICE1_NAME, &fops);
    if (major_number < 0)
    {
        printk(KERN_ALERT "prusw: Failed to register a major number\n");
        return major_number;
    }
    printk(KERN_INFO "prusw: Registered with major number: %d\n", major_number);

    prusw_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(prusw_class))
    {
        unregister_chrdev(major_number, DEVICE1_NAME);
        printk(KERN_ALERT "prusw: Failed to register device class\n");
        return PTR_ERR(prusw_class); // Return an error on a pointer
    }
    printk(KERN_INFO "prusw: Device class registered\n");

    prusw_device1 = device_create(
        prusw_class,
        NULL,
        MKDEV(major_number, DEVICE1_MINOR),
        NULL,
        DEVICE1_NAME
    );
    if (IS_ERR(prusw_device1))
    {
        class_unregister(prusw_class);
        class_destroy(prusw_class);
        unregister_chrdev(major_number, DEVICE1_NAME);
        printk(KERN_ALERT "prusw: Failed to create device 1\n");
        return PTR_ERR(prusw_device1);
    }
    printk(KERN_INFO "prusw: Device 1 created\n");

    prusw_device2 = device_create(
        prusw_class,
        NULL,
        MKDEV(major_number, DEVICE2_MINOR),
        NULL,
        DEVICE2_NAME
    );
    if (IS_ERR(prusw_device2))
    {
        device_destroy(prusw_class, MKDEV(major_number, DEVICE1_MINOR));
        class_unregister(prusw_class);
        class_destroy(prusw_class);
        unregister_chrdev(major_number, DEVICE1_NAME);
        printk(KERN_ALERT "prusw: Failed to create device 2\n");
        return PTR_ERR(prusw_device2);
    }
    printk(KERN_INFO "prusw: Device 2 created\n");

    pru_mem = ioremap(PRU_MEM_ADDR, PRU_MEM_LEN);
    if (!pru_mem)
    {
        device_destroy(prusw_class, MKDEV(major_number, DEVICE1_MINOR));
        device_destroy(prusw_class, MKDEV(major_number, DEVICE2_MINOR));
        class_unregister(prusw_class);
        class_destroy(prusw_class);
        unregister_chrdev(major_number, DEVICE1_NAME);
        printk(KERN_INFO "prusw: Failed to access PRU memory");
        return -ENOMEM;
    }
    pru_shared_mem = pru_mem + PRU_SHAREDMEM_OFFSET;
    printk(KERN_INFO "prusw: PRU memory mapped\n");

    mutex_init(&prusw_mutex);

    return 0;
}

static void __exit prusw_exit(void)
{
    iounmap(pru_mem);
    device_destroy(prusw_class, MKDEV(major_number, DEVICE1_MINOR));
    device_destroy(prusw_class, MKDEV(major_number, DEVICE2_MINOR));
    class_unregister(prusw_class);
    class_destroy(prusw_class);
    unregister_chrdev(major_number, DEVICE1_NAME);
    mutex_destroy(&prusw_mutex);
    printk(KERN_INFO "prusw: Exited\n");
}

// File operations

static int dev_open(struct inode *inodep, struct file *filep)
{
    if (!mutex_trylock(&prusw_mutex))
    {
        printk(KERN_ALERT "prusw: Device in use by another process");
        return -EBUSY;
    }
    return 0;
}

static ssize_t dev_read(
    struct file *filep,
    char *buffer,
    size_t len,
    loff_t *offset
){
    const char* test_msg = "prusw output test\n";
    size_t test_msg_sz = strlen(test_msg);
    size_t count = len;
    ssize_t retval = 0;
    unsigned long ret = 0;
    if (*offset >= test_msg_sz)
    {
        return retval;
    }
    if (*offset + len > test_msg_sz)
    {
        count = test_msg_sz - *offset;
    }
    ret = copy_to_user(buffer, test_msg, count);
    *offset += count - ret;
    retval = count - ret;
    printk(KERN_INFO "prusw: Sent %d characters\n", count);
    return retval;
}

static ssize_t dev_write(
    struct file *filep,
    const char *buffer,
    size_t len,
    loff_t *offset
){
    printk(KERN_INFO "prusw: Operation unsupported");
    return -EINVAL;
}

static int dev_release(struct inode *inodep, struct file *filep)
{
    mutex_unlock(&prusw_mutex);
    printk(KERN_INFO "prusw: Device closed\n");
    return 0;
}

module_init(prusw_init);
module_exit(prusw_exit);
