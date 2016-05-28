/*
 * File: Subsystem_data.c
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

#include "Subsystem.h"
#include "Subsystem_private.h"

/* Block parameters (auto storage) */
Parameters_Subsystem Subsystem_P = {
  0.015625,                            /* Expression: I
                                        * Referenced by: '<S2>/Integral Gain'
                                        */
  0.015625,                            /* Computed Parameter: Integrator_gainval
                                        * Referenced by: '<S2>/Integrator'
                                        */
  0.0,                                 /* Expression: InitialConditionForIntegrator
                                        * Referenced by: '<S2>/Integrator'
                                        */
  8.0E-6,                              /* Expression: P
                                        * Referenced by: '<S2>/Proportional Gain'
                                        */
  18.0,                                /* Expression: UpperSaturationLimit
                                        * Referenced by: '<S2>/Saturation'
                                        */
  -18.0,                               /* Expression: LowerSaturationLimit
                                        * Referenced by: '<S2>/Saturation'
                                        */
  0.015625                             /* Expression: Kb
                                        * Referenced by: '<S2>/Kb'
                                        */
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
