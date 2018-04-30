#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <asm/page.h>
#include <linux/highmem.h>

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

static void dump_pgd(struct task_struct *task) {
    unsigned int i = 0;
    unsigned long pgd_entry;

    /*
     * Intel® 64 and IA-32 Architectures Software Developer’s Manual, Volume 3A: System Programming Guide, Part 1
     * Section 4.5 Four Level Paging:
     * 0 (P) Present; must be 1 to reference a page-directory-pointer table
     * 1 (R/W) Read/write; if 0, writes may not be allowed to the 512-GByte region controlled by this entry
     * 2 (U/S) User/supervisor; if 0, user-mode accesses are not allowed to the 512-GByte region controlled by this entry
     * 3 (PWT) Page-level write-through; indirectly determines the memory type used to access the page-directory-pointer table
referenced by this entry
     * 4 (PCD) Page-level cache disable; indirectly determines the memory type used to access the page-directory-pointer table
referenced by this entry
     * 5 (A) Accessed; indicates whether this entry has been used for linear-address translation
     * 7 (PS) Reserved (must be 0)
     * M–1:12 Physical address of 4-KByte aligned page-directory-pointer table referenced by this entry
     * 51:M Reserved (must be 0)
     * 62:52 Ignored
     * 63 (XD) If IA32_EFER.NXE = 1, execute-disable (if 1, instruction fetches are not allowed from the 512-GByte region
controlled by this entry; see Section 4.6); otherwise, reserved (must be 0)
     *
     * https://lwn.net/Articles/117749/
     */

    for(; i < 512; i++) {
        pgd_entry = pgd_val(task->mm->pgd[i]);
        if ( (pgd_entry % 2) == 1 ) { //Last bit indicates present or not
            printk("-----pgd[%d]\n", i);

            printk("-----P: %lu \t", pgd_entry & 0X01);
            pgd_entry = pgd_entry >> 1;
            printk("-----R/W: %lu \t", pgd_entry & 0X01);
            pgd_entry = pgd_entry >> 1;
            printk("-----U/S: %lu \t", pgd_entry & 0X01);
            pgd_entry = pgd_entry >> 1;
            printk("-----PWT: %lu \n", pgd_entry & 0X01);
            pgd_entry = pgd_entry >> 1;
            printk("-----PCD: %lu \t", pgd_entry & 0X01);
            pgd_entry = pgd_entry >> 1;
            printk("-----A: %lu \t", pgd_entry & 0X01);
            pgd_entry = pgd_entry >> 2;
            printk("-----PS: %lu \n", pgd_entry & 0X01);
            pgd_entry = pgd_entry >> 5;

            printk("-----PTE bits: %lu \n", pgd_entry & 0X09);
            pgd_entry = pgd_entry >> 9;
            printk("-----PMD bits: %lu \n", pgd_entry & 0X09);
            pgd_entry = pgd_entry >> 9;
            printk("-----PUD bits: %lu \n", pgd_entry & 0X09);
            pgd_entry = pgd_entry >> 9;
            printk("-----PGD bits: %lu \n", pgd_entry & 0X09);
            pgd_entry = pgd_entry >> 9;
            //printk("----- 48-51th bits reserved to 0:", pgd_entry & 0X04);
            pgd_entry = pgd_entry >> 15;
            printk("-----XD: %lu \n", pgd_entry & 0X01);
            printk("\n");
        }
    }
}

static void dump_heap_stack(struct mm_struct* mm) {
    struct vm_area_struct* vma = NULL;

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
// Module Cleanup
void cleanup_module() {
	printk(KERN_INFO "OSP3 Exiting...\n");
}

// Module Initialization
int init_module() {
	struct task_struct* task = NULL;
	struct mm_struct* mm = NULL;

	printk(KERN_INFO "OSP3 Initializing...\n");

	for_each_process(task)
		if(task->pid == pid) {
			mm = task->mm;
			break;
		}
	
	if(task != NULL && mm != NULL) {
        printk("Part 2a\n");
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
        printk("Part 2b\n");
        dump_heap_stack(mm);

        printk("Part 2c\n");
        dump_pgd(task);

    }
	else 
		printk("OSP3 ERROR: Process with pid = %d not found\n", pid);
		
	return 0;
}
