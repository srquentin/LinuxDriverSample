#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h> /* kthread_create(), kthread_run() */
#include <linux/sched.h> /* wake_up_process() */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Quentin <lee.rhapsody AT gmail.com>");
MODULE_DESCRIPTION("use kthread to monitor status");

static struct task_struct *kthread_handler;

static DECLARE_WAIT_QUEUE_HEAD(queue);
EXPORT_SYMBOL(queue);

static int kthread_fun(void *arg);
static int kthread_id = 0;

static void call_user(void)
{
	int ret = 0;
	char path[] = "/bin/touch";  /*create a file*/
	char *argv[] = {path, "/CreateFromKernel.txt", NULL};
	char *envp[] = {"HOME=/", "PATH=/sbin:/usr/sbin:/bin:/usr/bin", NULL};
  
	ret = call_usermodehelper(path, argv, envp, UMH_WAIT_PROC); 
  
 	if (ret) 
    		pr_debug("Fail to use call_usermodehelper\n");
} 
static int kthread_fun(void *arg)
{
	DECLARE_WAITQUEUE(wait, current);
	allow_signal(SIGKILL); /* SIGKILL signal */
	add_wait_queue(&queue, &wait);
	while (1) {
		if (kthread_should_stop())
			break;
		if (signal_pending(current)) { 
   			pr_debug("[kthread_%d] received a SIGKILL signal\n", kthread_id);
   			break;
  		}
		set_current_state(TASK_INTERRUPTIBLE);
		schedule(); /* schedule_timeout(HZ) */
		/* Detect */
		/* user another module to call "wake_up_interruptible(&queue);" */
		call_user();
		
	}
	set_current_state(TASK_RUNNING);
	remove_wait_queue(&queue, &wait);
	return 0;
}
static int task_init(void)
{
	/* no input to kthread_fun, so set the second param to NULL*/       	
	kthread_handler = kthread_run(kthread_fun, NULL, "kthread_%d", kthread_id);
	if  (IS_ERR(kthread_handler))  
		pr_debug("Fail to create kthread!\n");  

       	return 0;	
}

static void task_exit(void)
{
	pr_debug("exit\n");
	if (kthread_handler) {
		kthread_stop(kthread_handler);
		kthread_handler = NULL;
	}
}

module_init(task_init);
module_exit(task_exit);
