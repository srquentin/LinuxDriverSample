#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sysfs.h>
#include <linux/device.h> /* class_create */

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
	pr_debug("task_open: major number %d, minor number = %d\n", imajor(inode), iminjor(inode));
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
	cdev_init(&task_cdev, &task_fops);
	task_cdev.owner = THIS_MODULE;

	ret = cev_add(&task_cdev, dev, dev_num);
    	if (cdev_ret) {
		pr_debug("fail to add char device\n");
		goto error;
	}
	/* sysfs */
	task_class = class_create(THIS_MODULE, DRIVER_NAME);
	if (IS_ERR(task_class)) {
		pr_debug("fail to create class\n");
		return -1;	
	}
	
	device_create(task_class, NULL, dev, NULL, "mydev%d", 0);
	return 0;	

error:
	task_exit();
	
}

static void task_exit(void)
{
	pr_debug("exit\n");
}

module_init(task_init);
module_exit(task_exit);
