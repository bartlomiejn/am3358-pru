#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/sysfs.h>
#include <linux/mutex.h>
#include <linux/io.h>

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Bartlomiej Nowak");
MODULE_DESCRIPTION("Driver for AM335x pru-stopwatch firmware");
MODULE_VERSION("0.1");

#define DEVICE_NAME             "prusw"
#define PRU_MEM_ADDR            0x4A300000
#define PRU_MEM_LEN             0x80000
#define PRU_SHAREDMEM_OFFSET    0x10000
#define PRU_SHAREDMEM_LEN       0x3000      // 12KB
#define SWITCH_DELIM            (char)59    // Semicolon

typedef enum {
    first = 0,
    second = 1
} switch_idx;

static struct device* root_dev;
static struct device_attribute switch1_attr;
static struct device_attribute switch2_attr;
static DEFINE_MUTEX(sharedmem_mux);
static void __iomem *pru_mem;
static uint8_t __iomem *pru_sharedmem;
static char sharedmem_buf[256];

static ssize_t switch1_read(
    struct device *dev,
    struct device_attribute *attr,
    char *buf
);
static ssize_t switch2_read(
    struct device *dev,
    struct device_attribute *attr,
    char *buf
);
static ssize_t switch_read(switch_idx idx, char *buf);
static void read_sharedmem(void);
static ssize_t read_tok(switch_idx idx, char *buf);

static int __init prusw_init(void)
{
    int err;

    printk(KERN_INFO "prusw: Initializing\n");

    root_dev = root_device_register(DEVICE_NAME);
    if (root_dev == NULL)
    {
        printk(KERN_INFO "prusw: Failed to register root device");
        goto deinit;
    }

    switch1_attr.attr.name = "switch1";
    switch1_attr.attr.mode = 0444; // Read only
    switch1_attr.show = switch1_read;
    switch1_attr.store = NULL;
    err = sysfs_create_file(&root_dev->kobj, &switch1_attr.attr);
    if (err)
    {
        printk(KERN_INFO "prusw: Failed to create switch1");
        goto deinit_attr1;
    }

    switch2_attr.attr.name = "switch2";
    switch2_attr.attr.mode = 0444; // Read only
    switch2_attr.show = switch2_read;
    switch2_attr.store = NULL;
    err = sysfs_create_file(&root_dev->kobj, &switch2_attr.attr);
    if (err)
    {
        printk(KERN_INFO "prusw: Failed to create switch2");
        goto deinit_attr2;
    }

    pru_mem = ioremap(PRU_MEM_ADDR, PRU_MEM_LEN);
    if (!pru_mem)
    {
        printk(KERN_INFO "prusw: Failed to map PRU memory");
        goto deinit_prumem;
    }
    pru_sharedmem = (uint8_t *)pru_mem + PRU_SHAREDMEM_OFFSET;

    mutex_init(&sharedmem_mux);

    printk(KERN_INFO "prusw: Initialized\n");
    return 0;

deinit_prumem:
    sysfs_remove_file(&root_dev->kobj, &switch2_attr.attr);
deinit_attr2:
    sysfs_remove_file(&root_dev->kobj, &switch1_attr.attr);
deinit_attr1:
    root_device_unregister(root_dev);
deinit:
    return -1;
}

static void __exit prusw_exit(void)
{
    mutex_destroy(&sharedmem_mux);
    iounmap(pru_mem);
    sysfs_remove_file(&root_dev->kobj, &switch2_attr.attr);
    sysfs_remove_file(&root_dev->kobj, &switch1_attr.attr);
    root_device_unregister(root_dev);
    printk(KERN_INFO "prusw: Exited\n");
}

static ssize_t switch1_read(
    struct device *dev,
    struct device_attribute *attr,
    char *buf
){
    return switch_read(first, buf);
}

static ssize_t switch2_read(
    struct device *dev,
    struct device_attribute *attr,
    char *buf
){
    return switch_read(second, buf);
}

static ssize_t switch_read(switch_idx idx, char *buf)
{
    size_t sz;
    if (mutex_trylock(&sharedmem_mux) != 0)
    {
        while (mutex_trylock(&sharedmem_mux) != 0);
        sz = read_tok(idx, buf);
        mutex_unlock(&sharedmem_mux);
        return sz;
    }
    read_sharedmem();
    sz = read_tok(idx, buf);
    mutex_unlock(&sharedmem_mux);
    return sz;
}

static void read_sharedmem(void)
{
    uint8_t i;
    iowrite8(0, (void *)pru_sharedmem);
    while (ioread8((void *)pru_sharedmem) == 0);
    for (i = 1; i < 256; i++)
    {
        char ch = (char)ioread8(pru_sharedmem + i);
        if (ch == 0)
        {
            break;
        }
        sharedmem_buf[i - 1] = ch;
    }
    sharedmem_buf[i - 1] = (char)10;
    sharedmem_buf[i] = (char)0;
}

ssize_t read_tok(switch_idx idx, char *buf)
{
    char *char_ptr = sharedmem_buf;
    size_t tok_sz;
    int i = 0, start = 0;
    switch (idx)
    {
        case second:
            while (char_ptr[start] != SWITCH_DELIM)
            {
                start++;
            }
            start++;
            while (char_ptr[start + i] != (char)0)
            {
                buf[i] = char_ptr[start + i];
                i++;
            }
            break;
        case first:
            while (char_ptr[i] != SWITCH_DELIM)
            {
                buf[i] = char_ptr[i];
                i++;
            }
    }
    buf[i] = (char)10;
    buf[i + 1] = (char)0;
    return (ssize_t)i + 1;
}

module_init(prusw_init);
module_exit(prusw_exit);
