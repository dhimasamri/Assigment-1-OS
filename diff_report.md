# CODE MODIFICATION

## Makefile
##### Line 3-4
sesudah diubah
```
CS333_PROJECT ?= 2
PRINT_SYSCALLS ?= 0
```

## usys.S
##### Line 34-39
```
SYSCALL(getuid)
SYSCALL(getgid)
SYSCALL(getppid)
SYSCALL(setuid)
SYSCALL(setgid)
SYSCALL(getprocs) 
```

## user.h
##### Line 51-58
```
#ifdef CS333_P2
uint getuid(void);
uint getgid(void);
uint getppid(void);
int setuid(uint);
int setgid(uint);
int getprocs(uint max, struct uproc* table);
#endif// CS333_P2 
```

## syscall.c
##### Line 113-120
```C
#ifdef CS333_P2
extern int sys_getuid(void);
extern int sys_getgid(void);
extern int sys_getppid(void);
extern int sys_setuid(void);
extern int sys_setgid(void);
extern int sys_getprocs(void);
#endif //CS333_P2
```

##### Line 150-157
```C
#ifdef CS333_P2
[SYS_getuid]    sys_getuid,
[SYS_getgid]    sys_getgid,
[SYS_getppid]   sys_getppid,
[SYS_setuid]    sys_setuid,
[SYS_setgid]    sys_setgid,
[SYS_getprocs]  sys_getprocs,
#endif //CS333_P2
```

## proc.h
##### Line 55-60
```H
  #ifdef CS333_P2
    uint uid;
    uint gid;
    uint cpu_ticks_total;
    uint cpu_ticks_in;
  #endif
```

## syscall.h
##### Line 26-31
```H
#define SYS_getuid      SYS_date+1
#define SYS_getgid      SYS_getuid+1
#define SYS_getppid     SYS_getgid+1
#define SYS_setuid      SYS_getppid+1
#define SYS_setgid      SYS_setuid+1
#define SYS_getprocs    SYS_setgid+1
```

## sysproc.c
##### Line 13-15
```C
#ifdef CS333_P2
#include "uproc.h"
#endif  // CS333_P2
```

##### Line 118-176
```C
#ifdef CS333_P2
int
sys_getuid(void)
{
  struct proc *curproc = myproc();
  return curproc->uid;
}
int
sys_getgid(void)
{
  struct proc *curproc = myproc();
  return curproc->gid;
}
int
sys_getppid(void)
{
  struct proc *curproc = myproc();
  struct proc *parent = curproc->parent;
  return parent != NULL ? parent->pid : 0;
}
int sys_setuid(void)
{
  uint uid;
  struct proc *curproc = myproc();
  if(argint(0, (int*)&uid) >= 0) {
    if(uid >= 0 && uid <= 32767) {
      curproc->uid = uid;
      return 0;
    }
  }
  return -1;
}
int sys_setgid(void)
{
  uint gid;
  struct proc *curproc = myproc();
  if(argint(0, (int*)&gid) >= 0) {
    if(gid >= 0 && gid <= 32767) {
      curproc->gid = gid;
      return 0;
    }
  }
  return -1;
}

int sys_getprocs(void)
{
  uint max;
  struct uproc* proc;
  if (argint(0,(int*)&max) >= 0) {
    if (max == 1 || max == 16 || max == 64 || max == 72) {
      if (argptr(1, (void*)&proc, sizeof(struct uproc)) >= 0) {
        return getprocs(max, proc);
      }
    }
  }
  return -1;
}
#endif //CS333_P2 
```

## testsetuid.c
```C
#ifdef CS333_P2
#include "types.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  printf(1, "***** In %s: my uid is %d\n\n", argv[0], getuid());
  exit();
}
#endif 
```

## time.c
```C
ifdef CS333_P2
#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  if(argc == 1) {
    printf(1, "(null) ran in 0.00\n");
  } else {
    int start = uptime();
    int pid = fork();
    if (pid > 0) {
      pid = wait();
    } else if (pid == 0) {
      exec(argv[1], argv+1);
      printf(1, "error : variable unknown");
      kill(getppid());
      exit();
    } else {
      printf(1, "error : fork return error -1\n");
    }
    int finsih = uptime();
    int time = finsih - start;
    int sec = time/1000;
    int ms = time%1000;
    char *msZeros = "";
    if (ms < 10) {
      msZeros = "00";
    } else if (ms < 100) {
      msZeros = "0";
    }
    printf(1, "%s ran in %d.%s%d\n", argv[1], sec, msZeros, ms);
  }

  exit();
}
#endif
```

