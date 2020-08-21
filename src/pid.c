/**
 * @file          pid.c
 * @defgroup      PID
 * @brief         functions for integer PID regulator
 * @author        paradigm
 * @{
 */

/*---------------------------- I N C L U D E S -------------------------------*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "pid.h"

/*---------------------- P R I V A T E  D E F I N E S ------------------------*/
/** @defgroup  PID_PrivateDefines
 *  @{
 */

/* multiplier for integer division */
/* increase for better accuracy [1000-1000000] */
/* decrese in case of overflow issues */
#define PID_MULT													(INT32_C(1000))
/* divider for kp, ki, kd */
#define PID_DIV_K													(INT32_C(100))
/* divider for ta */
#define PID_DIV_TA													(INT32_C(1000))

/* values for pid controller in % [0-100] */
#define PID_KP														(INT32_C(48))
#define PID_KI														(INT32_C(17))
#define PID_KD														(INT32_C(0))

/* limits for output values */
#define PID_OUT_MIN													(INT32_C(0))
#define PID_OUT_MAX													(INT32_C(1500))

/* calculated interim results for pid controller with integer division */
#define _PID_KP														(PID_KP * (PID_MULT / PID_DIV_K))
#define _PID_KI														(PID_KI * (PID_MULT / PID_DIV_K))
#define _PID_KD														(PID_KD * (PID_MULT / PID_DIV_K))
#define _PID_TA														(PID_TA * (PID_MULT / PID_DIV_TA))

/* calculated constants for pid controller algorithm */
#define PID_Q0														(_PID_KP + (_PID_KI * _PID_TA) / PID_MULT + (_PID_KD / _PID_TA) * PID_MULT)
#define PID_Q1														(_PID_KP + (INT32_C(2) * _PID_KD / _PID_TA) * PID_MULT)
#define PID_Q2														((_PID_KD / _PID_TA) * PID_MULT)

/* uncomment if you want to print debug messages */
#define PID_DEBUG

/** @}
 *
 */
/*--------------------- P R I V A T E  T Y P E D E F S -----------------------*/
/** @defgroup  PID_PrivateTypedefs
 *  @{
 */

/* variables and constants for pid controller */
typedef struct PID_HandleTypeDef
{
	/* setpoint value r(k) * multiplier */
	int32_t ref;
	/* process value y(k) * multiplier */
	int32_t meas;
	/* control output u(k) */
	int32_t out;
	/* interim result for pid algorithm */
	int32_t err_0;
	int32_t err_1;
	int32_t err_2;
	int32_t out_old;
} PID_HandleTypeDef;

/** @}
 *
 */
/*-------------------- P R I V A T E  V A R I A B L E S ----------------------*/
/** @defgroup  PID_PrivateVariables
 *  @{
 */

PID_HandleTypeDef PidReg = { 0 };

/** @}
 *
 */
/*---------- P R I V A T E  F U N C T I O N  P R O T O T Y P E S -------------*/
/** @defgroup  PID_PrivateFunctions
 *  @{
 */

int32_t PID_ReadCurrentSetpoint();
int32_t PID_ReadCurrentProcessValue();

/** @}
 *
 */
/*-------------------- P U B L I C  F U N C T I O N S ------------------------*/
/** @addtogroup  PID_PublicFunctions
 *  @{
 */

/**
 * @brief         testing function for pid
 *                check constants at beginning (integer division)
 * @author        paradigm
 */
void PID_Test(void)
{
#ifdef PID_DEBUG
	printf("PID Parameter:\r\n");
	printf("q0: %d\t", PID_Q0);
	printf("q1: %d\t", PID_Q1);
	printf("q2: %d\t", PID_Q2);
	printf("\r\n");
#endif
}

/**
 * @brief         main pid routine
 *                has to be called periodically i.e. via timer interrupt
 * @author        paradigm
 */
void PID_UpdateValues(void)
{
	/* get new input values */
	PidReg.ref = PID_ReadCurrentSetpoint();
	PidReg.meas = PID_ReadCurrentProcessValue();

	/* get proportional error */
	PidReg.err_0 = PidReg.ref - PidReg.meas;
	/* calculate output value (integer division) */
	int32_t output = 0;
	output = PidReg.out_old * PID_MULT + PID_Q0 * PidReg.err_0 - PID_Q1 * PidReg.err_1 + PID_Q2 * PidReg.err_2;
	output = output / PID_MULT;
	/* limit output */
	if (output > PID_OUT_MAX) {PidReg.out = PID_OUT_MAX;}
	else if (output < PID_OUT_MIN) {PidReg.out = PID_OUT_MIN;}
	else {PidReg.out = output;}
	/* calculate other errors for next calculation */
	PidReg.err_2 = PidReg.err_1;
	PidReg.err_1 = PidReg.err_0;
	PidReg.out_old = PidReg.out;

#ifdef PID_DEBUG
	/* print controller values for real time plotting */
	printf("meas: %d\t", PidReg.meas);
	printf("ref: %d\t", PidReg.ref);
	printf("out: %d\t", PidReg.out);
	printf("\r\n");
#endif
}

/**
 * @brief         return pid output
 * @author        paradigm
 */
int32_t PID_GetOutput(void)
{
	return PidReg.out;
}

/** @}
 *
 */
/*--------------------- P R I V A T E  F U N C T I O N S ---------------------*/
/** @addtogroup  PID_PrivateFunctions
 *  @{
 */

/**
 * @brief         get current setpoint value
 * @author        paradigm
 */
int32_t PID_ReadCurrentSetpoint()
{
	// has to be implemented manually for the application in question
	// the value has to be in a fixed range for the pid controler to work properly
	// please try to scale the value between PID_OUT_MIN and PID_OUT_MAX

	// an example could be a potentiometer which dictates the speed of a motor
	// this function could aquire the desired rpm by reading an adc value and
	// using integer multiplication and division to scale it accordingly

	return 0;
}

/**
 * @brief         get current process value
 * @author        paradigm
 */
int32_t PID_ReadCurrentProcessValue()
{
	// has to be implemented manually for the application in question
	// the value has to be in a fixed range for the pid controler to work properly
	// please try to scale the value between PID_OUT_MIN and PID_OUT_MAX

	// an example could be a speed sensor which measures the speed of a motor
	// this function could aquire the current rpm by reading a sensor value and
	// using integer multiplication and division to scale it accordingly

	return 0;
}

/**
 * @}
 */

/**
 * @}
 */

