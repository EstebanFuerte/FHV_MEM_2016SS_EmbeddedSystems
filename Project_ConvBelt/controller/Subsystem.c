/*
 * File: Subsystem.c
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

/* Block states (auto storage) */
D_Work_Subsystem Subsystem_DWork;

/* External inputs (root inport signals with auto storage) */
ExternalInputs_Subsystem Subsystem_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_Subsystem Subsystem_Y;

/* Real-time model */
RT_MODEL_Subsystem Subsystem_M_;
RT_MODEL_Subsystem *const Subsystem_M = &Subsystem_M_;

/* Model step function */
void Subsystem_step(void)
{
  real_T rtb_Sum;
  real_T rtb_Saturation;

  /* Sum: '<S2>/Sum' incorporates:
   *  DiscreteIntegrator: '<S2>/Integrator'
   *  Gain: '<S2>/Proportional Gain'
   *  Inport: '<Root>/In2'
   */
  rtb_Sum = Subsystem_P.ProportionalGain_Gain * Subsystem_U.In2 +
    Subsystem_DWork.Integrator_DSTATE;

  /* Saturate: '<S2>/Saturation' */
  if (rtb_Sum >= Subsystem_P.Saturation_UpperSat) {
    rtb_Saturation = Subsystem_P.Saturation_UpperSat;
  } else if (rtb_Sum <= Subsystem_P.Saturation_LowerSat) {
    rtb_Saturation = Subsystem_P.Saturation_LowerSat;
  } else {
    rtb_Saturation = rtb_Sum;
  }

  /* End of Saturate: '<S2>/Saturation' */

  /* Outport: '<Root>/u_ref' */
  Subsystem_Y.u_ref = rtb_Saturation;

  /* Update for DiscreteIntegrator: '<S2>/Integrator' incorporates:
   *  Gain: '<S2>/Integral Gain'
   *  Gain: '<S2>/Kb'
   *  Inport: '<Root>/In2'
   *  Sum: '<S2>/SumI1'
   *  Sum: '<S2>/SumI2'
   */
  Subsystem_DWork.Integrator_DSTATE += ((rtb_Saturation - rtb_Sum) *
    Subsystem_P.Kb_Gain + Subsystem_P.IntegralGain_Gain * Subsystem_U.In2) *
    Subsystem_P.Integrator_gainval;
}

/* Model initialize function */
void Subsystem_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(Subsystem_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&Subsystem_DWork, 0,
                sizeof(D_Work_Subsystem));

  /* external inputs */
  Subsystem_U.In2 = 0.0;

  /* external outputs */
  Subsystem_Y.u_ref = 0.0;

  /* InitializeConditions for DiscreteIntegrator: '<S2>/Integrator' */
  Subsystem_DWork.Integrator_DSTATE = Subsystem_P.Integrator_IC;
}

/* Model terminate function */
void Subsystem_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
