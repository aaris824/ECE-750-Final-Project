/*************************** START OF MY CODE ***********************/ 

#include <webots/robot.h>
#include <webots/utils/motion.h> 


#include <stdio.h>
#include <stdlib.h>

#define TIME_STEP 64

static void play_motion(WbMotionRef motion) {
  if (motion) {
    wbu_motion_play(motion);
    // Wait for the motion to finish
    while (!wbu_motion_is_over(motion))
      wb_robot_step(TIME_STEP);
  }
}

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

  int time_step = wb_robot_get_basic_time_step();

  // Calculate the number of steps to wait
  int wait_steps = (int)(2000 / time_step); // Wait for 5 seconds

  set_natural_arm_position();

  // Wait loop
  for (int i = 0; i < wait_steps; i++) {
    wb_robot_step(time_step);
  }

  // Load motions
  WbMotionRef turnLeft180 = wbu_motion_new("../../motions/TurnLeft180.motion");
  WbMotionRef forwards = wbu_motion_new("../../motions/Forwards50.motion");

  // Perform a 60-degree turn to the left
  play_motion(turnLeft180);
  
  // Start walking forward after the turn
  play_motion(forwards);

  // Cleanup motions when done
  wbu_motion_delete(turnLeft180);
  wbu_motion_delete(forwards);

  // Cleanup code
  wb_robot_cleanup();
  
  return 0;
}

/*************************** END OF MY CODE ***********************/ 