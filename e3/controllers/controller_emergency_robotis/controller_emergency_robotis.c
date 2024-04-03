/*************************** START OF MY CODE ***********************/ 

#include <webots/robot.h>
#include <webots/led.h>
#include <webots/motor.h>
#include <stdlib.h>
#include <math.h>

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
  WbDeviceTag neck_motor = wb_robot_get_device("Neck");
  
  if (eye_led == 0 || head_led == 0 || neck_motor == 0) {
    wb_robot_cleanup();
    return 0; // Exit if we cannot find the devices
  }
  
  // Blinking and "No" gesture loop for an extended period
  for (int i = 0; i < 56; ++i) {
    double neck_position = sin((double)i / 28 * 2 * M_PI) * 0.9; // Sine wave pattern for neck movement
    wb_motor_set_position(neck_motor, neck_position); // Set neck position
    
    if (i % 2 == 0) {
      wb_led_set(eye_led, 0xFF0000); // Red color for EyeLed, on even cycles
      wb_led_set(head_led, 0); // HeadLed off
    } else {
      wb_led_set(eye_led, 0); // EyeLed off
      wb_led_set(head_led, 0xFF0000); // Red color for HeadLed, on odd cycles
    }
    
    my_step();
    wb_robot_step(time_step * 10); // Each cycle lasts for 0.64 seconds
  }
  
  // Turn both LEDs off after the loop
  wb_led_set(eye_led, 0);
  wb_led_set(head_led, 0);

  wb_robot_cleanup();
  return 0;
}

/*************************** END OF MY CODE ***********************/ 