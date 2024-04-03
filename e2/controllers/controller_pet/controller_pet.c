/*************************** START OF MY CODE ***********************/ 

#include <webots/robot.h>
#include <webots/motor.h>
#include <math.h>

#define TIME_STEP 64

int main(int argc, char **argv) {
  wb_robot_init();

  // Get device tags for the arm and shoulder motors
  WbDeviceTag armUpper = wb_robot_get_device("ArmUpperR");
  WbDeviceTag shoulderR = wb_robot_get_device("ShoulderR");
  WbDeviceTag armLowerR = wb_robot_get_device("ArmLowerR");

  // Set initial positions for ArmUpper and ShoulderR to start the petting motion
  wb_motor_set_position(armUpper, 1.1);
  wb_motor_set_position(shoulderR, -0.5);

  double minArmLowerRPosition = -0.7; // Minimum petting position
  double maxArmLowerRPosition = -0.3; // Maximum petting position
  double pettingSpeed = 0.05; // Speed of the petting motion
  double position = minArmLowerRPosition;
  double direction = pettingSpeed; // Start moving towards the maximum position

  // Main simulation loop
  while (wb_robot_step(TIME_STEP) != -1) {
    // Update the ArmLowerR position for petting motion
    position += direction;

    // Reverse direction at the limits
    if (position >= maxArmLowerRPosition || position <= minArmLowerRPosition) {
      direction = -direction;
    }

    wb_motor_set_position(armLowerR, position);
  }

  wb_robot_cleanup();
  return 0;
}

/*************************** END OF MY CODE ***********************/ 