#include <msp430G2553.h>
#include "../cc1101/CC1100.h"
#include "bsp.h"
/*����P1.6ΪPWM���ε������  CCR1 */
void PWM_set(INT8U moto_num,INT16U pwm_t)
{
    if(moto_num == moto_1st) TA0CCR1= pwm_t;
    if(moto_num == moto_2nd) TA0CCR2= pwm_t;
}