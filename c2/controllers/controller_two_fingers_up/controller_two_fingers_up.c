/*************************** START OF MY CODE ***********************/ 

#include <webots/robot.h>
#include <webots/motor.h>
#include <stdio.h>

#define TIME_STEP 64


static void set_natural_arm_position() {
  // Shoulder pitch adjustment
  wb_motor_set_position(wb_robot_get_device("RShoulderPitch"), 1.5); 
  wb_motor_set_position(wb_robot_get_device("LShoulderPitch"), 1.5); 
  
  // Shoulder roll adjustment, outward
  wb_motor_set_position(wb_robot_get_device("RShoulderRoll"), -0.2); 
  wb_motor_set_position(wb_robot_get_device("LShoulderRoll"), 0.2); 


}


int main(int argc, char **argv) {
  wb_robot_init();

  // Get the motor devices
  WbDeviceTag lShoulderPitch = wb_robot_get_device("LShoulderPitch");
  WbDeviceTag lShoulderRoll = wb_robot_get_device("LShoulderRoll");
  WbDeviceTag lElbowYaw = wb_robot_get_device("LElbowYaw");
  WbDeviceTag lElbowRoll = wb_robot_get_device("LElbowRoll");
  WbDeviceTag lPhalanx[6];
  char phalanxNames[6][16] = {"LPhalanx1", "LPhalanx2", "LPhalanx3", "LPhalanx4", "LPhalanx5", "LPhalanx6"};
  for (int i = 0; i < 6; i++) {
    lPhalanx[i] = wb_robot_get_device(phalanxNames[i]);
  }

  // Gradually move the arm and fingers
  double targetPositions[6] = {0, -1.54, -1.9, 1, 1, 1.33}; // Target positions for the motors
  double initialPositions[6] = {0}; // Initial positions for the motors
  double step = 0.01; // Step size
  
  set_natural_arm_position();

  while (wb_robot_step(TIME_STEP) != -1) {
    for (double pos = 0; pos <= 1; pos += step) {
      wb_motor_set_position(lShoulderPitch, pos * targetPositions[0]);
      wb_motor_set_position(lElbowRoll, pos * targetPositions[1]);
      wb_motor_set_position(lElbowYaw, pos * targetPositions[2]);
      for (int i = 0; i < 6; i++) {
        wb_motor_set_position(lPhalanx[i], pos * targetPositions[3]); // All phalanx motors to the same position
      }
      wb_motor_set_position(lShoulderRoll, pos * targetPositions[4]);
      wb_robot_step(TIME_STEP); 
    }
    break; // Exit after reaching the target positions
  }

  wb_robot_cleanup();
  return 0;
}

/*************************** END OF MY CODE ***********************/ 