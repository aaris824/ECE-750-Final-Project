#include <webots/robot.h>
#include <webots/led.h>
#include <stdio.h>
#include <stdlib.h>

#define TIME_STEP 64

int main() {
  wb_robot_init();
  
  // Define LED device tags
  WbDeviceTag front_eye = wb_robot_get_device("front eye");
  WbDeviceTag led = wb_robot_get_device("led");
  WbDeviceTag left_eye = wb_robot_get_device("left eye");
  WbDeviceTag right_eye = wb_robot_get_device("right eye");
  
  // Turn LEDs on and off to create a flashing effect
  while (wb_robot_step(TIME_STEP) != -1) {
    static int counter = 0;
    
    // Create a slightly offset pattern for each LED
    int front_eye_value = (counter % 10 < 5) ? 0xFF0000 : 0; // Red for the first half, off for the second half of the cycle
    int led_value = ((counter + 2) % 10 < 5) ? 0xFF0000 : 0; // Starts 2 timesteps later
    int left_eye_value = ((counter + 4) % 10 < 5) ? 0xFF0000 : 0; // Starts 4 timesteps later
    int right_eye_value = ((counter + 6) % 10 < 5) ? 0xFF0000 : 0; // Starts 6 timesteps later

    // Set the LED colors
    wb_led_set(front_eye, front_eye_value);
    wb_led_set(led, led_value);
    wb_led_set(left_eye, left_eye_value);
    wb_led_set(right_eye, right_eye_value);
    
    counter++;
  }
  
  wb_robot_cleanup();
  return 0;
}
