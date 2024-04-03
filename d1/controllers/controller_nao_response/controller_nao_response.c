#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/keyboard.h>
#include <webots/utils/motion.h>

#define TIME_STEP 32

int main(int argc, char **argv) {
  wb_robot_init();

  // Find and enable devices, assume this function is defined elsewhere
  // find_and_enable_devices();

  // Initialize motion
  WbMotionRef walk_motion = wbu_motion_new("motions/Walk.motion");

  int wait_time = 10000; // 10 seconds in milliseconds
  int elapsed_time = 0;

  // Wait for 10 seconds before walking
  while (elapsed_time < wait_time) {
    wb_robot_step(TIME_STEP);
    elapsed_time += TIME_STEP;
  }

  // Play walk motion after waiting
  if (walk_motion) {
    wbu_motion_play(walk_motion);

    // Continue simulation until motion is over
    while (!wbu_motion_is_over(walk_motion)) {
      wb_robot_step(TIME_STEP);
    }
  }

  // Cleanup and exit
  wbu_motion_delete(walk_motion);
  wb_robot_cleanup();
  return 0;
}
