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

#define MOTOR_STOP 2000
#define MOTOR_DOWN 0
#define MOTOR_UP 4000



/*
 @brief Initialize the PWM
 */
void PWM_Initialize();

#endif

