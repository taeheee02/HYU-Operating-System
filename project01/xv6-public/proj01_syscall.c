#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "stddef.h"

// Simple System call
int getgpid(struct proc* p) {
    if (!p->parent || !p->parent->parent)
        return -1;
    
    return p->parent->parent->pid; 
}

// Wrapper for the system call.
int sys_getgpid(void) {
    struct proc* current_proc = myproc();
	if(!current_proc)
		return -1;
    return getgpid(current_proc);
}

