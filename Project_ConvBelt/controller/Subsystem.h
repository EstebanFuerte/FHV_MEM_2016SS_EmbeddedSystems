/*
 * File: Subsystem.h
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

#ifndef RTW_HEADER_Subsystem_h_
#define RTW_HEADER_Subsystem_h_
#ifndef Subsystem_COMMON_INCLUDES_
# define Subsystem_COMMON_INCLUDES_
#include <stddef.h>
#include <string.h>
#include "rtwtypes.h"
#endif                                 /* Subsystem_COMMON_INCLUDES_ */

#include "Subsystem_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real_T Integrator_DSTATE;            /* '<S2>/Integrator' */
} D_Work_Subsystem;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T In2;                          /* '<Root>/In2' */
} ExternalInputs_Subsystem;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T u_ref;                        /* '<Root>/u_ref' */
} ExternalOutputs_Subsystem;

/* Parameters (auto storage) */
struct Parameters_Subsystem_ {
  real_T IntegralGain_Gain;            /* Expression: I
                                        * Referenced by: '<S2>/Integral Gain'
                                        */
  real_T Integrator_gainval;           /* Computed Parameter: Integrator_gainval
                                        * Referenced by: '<S2>/Integrator'
                                        */
  real_T Integrator_IC;                /* Expression: InitialConditionForIntegrator
                                        * Referenced by: '<S2>/Integrator'
                                        */
  real_T ProportionalGain_Gain;        /* Expression: P
                                        * Referenced by: '<S2>/Proportional Gain'
                                        */
  real_T Saturation_UpperSat;          /* Expression: UpperSaturationLimit
                                        * Referenced by: '<S2>/Saturation'
                                        */
  real_T Saturation_LowerSat;          /* Expression: LowerSaturationLimit
                                        * Referenced by: '<S2>/Saturation'
                                        */
  real_T Kb_Gain;                      /* Expression: Kb
                                        * Referenced by: '<S2>/Kb'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_Subsystem {
  const char_T *errorStatus;
};

/* Block parameters (auto storage) */
extern Parameters_Subsystem Subsystem_P;

/* Block states (auto storage) */
extern D_Work_Subsystem Subsystem_DWork;

/* External inputs (root inport signals with auto storage) */
extern ExternalInputs_Subsystem Subsystem_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExternalOutputs_Subsystem Subsystem_Y;

/* Model entry point functions */
extern void Subsystem_initialize(void);
extern void Subsystem_step(void);
extern void Subsystem_terminate(void);

/* Real-time Model object */
extern RT_MODEL_Subsystem *const Subsystem_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('Motor/Subsystem')    - opens subsystem Motor/Subsystem
 * hilite_system('Motor/Subsystem/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Motor'
 * '<S1>'   : 'Motor/Subsystem'
 * '<S2>'   : 'Motor/Subsystem/Discrete PI Speed'
 */
#endif                                 /* RTW_HEADER_Subsystem_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
