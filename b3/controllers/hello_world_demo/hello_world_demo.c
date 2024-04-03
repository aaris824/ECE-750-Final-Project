/*************************** START OF MY CODE ***********************/ 

#include <webots/robot.h>
#include <webots/motor.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TIME_STEP 16

void my_step() {
  if (wb_robot_step(TIME_STEP) == -1) {
    wb_robot_cleanup();
    exit(EXIT_SUCCESS);
  }
}

int main(int argc, char **argv) {
  wb_robot_init();

  // Motor devices
  WbDeviceTag l_arm_shx = wb_robot_get_device("LArmShx");
  WbDeviceTag r_arm_shx = wb_robot_get_device("RArmShx");
  WbDeviceTag r_arm_elx = wb_robot_get_device("RArmElx");
  WbDeviceTag back_lbz = wb_robot_get_device("BackLbz");
  WbDeviceTag back_mby = wb_robot_get_device("BackMby");
  WbDeviceTag back_ubx = wb_robot_get_device("BackUbx");
  

  // Position arms by the side
  wb_motor_set_position(l_arm_shx, -1.2); 
  wb_motor_set_position(r_arm_shx, 1.2); 


  // Main simulation loop
  double initTime = wb_robot_get_time();
  while (true) {
    double time = wb_robot_get_time() - initTime;

    

    // Torso movements
   wb_motor_set_position(back_lbz, 0.6 * sin(2 * time));  // Bend lower back with more pronounced movement


    my_step();
  };

  wb_robot_cleanup();
  return EXIT_SUCCESS;
}


/*************************** END OF MY CODE ***********************/ 