/*************************** START OF MY CODE ***********************/ 

#include <webots/robot.h>
#include <webots/motor.h>
#include <math.h>

#define TIME_STEP 64

int main(int argc, char **argv) {
  wb_robot_init(); // Necessary to initialize webots stuff

  // Getting the head motor
  WbDeviceTag headMotor = wb_robot_get_device("Head");

  // Ensure the motor is available
  if (headMotor == 0) {
    printf("Could not find Head motor.\n");
    return 1; // Exit if the motor is not found
  }

  double time = 0; // A variable to keep track of the simulation time

  // Main loop
  while (wb_robot_step(TIME_STEP) != -1) {
    // Calculate a new position for the head nodding motion
    // The position oscillates between -0.36 and 0.94
    double position = 0.3u * sin(0.5 * time); // Adjust amplitude and frequency
    // Set the new position
    wb_motor_set_position(headMotor, position);

    time += TIME_STEP; // Increment time
  };

  wb_robot_cleanup(); // Cleanup webots resources

  return 0; // Exit cleanly
}

/*************************** END OF MY CODE ***********************/ 
