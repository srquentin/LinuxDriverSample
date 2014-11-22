#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Quentin <lee.rhapsody AT gmail.com>");

static int task_init(void)
{
       	printk(KERN_DEBUG"init simple Linux driver");
    	return result;	
}

static void task_exit(void)
{
	printk(KERN_DEBUG"exit\n");
}

module_init(task_init);
module_exit(task_exit);
