#include "main.h"
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       james                                                     */
/*    Created:      Sat Aug 06 2022                                           */
/*    Description:  V5 PROS project with EZ Template                          */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// The drive used for testing had 8 motors (4 per side, 2 per corner)
pros::Motor motor_lf1(1, true, pros::MotorGears::green);   // Left front 1
pros::Motor motor_lf2(3, false, pros::MotorGears::green);  // Left front 2

pros::Motor motor_lb1(2, true, pros::MotorGears::green);   // Left back 1
pros::Motor motor_lb2(11, false, pros::MotorGears::green); // Left back 2

pros::Motor motor_rf1(10, false, pros::MotorGears::green); // Right front 1
pros::Motor motor_rf2(8, true, pros::MotorGears::green);   // Right front 2

pros::Motor motor_rb1(9, false, pros::MotorGears::green);  // Right back 1
pros::Motor motor_rb2(20, true, pros::MotorGears::green);  // Right back 2

// IMU in port 12
pros::IMU imu1(12);

//
// A task to handle motor debugging on the 8 motor drive
//
void motorDebugTask() {
  // show actual velocity of motors
  while (true) {
    pros::lcd::print(0, "LF1: %4d  |  RF1: %-4d", (int)motor_lf1.get_actual_velocity(), (int)motor_rf1.get_actual_velocity());
    pros::lcd::print(1, "LF2: %4d  |  RF2: %-4d", (int)motor_lf2.get_actual_velocity(), (int)motor_rf2.get_actual_velocity());
    pros::lcd::print(2, "LB1: %4d  |  RB1: %-4d", (int)motor_lb1.get_actual_velocity(), (int)motor_rb1.get_actual_velocity());
    pros::lcd::print(3, "LB2: %4d  |  RB2: %-4d", (int)motor_lb2.get_actual_velocity(), (int)motor_rb2.get_actual_velocity());
    pros::delay(20);
  }
}
//
// A task to handle the drive base with field-centric control
//
void fieldCentricMecanumDriveTask() {
  static double deg_to_rad = (2 * M_PI) / 360.0;
  
  // wait for imu calibration to be complete
  while (imu1.is_calibrating()) {
    pros::delay(20);
  }

  pros::Controller controller(pros::E_CONTROLLER_MASTER);

  while (true) {
    // get the control inputs
    int ctrl_fwd = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int ctrl_turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    int ctrl_right = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

    // clip values close to 0
    const int deadband = 15;
    if (abs(ctrl_fwd) < deadband)
      ctrl_fwd = 0;
    if (abs(ctrl_turn) < deadband)
      ctrl_turn = 0;
    if (abs(ctrl_right) < deadband)
      ctrl_right = 0;

    // Get IMU angle in radians
    double theta = imu1.get_heading() * deg_to_rad;
      
    // rotate coordinate system - IMU positive angle is CW
    int temp = (int)(ctrl_right * sin(theta) + ctrl_fwd * cos(theta));
    ctrl_right = (int)(ctrl_right * cos(theta) - ctrl_fwd * sin(theta));
    ctrl_fwd = temp;
    
    // Set drive motor values
    int drive_l_front = ctrl_fwd + ctrl_turn + ctrl_right;
    int drive_l_back = ctrl_fwd + ctrl_turn - ctrl_right;

    int drive_r_front = ctrl_fwd - ctrl_turn - ctrl_right;
    int drive_r_back = ctrl_fwd - ctrl_turn + ctrl_right;

    // optional normalization
    int max_rpm = 100;
    int max = abs(drive_l_front);
    if (abs(drive_l_back) > max)
      max = abs(drive_l_back);
    if (abs(drive_r_back) > max)
      max = abs(drive_r_back);
    if (abs(drive_r_front) > max)
      max = abs(drive_r_front);

    if (max > max_rpm) {
      drive_l_front = max_rpm * drive_l_front / max;
      drive_l_back = max_rpm * drive_l_back / max;
      drive_r_back = max_rpm * drive_r_back / max;
      drive_r_front = max_rpm * drive_r_front / max;
    }
  
    // send final drive values to motors
    motor_lf1.move(drive_l_front);
    motor_lf2.move(drive_l_front);
    motor_lb1.move(drive_l_back);
    motor_lb2.move(drive_l_back);
    motor_rf1.move(drive_r_front);
    motor_rf2.move(drive_r_front);
    motor_rb1.move(drive_r_back);
    motor_rb2.move(drive_r_back);

    // no need to run quickly
    pros::delay(20);
  }
}

int main() {
  // run the drivetrain in a separate task
  pros::Task drive_task(fieldCentricMecanumDriveTask);
  // start task to show motor velocity
  pros::Task debug_task(motorDebugTask);

  while (true) {
    pros::lcd::print(4, "IMU Heading: %.2f deg", imu1.get_heading());
    pros::delay(100);
  }
}
