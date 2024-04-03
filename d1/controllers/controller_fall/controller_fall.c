/*************************** START OF MY CODE ***********************/ 

#include <webots/device.h>
#include <webots/motor.h>
#include <webots/robot.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define TIME_STEP 16

void my_step() {
  if (wb_robot_step(TIME_STEP) == -1) {
    wb_robot_cleanup();
    exit(EXIT_SUCCESS);
  }
}

int main(int argc, char **argv) {
  wb_robot_init();

  // Get device tags for torso and legs
  WbDeviceTag torso_pitch = wb_robot_get_device("torsoPitch");
  WbDeviceTag right_leg_pitch = wb_robot_get_device("rightLegPitch");
  WbDeviceTag left_leg_pitch = wb_robot_get_device("leftLegPitch");

  // Get device tags for arms
  WbDeviceTag left_arm_shx = wb_robot_get_device("LArmShx");
  WbDeviceTag right_arm_shx = wb_robot_get_device("RArmShx");

  // Initial lean and step preparation
  wb_motor_set_position(torso_pitch, 0.1); // Lean torso forward
  wb_motor_set_position(right_leg_pitch, 0.0);
  wb_motor_set_position(left_leg_pitch, 0.0);

  // Position arms to not obstruct the fall
  wb_motor_set_position(left_arm_shx, -1.4);  // Move left arm
  wb_motor_set_position(right_arm_shx, 1.4);  // Move right arm

  // Simulate a step and potential fall
  for (int i = 0; i < 100; i++) {
    double position = sin((double)i / 100.0 * M_PI) * 0.5; // Step motion
    wb_motor_set_position(right_leg_pitch, position); // Move right leg
    my_step();
  }

  // Continue simulation
  while (true) {
    my_step();
  };

  return EXIT_FAILURE;
}

/*************************** END OF MY CODE ***********************/ 