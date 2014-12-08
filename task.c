#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Quentin <lee.rhapsody AT gmail.com>");
MODULE_DESCRIPTION("Simple Linux driver");

static int task_init(void)
{
       	pr_debug("init simple Linux driver\n");
    	return 0;	
}

static void task_exit(void)
{
	pr_debug("exit\n");
}

module_init(task_init);
module_exit(task_exit);
