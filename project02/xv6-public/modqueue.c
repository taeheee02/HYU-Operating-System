#include "types.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "spinlock.h"
#include "proc.h"

extern struct queue queues[4];
extern struct queue moq;

void enqueue(struct queue *q, struct proc *p) {
  if ((q->tail + 1) % NPROC == q->head) { 
	cprintf("queue is full (Queue Level: %d)\n", q->level);
    panic("queue is full");
  }
  q->procs[q->tail] = p;
  q->tail = (q->tail + 1) % NPROC;
}

struct proc* dequeue(struct queue *q) {
  struct proc *p;

  if (q->head == q->tail) { 
	cprintf("queue is empty (Queue Level: %d)\n", q->level);
    return 0;
  }
  p = q->procs[q->head];
  q->head = (q->head + 1) % NPROC;
  return p;
}

void sortPriority(struct queue *q){
  int n = (q->tail - q->head + NPROC) % NPROC;
  struct proc *temproc;
  int tmp;
  for(int i=1; i<n; i++){
    temproc = q->procs[(q->head + i) % NPROC];
    tmp = i -1;

	while (tmp >= 0 && q->procs[(q->head + tmp) % NPROC]->pri < temproc->pri) {
      q->procs[(q->head + tmp + 1) % NPROC] = q->procs[(q->head + tmp) % NPROC];
      tmp -=  1;
    }
    q->procs[(q->head + tmp + 1) % NPROC] = temproc;
  }
}


void downgrade(struct proc *p){
  int level = p -> level;
  if (level == 0){
	if(p->pid % 2 == 1)
	  p->level = 1;
	else
	  p->level = 2;
  }
  else if(level < 3){
	p->level = 3;

  }
  else if(level == 3){
    if(p->pri > 0)
      p->pri --;
  }
  else
   panic("wrong level");
}
void properEnqueue(struct proc *p){
  p->state = RUNNABLE;
  if(p->isMonopoly){
    enqueue(&moq,p);
  }
  else{
    enqueue(&queues[p->level],p);
	sortPriority(&queues[p->level]);
  }
}

void removequeue(struct proc *p){
  struct queue *q = &queues[p->level];
  int i = q->head;
    while (i != q->tail) {
	  if (q->procs[i] == p) {
		while (i != q->tail) {
          q->procs[i] = q->procs[(i + 1) % NPROC];
          i = (i + 1) % NPROC;
        }
        q->tail = (q->tail - 1 + NPROC) % NPROC; 
        break;
      }
      i = (i + 1) % NPROC;
    }
}
