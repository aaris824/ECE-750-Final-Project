/*************************** START OF MY CODE ***********************/ 

#include <webots/robot.h>
#include <webots/utils/motion.h> // Correct include for wbu_motion functions

// Include necessary headers
#include <stdio.h>
#include <stdlib.h>

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

  // Your existing setup code here
  int time_step = wb_robot_get_basic_time_step();

  // Calculate the number of steps to wait
  int wait_steps = (int)(10000 / time_step); // 10 seconds delay
  
  set_natural_arm_position();

  // Wait loop
  for (int i = 0; i < wait_steps; i++) {
    wb_robot_step(time_step);
  }

  // After 10 seconds, start walking
  WbMotionRef forwards = wbu_motion_new("../../motions/Forwards50.motion");

  if (forwards) {
    wbu_motion_play(forwards);
    
    // Loop to continue the simulation after starting to walk
    while (wb_robot_step(time_step) != -1) {
      // Simulation continues running with NAO walking
    }
    
    // Cleanup the motion when done
    wbu_motion_delete(forwards);
  }

  // Cleanup code
  wb_robot_cleanup();
  
  return 0;
}

/*************************** END OF MY CODE ***********************/ 