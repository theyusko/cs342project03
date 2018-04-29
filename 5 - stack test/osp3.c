#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/mm.h>
#include <linux/mm_types.h>

/*
	Load module: sudo insmod ./osp3.ko pid
	Unload module: sudo rmmod osp3 
	
	Global Page Table: mm_struct ---> pgd_t* pgd;
*/

MODULE_LICENSE("GPL");

static int pid = 0;
module_param(pid, int, S_IRUGO);

unsigned long absVal(unsigned long a, unsigned long b) {
	if(a > b)
		return a - b;
	else
		return b - a;
}

// Module Cleanup
void cleanup_module() {
	printk(KERN_INFO "OSP3 Exiting...\n");
}

// Module Initialization
int init_module() {
	struct task_struct* task = NULL;
	struct mm_struct* mm = NULL;
	struct vm_area_struct* vma = NULL;

	printk(KERN_INFO "OSP3 Initializing...\n");

	for_each_process(task)
		if(task->pid == pid) {
			mm = task->mm;
			break;
		}
	
	if(task != NULL && mm != NULL) {		
		printk("-----pid: %d\n", task->pid);
		printk("-----Code Start: %lu\n", mm->start_code);
		printk("-----Code End: %lu\n", mm->end_code);
		printk("-----Code Size: %lu\n", absVal(mm->end_code, mm->start_code));
		
		printk("-----Data Start: %lu\n", mm->start_data);
		printk("-----Data End: %lu\n", mm->end_data);
		printk("-----Data Size: %lu\n", absVal(mm->start_data, mm->end_data));

		printk("-----Arg Start: %lu\n", mm->arg_start);
		printk("-----Arg End: %lu\n", mm->arg_end);
		printk("-----Arg Size: %lu\n", absVal(mm->arg_start, mm->arg_end));
		
		printk("-----Env Start: %lu\n", mm->env_start);
		printk("-----Env End: %lu\n", mm->env_end);
		printk("-----Env Size: %lu\n", absVal(mm->env_start, mm->env_end));
		
		printk("-----Total VM: %lu\n", mm->total_vm);
		printk("-----Number of Frames: %lu\n", get_mm_rss(mm));
		
		// Find Heap & Stack Boundaries
		// vm-area-start vm-area-end vm-area-size
		for(vma = mm->mmap; vma != NULL; vma = vma->vm_next) {
			if (vma->vm_start <= mm->brk && vma->vm_end >= mm->start_brk) {
				printk("-----Heap Start: %lu\n", vma->vm_start);
				printk("-----Heap End: %lu\n", vma->vm_end);
				printk("-----Heap Size: %lu", absVal(vma->vm_start, vma->vm_end));
				printk("-----Heap Size: %lu", absVal(vma->vm_start, vma->vm_end));
				printk("-----Heap Size: %lu", absVal(vma->vm_start, vma->vm_end));
			}
			else if(vma->vm_start < mm->start_stack && vma->vm_end >= mm->start_stack) {
				printk("-----Stack Start: %lu\n", vma->vm_start);
				printk("-----Stack Current End: %lu\n", vma->vm_end);
				printk("-----Stack Current Size: %lu\n", absVal(vma->vm_start, vma->vm_end));
			}
			else
				continue;
		}
	}
	else 
		printk("OSP3 ERROR: Process with pid = %d not found\n", pid);
		
	return 0;
}

/*
	stack end
	
	vma start
	
	vma end
	
	stack start
*/
