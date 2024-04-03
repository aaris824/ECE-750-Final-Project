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

  // Initial targets for arm movement
  double l_arm_shx_target = -1.396;
  double r_arm_shx_target = -0.77;

  // Move arms to initial position
  int n_steps_to_achieve_target = 1000 / TIME_STEP;  // 1 second
  for (int i = 0; i < n_steps_to_achieve_target; i++) {
    double ratio = (double)i / n_steps_to_achieve_target;
    wb_motor_set_position(l_arm_shx, l_arm_shx_target * ratio);
    wb_motor_set_position(r_arm_shx, r_arm_shx_target * ratio);
    my_step();
  }

  // Main simulation loop
  double initTime = wb_robot_get_time();
  while (true) {
    double time = wb_robot_get_time() - initTime;

    // Arm movements
    wb_motor_set_position(r_arm_elx, 0.3 * sin(5 * time) - 0.3);

    // Torso movements
    wb_motor_set_position(back_lbz, 0.1 * sin(2 * time));  // Bend lower back
    wb_motor_set_position(back_mby, 0.1 * sin(2 * time + 1));  // Bend middle back
    wb_motor_set_position(back_ubx, 0.1 * cos(2 * time));  // Twist upper back

    my_step();
  };

  wb_robot_cleanup();
  return EXIT_SUCCESS;  // This line is never reached
}

/*************************** END OF MY CODE ***********************/ 
