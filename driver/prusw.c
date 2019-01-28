#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/sysfs.h>
#include <linux/mutex.h>
#include <linux/string.h>
#include <linux/io.h>

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Bartlomiej Nowak");
MODULE_DESCRIPTION("Driver for AM335x pru-stopwatch firmware");
MODULE_VERSION("0.1");
MODULE_SOFTDEP("pre: remoteproc");

#define DEVICE_NAME             "prusw"
#define PRU_MEM_ADDR            0x4A300000
#define PRU_MEM_LEN             0x80000
#define PRU_SHAREDMEM_OFFSET    0x10000
#define PRU_SHAREDMEM_LEN       0x3000          // 12KB
#define SWITCH_STATE_LEN        32

typedef enum {
    first = 0,
    second = 1
} switch_idx;

static struct device* root_dev;
static struct device_attribute switch1_attr;
static struct device_attribute switch2_attr;
static void __iomem *pru_mem;
static uint8_t __iomem *pru_switch1_bit;
static uint8_t __iomem *pru_switch1_state;
static uint8_t __iomem *pru_switch2_bit;
static uint8_t __iomem *pru_switch2_state;
static DEFINE_MUTEX(switch1_mux);
static DEFINE_MUTEX(switch2_mux);

static int register_root_dev(void);
static int create_switch1_file(void);
static int create_switch2_file(void);
static int remap_prumem(void);
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

static int __init prusw_init(void)
{
    printk(KERN_INFO "prusw: Initializing\n");
    if (register_root_dev())
    {
        printk(KERN_INFO "prusw: Failed to register root device");
        goto deinit;
    }
    if (create_switch1_file())
    {
        printk(KERN_INFO "prusw: Failed to create switch1");
        goto deinit_attr1;
    }
    if (create_switch2_file())
    {
        printk(KERN_INFO "prusw: Failed to create switch2");
        goto deinit_attr2;
    }
    if (remap_prumem())
    {
        printk(KERN_INFO "prusw: Failed to map PRU memory");
        goto deinit_prumem;
    }
    mutex_init(&switch1_mux);
    mutex_init(&switch2_mux);
    printk(KERN_INFO "prusw: Initialized");
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
    mutex_destroy(&switch2_mux);
    mutex_destroy(&switch1_mux);
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
    ssize_t len;
    if (mutex_trylock(&switch1_mux) == 0)
        return -EBUSY;
    iowrite8(0, (void *)pru_switch1_bit);
    while (ioread8((void *)pru_switch1_bit) == 0);
    for (len = 0; len < SWITCH_STATE_LEN; len++)
    {
        char ch = (char)ioread8(pru_switch1_state + len);
        if (ch == 0)
            break;
        buf[len] = ch;
    }
    buf[len + 1] = (char)0;
    mutex_unlock(&switch1_mux);
    return len;
}

static ssize_t switch2_read(
    struct device *dev,
    struct device_attribute *attr,
    char *buf
){
    ssize_t len;
    if (mutex_trylock(&switch2_mux) == 0)
        return -EBUSY;
    iowrite8(0, (void *)pru_switch2_bit);
    while (ioread8((void *)pru_switch2_bit) == 0);
    for (len = 0; len < SWITCH_STATE_LEN; len++)
    {
        char ch = (char)ioread8(pru_switch2_state + len);
        if (ch == 0)
            break;
        buf[len] = ch;
    }
    buf[len + 1] = (char)0;
    mutex_unlock(&switch2_mux);
    return len;
}

static int register_root_dev(void)
{
    root_dev = root_device_register(DEVICE_NAME);
    return -(root_dev == NULL);
}

static int create_switch1_file(void)
{
    switch1_attr.attr.name = "switch1";
    switch1_attr.attr.mode = 0444; // Read only
    switch1_attr.show = switch1_read;
    switch1_attr.store = NULL;
    return sysfs_create_file(&root_dev->kobj, &switch1_attr.attr);
}

static int create_switch2_file(void)
{
    switch2_attr.attr.name = "switch2";
    switch2_attr.attr.mode = 0444; // Read only
    switch2_attr.show = switch2_read;
    switch2_attr.store = NULL;
    return sysfs_create_file(&root_dev->kobj, &switch2_attr.attr);
}

static int remap_prumem(void)
{
    pru_mem = ioremap(PRU_MEM_ADDR, PRU_MEM_LEN);
    if (!pru_mem)
        return -1;
    pru_switch1_bit = (uint8_t *)pru_mem + PRU_SHAREDMEM_OFFSET;
    pru_switch1_state = pru_switch1_bit + 1;
    pru_switch2_bit = pru_switch1_state + SWITCH_STATE_LEN;
    pru_switch2_state = pru_switch2_bit + 1;
    return 0;
}

module_init(prusw_init);
module_exit(prusw_exit);
