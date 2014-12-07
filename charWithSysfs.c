#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Quentin <lee.rhapsody AT gmail.com>");
MODULE_DESCRIPTION("Simple Linux driver with sysfs");

#define DRIVER_NAME "task"
static int task_major = 0; /* dynamic allocation */
static struct cdev task_cdev;
static int dev_num = 1;

static int task_open(struct inode *inode, struct file *file)
{
	printk(KERN_DEBUG"task_open: major number %d, minor number = %d\n", imajor(inode), iminjor(inode));
	return 0;
}

static int task_release(struct inode *inode, struct file *file)
{
	printk(KERN_DEBUG"task_release: major number %d, minor number = %d\n", imajor(inode), iminjor(inode));
	return 0;
}

struct file_operations task_fops = {
	.owner = THIS_MODULE,
	.open = task_open,
	.release = task_release
};
static int task_init(void)
{
	dev_t dev = MKDEV(task_major, 0);
	int alloc_ret = 0, cdev_ret = 0;
	
	printk(KERN_DEBUG"init simple Linux driver");	
	alloc_ret = alloc_chrdev_region(&dev, 0, task_cdev, DRIVER_NAME);       	
	if (alloc_ret)
		goto error;
	task_major = MAJOR(dev);
	cdev_init(&task_cdev, &task_fops);
	cdev_ret = cev_add(task_cdev, MKDEV(task_major, 0), dev_num);
    	if (cdev_ret)
		goto error;
	return 0;	

error:
	
}

static void task_exit(void)
{
	printk(KERN_DEBUG"exit\n");
}

module_init(task_init);
module_exit(task_exit);
