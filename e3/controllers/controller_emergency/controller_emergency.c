#include <webots/robot.h>
#include <webots/led.h>
#include <stdlib.h>

#define TIME_STEP 64

void my_step() {
  if (wb_robot_step(TIME_STEP) == -1) {
    wb_robot_cleanup();
    exit(EXIT_SUCCESS);
  }
}

int main(int argc, char **argv) {
  wb_robot_init();
  
  int time_step = wb_robot_get_basic_time_step();
  
  // Getting device tags for the LEDs
  WbDeviceTag front_eye = wb_robot_get_device("front eye");
  WbDeviceTag led = wb_robot_get_device("led");
  WbDeviceTag left_eye = wb_robot_get_device("left eye");
  WbDeviceTag right_eye = wb_robot_get_device("right eye");
  
  // Check if we successfully got all LEDs
  if (front_eye == 0 || led == 0 || left_eye == 0 || right_eye == 0) {
    wb_robot_cleanup();
    return 0; // Exit if we cannot find the LEDs
  }
  
  // Blinking loop
  for (int i = 0; i < 140; ++i) { // Extended loop for continuous blinking
    // Turn the LEDs red with staggered timings for dynamic effect
    wb_led_set(front_eye, (i % 2) * 0xFF0000); // Red color, toggles every iteration
    wb_led_set(led, ((i + 1) % 4 < 2) * 0xFF0000); // Red color, toggles with a different phase
    wb_led_set(left_eye, ((i + 2) % 4 < 2) * 0xFF0000); // Red color, different phase
    wb_led_set(right_eye, ((i + 3) % 4 < 2) * 0xFF0000); // Red color, different phase
    
    my_step();
  }

  wb_robot_cleanup();
  return 0;
}
