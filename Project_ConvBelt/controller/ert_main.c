/*
 * File: ert_main.c
 *
 * Code generated for Simulink model 'Subsystem'.
 *
 * Model version                  : 1.58
 * Simulink Coder version         : 8.4 (R2013a) 13-Feb-2013
 * TLC version                    : 8.4 (Jan 19 2013)
 * C/C++ source code generated on : Mon Aug 11 17:27:38 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Subsystem.h"                 /* Model's header file */
#include "rtwtypes.h"                  /* MathWorks types */

/* ANSI C headers */
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* VxWorks headers */
#include <vxWorks.h>
#include <taskLib.h>
#include <sysLib.h>
#include <semLib.h>
#include <rebootLib.h>
#include <logLib.h>

/* this sets the standard stack size for spawned tasks used by the model.
 * this can be changed by compiling with '-DSTACK_SIZE=nnn' where nnn is
 * the stack size desired.
 */
#ifndef STACK_SIZE
#define STACK_SIZE                     16384
#endif

static int_T tBaseRate(SEM_ID sem, SEM_ID startStopSem)
{
  int_T i;
  while (1) {
    if (rtmGetErrorStatus(Subsystem_M) != (NULL)) {
      fprintf(stderr,"Error status: %s \n", rtmGetErrorStatus(Subsystem_M));
      semGive(startStopSem);
      return(1);
    }

    if (semTake(sem,NO_WAIT) != ERROR) {
      logMsg("Rate for SingleRate task too fast.\n",0,0,0,0,0,0);
    } else {
      semTake(sem, WAIT_FOREVER);
    }

    /* Set model inputs here */
	 Subsystem_U.In2 = -getErrorC();		// to call c++ method in controller.c

    /* Step the model */
    Subsystem_step();

    /* Get model outputs here */
	writeAnalog(0,Subsystem_Y.u_ref*220+2660));
	//220 digits ~= 1 Volt; 
	//2660 is approx. the zero speed value
  }

  return(1);
}

/*
 * Spawn Subsystem_main as an independent VxWorks task from your
 * application code, specifying its O/S priority
 */
int_T Subsystem_main(int_T priority)
{
  const char *status;
  real_T requestedSR, actualSR;
  int_T VxWorksTIDs[1];
  SEM_ID rtTaskSemaphoreList[1];
  SEM_ID rtClockSem, startStopSem;
  printf("Warning: The simulation will run forever. "
         "To change this behavior select the 'MAT-file logging' option.\n");
  fflush((NULL));
  if (priority <= 0 || priority > 255-(1)+1) {
    priority = 30;
  }

  rtClockSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
  startStopSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);

  /* Initialize model */
  Subsystem_initialize();
  requestedSR = 1.0 / 0.015625;
  sysAuxClkDisable();
  sysAuxClkRateSet((int_T)(requestedSR + 0.5));
  actualSR = (real_T) sysAuxClkRateGet();
  printf("Actual sample rate in Hertz: %f\n",actualSR);
  VxWorksTIDs[0] = taskSpawn("tBaseRate",
    priority, VX_FP_TASK, STACK_SIZE, (FUNCPTR)tBaseRate, (int_T) rtClockSem,
                             (int_T) startStopSem, (int_T) rtTaskSemaphoreList,
    0, 0, 0, 0, 0, 0, 0);
  if (sysAuxClkConnect((FUNCPTR) semGive, (int_T) rtClockSem) == OK) {
    rebootHookAdd((FUNCPTR) sysAuxClkDisable);
    printf("\nSimulation starting\n");
    sysAuxClkEnable();
  }

  semTake(startStopSem, WAIT_FOREVER);
  sysAuxClkDisable();
  taskDelete(VxWorksTIDs[0]);
  semDelete(rtClockSem);
  semDelete(startStopSem);

  /* Disable rt_OneStep() here */

  /* Terminate model */
  Subsystem_terminate();
  return(EXIT_SUCCESS);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
