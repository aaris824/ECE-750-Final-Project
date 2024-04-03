/*************************** START OF MY CODE ***********************/ 

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
  
  WbDeviceTag eye_led = wb_robot_get_device("EyeLed");
  WbDeviceTag head_led = wb_robot_get_device("HeadLed");
  
  if (eye_led == 0 || head_led == 0) {
    wb_robot_cleanup();
    return 0; // Exit if we cannot find the LEDs
  }
  
  // Blinking loop
  for (int i = 0; i < 7; ++i) {
    // Turn the LEDs green
    wb_led_set(eye_led, 0x00FF00); // Green color
    wb_led_set(head_led, 0x00FF00); // Green color
    my_step();
    wb_robot_step(time_step * 10); // on for 0.64 seconds

    // Turn the LEDs off
    wb_led_set(eye_led, 0);
    wb_led_set(head_led, 0);
    my_step();
    wb_robot_step(time_step * 10); // off for 0.64 seconds
  }
  
  // Keep the LEDs on green for 3 seconds
  wb_led_set(eye_led, 0x00FF00); // Green color
  wb_led_set(head_led, 0x00FF00); // Green color
  for (int i = 0; i < (3000 / TIME_STEP); ++i) {
    my_step();
  }

  wb_robot_cleanup();
  return 0;
}

/*************************** END OF MY CODE ***********************/ 
