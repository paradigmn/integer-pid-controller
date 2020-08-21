#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "pid.h"


/**
 * @brief         sanity check for implementation
 * @author        paradigm
 */
int main()
{
    PID_Test();
    // PID Parameter:
    // q0: 488 q1: 480 q2: 0

    while (1)
    {
        PID_UpdateValues();
        // meas: 0 ref: 0  out: 0
        // meas: 0 ref: 0  out: 0
        // ...

        int output = PID_GetOutput();

        sleep(1);
    }
    
}
