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
  
  // Get device tags for motors and LEDs
  WbDeviceTag eye_led = wb_robot_get_device("EyeLed");
  WbDeviceTag left_shoulder = wb_robot_get_device("ShoulderL");
  WbDeviceTag right_shoulder = wb_robot_get_device("ShoulderR");
  WbDeviceTag left_upper_arm = wb_robot_get_device("ArmUpperL");
  WbDeviceTag right_upper_arm = wb_robot_get_device("ArmUpperR");
  WbDeviceTag left_lower_arm = wb_robot_get_device("ArmLowerL");
  WbDeviceTag right_lower_arm = wb_robot_get_device("ArmLowerR");

  // Initial setup for shoulder and upper arm positions
  wb_motor_set_position(left_shoulder, -2);
  wb_motor_set_position(right_shoulder, 2);
  wb_motor_set_position(left_upper_arm, -0.8);
  wb_motor_set_position(right_upper_arm, 0.8);
  
  WbDeviceTag head_motor = wb_robot_get_device("Head");
  wb_motor_set_position(head_motor, 0.5); // Tilt head up slightly
  

  // Loop for flashing LEDs and waving
  for (int i = 0; i < 84; ++i) {
    double phase = (double)i / 28 * M_PI; // Phase for the sine wave of the arm motion
    double left_lower_position = sin(phase) * 0.8;
    double right_lower_position = -sin(phase) * 0.8;
    wb_motor_set_position(left_lower_arm, left_lower_position);
    wb_motor_set_position(right_lower_arm, right_lower_position);

    // Set LEDs to blue alternatively
    int led_color = (i % 2 == 0) ? 0x0000FF : 0; // Blue color
    wb_led_set(eye_led, led_color);

    my_step();
    wb_robot_step(time_step * 5); // Adjust the cycle time if needed
  }

  wb_robot_cleanup();
  return 0;
}

/*************************** END OF MY CODE ***********************/ 