## ps.c
##### Line 1-40
```C
#ifdef CS333_P2
#include "types.h"
#include "user.h"
#include "uproc.h"
#define MAX 16

int
main(void)
{
  struct uproc *proc = malloc(sizeof(struct uproc)*MAX);
  int procsNumber = getprocs(MAX, proc);
  printf(1,"PID\tName\t\tUID\tGID\tPPID\tElapsed\tCPU\tState\tSize\n");
  int i;
  for(i = 0; i<procsNumber; i++){
    struct uproc cP = proc[i];
    uint Ticks = cP.elapsed_ticks;
    uint TicksSecond = Ticks/1000;
    uint TicksMs = Ticks%1000;
    char* zeros = "";
    uint cpuTicksTotal = cP.CPU_total_ticks;
    uint cpuTicksTotalSecond = cpuTicksTotal/1000;
    uint cpuTicksTotalMs = cpuTicksTotal % 1000;
    char* cpuZeros = "";
    if (TicksMs < 10) {
      zeros = "00";
    } else if (TicksMs < 100) {
      zeros = "0";
    }
    if(cpuTicksTotalMs < 10){
      cpuZeros = "00";
    } else if (cpuTicksTotalMs < 100) {
      cpuZeros = "0";
    }
    printf(1, "%d\t%s\t\t%d\t%d\t%d\t%d.%s%d\t%d.%s%d\t%s\t%d\n", cP.pid, cP.name, cP.uid, cP.gid, cP.ppid, TicksSecond, zeros, TicksMs, cpuTicksTotalSecond, cpuZeros, cpuTicksTotalMs, cP.state, cP.size);
  }
  free(proc);
  exit();
}
#endif

```

## proc.c
##### Line 9-11
Sesudah diubah
```C
#ifdef CS333_P2
#include "uproc.h"
#endif
```

##### Line 157-160 (dalam fungsi allocproc)
Sesudah diubah
```C
#ifdef CS333_P2
  p->cpu_ticks_total = 0;
  p->cpu_ticks_in = 0;
#endif //CS333_P2
```

##### Line 187-190 (dalam fungsi userinit)
```C
#ifdef CS333_P2
  p->uid = DEFAULT_UID;
  p->gid = DEFAULT_GID;
#endif // CS333_P2
```

##### Line 251-254 (dalam fungsi fork)
```C
#ifdef CS333_P2
  np->uid = curproc->uid;
  np->gid = curproc->gid;
#endif //CS333_P2
```

##### Line 409-411 (dalam fungsi scheduler)
```C
   #ifdef CS333_P2
      p->cpu_ticks_in = ticks;
    #endif // CS333_P2
```

##### Line 454-456 (dalam fungsi sched)
```C
#ifdef CS333_P2
  p->cpu_ticks_total += (ticks - p->cpu_ticks_in);
#endif // CS333_P2
```

##### Line 583-608 (fungsi ProcdumpP2P3P4)
```C
procdumpP2P3P4(struct proc *p, char *state_string)
{
  uint tick = ticks-p->start_ticks;
  uint tick2 = (tick)/1000;
  uint tick3 = tick%1000;
  char *zeros = "";
  char *cpuZeros = "";
  uint cpuTicksTotal = p->cpu_ticks_total;
  uint cpuSecond = cpuTicksTotal/1000;
  uint cpuMs = cpuTicksTotal%1000;
  uint ppid = p->parent ? p->parent->pid : p->pid;

  if (tick3<10) {
    zeros = "00";
  } else if (tick3<100) {
    zeros = "0";
  }

  if (cpuMs<10) {
    cpuZeros = "00";
  } else if (cpuMs<100) {
    cpuZeros = "0";
  }

  cprintf("\n%d\t%s\t%d\t%d\t%d\t%d.%s%d\t%d.%s%d\t%s\t%d\t", p->pid, p->name, p->uid, p->gid, ppid, tick2, zeros, tick3, cpuSecond, cpuZeros, cpuMs, state_string, p->sz);
}
```

##### Line 637-645 (dalam procdump)
```C
  #if defined(CS333_P4)
    #define HEADER "\nPID\tName\tUID\tGID\tPPID\tPrio\tElapsed\tCPU\tState\tSize\t PCs\n"
  #elif defined(CS333_P2)
    #define HEADER "\nPID\tName\tUID\tGID\tPPID\tElapsed\tCPU\tState\tSize\t PCs\n"
  #elif defined(CS333_P1)
    #define HEADER "\nPID\tName\tElapsed\tState\tSize\t PCs\n"
  #else
    #define HEADER "\n"
  #endif
```

##### Line 976-1009
```C
#ifdef CS333_P2
int
getprocs(uint max, struct uproc* upTable){
  struct proc* p;
  int procsNumber = 0;
  acquire(&ptable.lock);

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if (procsNumber < max) {
      if(p->state != UNUSED && p->state != EMBRYO) {
        if(p->state >= 0 && p->state < NELEM(states) && states[p->state]){
          safestrcpy(upTable[procsNumber].state, states[p->state],STRMAX);
        } else {
          safestrcpy(upTable[procsNumber].state,"???",STRMAX);
        }

        upTable[procsNumber].pid = p->pid;
        upTable[procsNumber].uid = p->uid;
        upTable[procsNumber].gid = p->gid;
        upTable[procsNumber].ppid = p->parent ? p->parent->pid : p->pid;
        upTable[procsNumber].elapsed_ticks = ticks - p->start_ticks;
        upTable[procsNumber].CPU_total_ticks = p->cpu_ticks_total;
        upTable[procsNumber].size = p->sz;
        safestrcpy(upTable[procsNumber].name, p->name, STRMAX);
        procsNumber++;
      }
    } else {
      break;
    }
  }
  release(&ptable.lock);
  return procsNumber;
}
#endif // CS333_P2
```
