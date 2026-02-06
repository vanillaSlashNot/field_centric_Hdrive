#ifndef _ROBOT_CONFIG_H_
#define _ROBOT_CONFIG_H_

#include "main.h"

// Motor ports
extern pros::Motor motor_lf1;
extern pros::Motor motor_lf2;
extern pros::Motor motor_lb1;
extern pros::Motor motor_lb2;
extern pros::Motor motor_rf1;
extern pros::Motor motor_rf2;
extern pros::Motor motor_rb1;
extern pros::Motor motor_rb2;

// IMU
extern pros::IMU imu1;

// Task functions
void fieldCentricMecanumDriveTask();
void motorDebugTask();

#endif // _ROBOT_CONFIG_H_
