#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/mm.h>

/*
	Load module: sudo insmod ./osp3.ko
	Unload module: sudo rmmod osp3 
*/
MODULE_LICENSE("GPL");

static int pid = 0;
module_param(pid, int, S_IRUGO);

// Module Initialization
int init_module() {
	printk(KERN_INFO "OSP3 Initializing...\n");
	/*for_each_process(task) {
		printk("pid: %d", task->pid);
	}*/
	if(pid != 0) {
		printk("Parameter value: %d\n", pid);
		// getPCB(pid);
	}
	else
		printk("No parameter\n");
	
	return 0;
}

// Module Cleanup
void cleanup_module() {
	printk(KERN_INFO "OSP3 Exiting...\n");
}

void getPCB(int pid) {
	if(pid <= 0)
		return;
	else {
		printk("current: %lu\n", current);
	}
}
