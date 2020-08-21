 
# integer pid controller
A lightweight implementation of an pid controller. It was originally used to control the speed of a motor with an low cost STM32L031 micro controller. 
  
## implementation
The software works with 32bit integer variables. This enables MCUs without floating point unit to be used as a pid controller. The algorithm uses basic arithmetic and integer division. To maintain a reasonable accuracy, the variables are scales by a constant factor before calculation. 
  
The factor is called PID_MULT and be configured in "pid.c". It can be incresed to get a higher accuracy, however the limits of int32 need to be taken into account. Either try to increase the value step by step and test for stability or calculate the limit of the algorithm by hand (each interim result of the algo has to stay in range).

### other parameter
The parameter PID_KP, PID_KD, PID_KI (pid.c) and PID_TA (pid.h) can be manually set for the individual application. They are transformed into the parameter PID_Q0, PID_Q1 and PID_Q2 to work with a slightly more performant variant of the pid algorithm.  
  
The output is limited to avoid overflow issues. The parameters for limiting are PID_OUT_MIN and PID_OUT_MAX (pid.c). They are set to 0 and 1500, which corresponds to the rpm of the former motor application.

### interfaces
`void PID_Test(void)`  
A testing function to retrieve the value of the algo parameters q0, q1 and q2. This is an easy way to check, that the parameters did not suffer from overflow issues.  
  
`void PID_UpdateValues(void)`  
The core function of the implementation. It has to be called periodically in a constant interval. The most sensible way is to put this function call into a timer interrupt (callback) function. **Important:** Set the value of PID_TA to the update time of your timer implementation (in ms).  
  
`int32_t PID_GetOutput(void)`  
Retrieve the output value of the algo to perform the actual controlling part (PWM, DAC, ...).  
  
`int32_t PID_ReadCurrentSetpoint()`  
**Needs to be implemented manually!** Get the current setpoint value of your application. E.g. read the ADC value of a connected potentiometer.  
  
`int32_t PID_ReadCurrentProcessValue()`  
**Needs to be implemented manually!** Get the current process value of your application. E.g. measure the periodicity of a Hall sensor.  
