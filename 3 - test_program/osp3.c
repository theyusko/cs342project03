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
	struct task_struct* task;
	struct mm_struct* mm;

	printk(KERN_INFO "OSP3 Initializing...\n");

	for_each_process(task) {
		if(task->pid == pid) {
			mm = task->mm;
			printk("-----pid:         %d\n", task->pid);
			printk("-----start_code:  %lu\n", mm->start_code);
			printk("-----end_code:    %lu\n", mm->end_code);
			printk("-----start_data:  %lu\n", mm->end_data);
			printk("-----start_stack: %lu\n", mm->start_stack);
			printk("-----arg_start:   %lu\n", mm->arg_start);
			printk("-----arg_end:     %lu\n", mm->arg_end);
			printk("-----env_start:   %lu\n", mm->env_start);
			printk("-----env_end:     %lu\n", mm->env_end);
			break;
		}
	}
	return 0;
}

// Module Cleanup
void cleanup_module() {
	printk(KERN_INFO "OSP3 Exiting...\n");
}
