# CODE MODIFICATION

## Makefile

##### Line 3-4
sesudah diubah
```
CS333_PROJECT ?= 1
PRINT_SYSCALLS ?= 1
```

## Syscall.c

##### Line 110-113
Sesudah diubah
```C
#ifdef CS333_P1
// internally, the function prototype must be ’int’ not ’uint’ for sys_date()
extern int sys_date(void);
#endif // CS333_P1
```

##### Line 141-143
Setelah diubah
```C
#ifdef CS333_P1
[SYS_date]    sys_date,
#endif //CS333_P1
```

##### Line 175-196
Setelah diubah
```C
void
syscall(void)
{
  int num;
  struct proc *curproc = myproc();

  num = curproc->tf->eax;

  #ifdef CS333_P1 //diubah dari sini
    #ifdef PRINT_SYSCALLS
      cprintf("%s -> %d\n", syscallnames[num], num);
    #endif
  #endif //CS333_P1

  if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
    curproc->tf->eax = syscalls[num]();
  } else {
    cprintf("%d %s: unknown sys call %d\n",
            curproc->pid, curproc->name, num);
    curproc->tf->eax = -1;
  }
}
```

## Syscall.h

##### Line 24-25
Setelah diubah
```h
// student system calls begin here. Follow the existing pattern.
#define SYS_date    SYS_halt+1
```

## Usys.S

##### Line 33
Setelah diubah
```S
SYSCALL(date)
```

## User.h
##### Line 46-49
```H

#ifdef CS333_P1
int date(struct rtcdate*);
#endif // CS333_P1
```

## Sysproc.c
##### Line 101-112
Setelah diubah
```C
#ifdef CS333_P1
int
sys_date(void)
{
  struct rtcdate *d;
  
  if(argptr(0, (void*)&d, sizeof(d)) < 0) 
    return -1;
  cmostime(d);
  return 0;
}
#endif
```

## Proc.h
##### Line 36-56 (di dalam struct proc)
Setelah diubah
```H
// Per-process state
struct proc {
  uint sz;                     // Size of process memory (bytes)
  pde_t* pgdir;                // Page table
  char *kstack;                // Bottom of kernel stack for this process
  enum procstate state;        // Process state
  uint pid;                    // Process ID
  struct proc *parent;         // Parent process. NULL indicates no parent
  struct trapframe *tf;        // Trap frame for current syscall
  struct context *context;     // swtch() here to run process
  void *chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)
  
  //diubah dari sini
  #ifdef CS333_P1
  uint start_ticks;
  #endif
};
```

## Proc.c
##### Line 151-154 (didalam fungsi allocproc)
Setelah diubah
```C
  //ctrl P nomor 6. NB: ticks dalam mikrodetik
  #ifdef CS333_P1 
  p -> start_ticks = ticks;
  #endif
```

##### Line 563-613
Setelah diubah
```C
#elif defined(CS333_P1)
void
procdumpP1(struct proc *p, char *state_string)
{
  int i;
  uint pc[10];
  char *state;
  uint enumState = p->state;

  if(enumState==0) 
  {
    state="Unused";
  }
  else if(enumState==1) 
  {
    state="Embryo";
  }
  else if(enumState==2) 
  {
    state="Sleeping";
  }
  else if(enumState==3) 
  {
    state="Runnable";
  }  
  else if(enumState==4) 
  {
    state="Running";
  }
  else 
  {
    state="Zombie";
  }

  uint tick1 = ticks-p -> start_ticks;
  uint tick2 = tick1 / 1000;
  uint tick3 = tick1 % 1000;

  cprintf("%d", tick1);

  cprintf("\n%d\t%s\t%d.%d\t%s\t%d\t", p->pid, p->name, tick2, tick3, state, p->sz);

  getcallerpcs((uint*)p->context->ebp+2, pc);

  for(i=0; i<10 && pc[i] != 0; i++)
    cprintf(" %p", pc[i]);

  cprintf("\n");
  return;
}
#endif
```
