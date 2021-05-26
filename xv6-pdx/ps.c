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
    struct uproc currentProc = proc[i];
    uint Ticks = currentProc.elapsed_ticks;
    uint TicksSecond = Ticks/1000;
    uint TicksMs = Ticks%1000;
    char* zeros = "";
    uint cpuTicksTotal = currentProc.CPU_total_ticks;
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
    printf(
      1,
      "%d\t%s\t\t%d\t%d\t%d\t%d.%s%d\t%d.%s%d\t%s\t%d\n",
      currentProc.pid,
      currentProc.name,
      currentProc.uid,
      currentProc.gid,
      currentProc.ppid,
      TicksSecond,
      zeros,
      TicksMs,
      cpuTicksTotalSecond,
      cpuZeros,
      cpuTicksTotalMs,
      currentProc.state,
      currentProc.size
    );
  }
  free(proc);
  exit();
}
#endif
