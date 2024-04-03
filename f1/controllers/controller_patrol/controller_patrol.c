/*************************** START OF MY CODE ***********************/ 

#include <webots/robot.h>
#include <webots/motor.h>
#include <math.h>
#include <stdlib.h>

#define TIME_STEP 64
#define MAX_SPEED 12  // Maximum speed for the Create robot

// Function to make the robot move straight for a given duration
void move_straight(WbDeviceTag left_motor, WbDeviceTag right_motor, double duration) {
  wb_motor_set_velocity(left_motor, MAX_SPEED);
  wb_motor_set_velocity(right_motor, MAX_SPEED);
  wb_robot_step(duration * 1000);  // duration is expected to be in seconds
}

// Function to make the robot turn approximately 90 degrees
void turn_90_degrees(WbDeviceTag left_motor, WbDeviceTag right_motor) {
  // Assuming the robot turns 90 degrees in about 1.2 seconds at MAX_SPEED/2.
  double turn_duration = 1.2; // Duration to turn 90 degrees, needs calibration
  wb_motor_set_velocity(left_motor, MAX_SPEED / 2);
  wb_motor_set_velocity(right_motor, -MAX_SPEED / 2);
  wb_robot_step(turn_duration * 1000);  // Convert seconds to milliseconds
}

int main() {
  wb_robot_init();

  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");

  // Make sure the motors are in velocity control mode
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);

  // Patrol cycle: move straight then turn approximately 90 degrees
  while (wb_robot_step(TIME_STEP) != -1) {
    move_straight(left_motor, right_motor, 6.0); // Move straight for 6 seconds
    turn_90_degrees(left_motor, right_motor); // Turn approximately 90 degrees
  }

  wb_robot_cleanup();
  return 0;
}

/*************************** END OF MY CODE ***********************/ 