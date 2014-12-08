#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sysfs.h>
#include <linux/device.h> /* class_create */
#include <linux/version.h> 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Quentin <lee.rhapsody AT gmail.com>");
MODULE_DESCRIPTION("Simple Linux driver with sysfs");

#define DRIVER_NAME "task"
static int task_major = 0; /* dynamic allocation */
static struct cdev task_cdev;
static int dev_num = 1;

static struct class *task_class = NULL;

static int task_open(struct inode *inode, struct file *file)
{
	pr_debug("task_open: major number %d, minor number = %d\n", imajor(inode), iminor(inode));
	return 0;
}

static int task_release(struct inode *inode, struct file *file)
{
	pr_debug("task_release\n");
	return 0;
}

struct file_operations task_fops = {
	.owner = THIS_MODULE,
	.open = task_open,
	.release = task_release
};
static int task_init(void)
{
	dev_t dev;
	int ret = 0;
	
	pr_debug("init simple Linux driver\n");	
	ret = alloc_chrdev_region(&dev, 0, dev_num, DRIVER_NAME);       	
	if (ret < 0) {
		pr_debug("fail to alloc char device\n");
		return ret;
	}
	task_major = MAJOR(dev);
	cdev_init(&task_cdev, &task_fops);
	task_cdev.owner = THIS_MODULE;

	ret = cdev_add(&task_cdev, dev, dev_num);
    	if (ret) {
		pr_debug("fail to add char device\n");
		return -1;
	}
	/* sysfs */
	
	/* class_create
	param 1: struct module *owner
	param 2:const char *name
        */
	task_class = class_create(THIS_MODULE, DRIVER_NAME);
	if (IS_ERR(task_class)) {
		pr_debug("fail to create class\n");
		return -1;	
	}
	/* device_create
	struct class *: device should be registered to
	struct class_device *: parent
	dev_t: char device to be added
	struct device * : a struct device
        */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,26)
	device_create(task_class, NULL, dev, NULL, "mydev%d", 0);
#else
	class_device_create(task_class, NULL, dev, NULL, "mydev%d", 0);
#endif
	return 0;	
}

static void task_exit(void)
{
	dev_t dev;
	
	pr_debug("exit\n");
	dev = MKDEV(task_major, 0);
	cdev_del(&task_cdev);
	unregister_chrdev_region(dev, dev_num);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,26)
	device_destroy(task_class, dev);
#else
	class_device_destroy(task_class, dev_num);
#endif
	class_destroy(task_class);	
}

module_init(task_init);
module_exit(task_exit);
