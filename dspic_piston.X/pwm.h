/*
 * File:   pwm.h
 * Author: Thomas
 *
 * Created on December 3, 2021, 1:44 PM
 */
#ifndef DSPIC_PWM_H
#define DSPIC_PWM_H

// Motor
#define MOTOR_CMD P1DC1

/*
 * [0, 4000] => [-16V, 16V] (16,8V to 14,4V)
 * Limit to +- 1500 around 2000 (1500, 3500)
 */

#define MOTOR_STOP 2000
#define MOTOR_DOWN 500
#define MOTOR_UP 3500



/*
 @brief Initialize the PWM
 */
void PWM_Initialize();

#endif

