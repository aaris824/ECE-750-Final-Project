/*
 * Copyright 1996-2023 Cyberbotics Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Description:   Simple controller to present the Spot robot.
 */

#include <webots/camera.h>
#include <webots/device.h>
#include <webots/led.h>
#include <webots/motor.h>
#include <webots/robot.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_LEDS 8
#define NUMBER_OF_JOINTS 12
#define NUMBER_OF_CAMERAS 5

// Initialize the robot's information
static WbDeviceTag motors[NUMBER_OF_JOINTS];
static const char *motor_names[NUMBER_OF_JOINTS] = {
  "front left shoulder abduction motor",  "front left shoulder rotation motor",  "front left elbow motor",
  "front right shoulder abduction motor", "front right shoulder rotation motor", "front right elbow motor",
  "rear left shoulder abduction motor",   "rear left shoulder rotation motor",   "rear left elbow motor",
  "rear right shoulder abduction motor",  "rear right shoulder rotation motor",  "rear right elbow motor"};
static WbDeviceTag cameras[NUMBER_OF_CAMERAS];
static const char *camera_names[NUMBER_OF_CAMERAS] = {"left head camera", "right head camera", "left flank camera",
                                                      "right flank camera", "rear camera"};
static WbDeviceTag leds[NUMBER_OF_LEDS];
static const char *led_names[NUMBER_OF_LEDS] = {"left top led",          "left middle up led", "left middle down led",
                                                "left bottom led",       "right top led",      "right middle up led",
                                                "right middle down led", "right bottom led"};

static void step() {
  const double time_step = wb_robot_get_basic_time_step();
  if (wb_robot_step(time_step) == -1) {
    wb_robot_cleanup();
    exit(0);
  }
}

// Movement decomposition
static void movement_decomposition(const double *target, double duration) {
  const double time_step = wb_robot_get_basic_time_step();
  const int n_steps_to_achieve_target = duration * 1000 / time_step;
  double step_difference[NUMBER_OF_JOINTS];
  double current_position[NUMBER_OF_JOINTS];

  for (int i = 0; i < NUMBER_OF_JOINTS; ++i) {
    current_position[i] = wb_motor_get_target_position(motors[i]);
    step_difference[i] = (target[i] - current_position[i]) / n_steps_to_achieve_target;
  }

  for (int i = 0; i < n_steps_to_achieve_target; ++i) {
    for (int j = 0; j < NUMBER_OF_JOINTS; ++j) {
      current_position[j] += step_difference[j];
      wb_motor_set_position(motors[j], current_position[j]);
    }
    step();
  }
}

static void lie_down(double duration) {
  const double motors_target_pos[NUMBER_OF_JOINTS] = {-0.40, -0.99, 1.59,   // Front left leg
                                                      0.40,  -0.99, 1.59,   // Front right leg
                                                      -0.40, -0.99, 1.59,   // Rear left leg
                                                      0.40,  -0.99, 1.59};  // Rear right leg
  movement_decomposition(motors_target_pos, duration);
}

static void stand_up(double duration) {
  const double motors_target_pos[NUMBER_OF_JOINTS] = {-0.1, 0.0, 0.0,   // Front left leg
                                                      0.1,  0.0, 0.0,   // Front right leg
                                                      -0.1, 0.0, 0.0,   // Rear left leg
                                                      0.1,  0.0, 0.0};  // Rear right leg

  movement_decomposition(motors_target_pos, duration);
}

static void sit_down(double duration) {
  const double motors_target_pos[NUMBER_OF_JOINTS] = {-0.20, -0.40, -0.19,  // Front left leg
                                                      0.20,  -0.40, -0.19,  // Front right leg
                                                      -0.40, -0.90, 1.18,   // Rear left leg
                                                      0.40,  -0.90, 1.18};  // Rear right leg

  movement_decomposition(motors_target_pos, duration);
}

static void give_paw() {
  // Stabilize posture
  const double motors_target_pos_1[NUMBER_OF_JOINTS] = {-0.20, -0.30, 0.05,   // Front left leg
                                                        0.20,  -0.40, -0.19,  // Front right leg
                                                        -0.40, -0.90, 1.18,   // Rear left leg
                                                        0.49,  -0.90, 0.80};  // Rear right leg

  movement_decomposition(motors_target_pos_1, 4);

  const double initial_time = wb_robot_get_time();
  while (wb_robot_get_time() - initial_time < 8) {
    wb_motor_set_position(motors[4], 0.2 * sin(2 * wb_robot_get_time()) + 0.6);  // Upperarm movement
    wb_motor_set_position(motors[5], 0.4 * sin(2 * wb_robot_get_time()));        // Forearm movement
    step();
  }
  // Get back in sitting posture
  const double motors_target_pos_2[NUMBER_OF_JOINTS] = {-0.20, -0.40, -0.19,  // Front left leg
                                                        0.20,  -0.40, -0.19,  // Front right leg
                                                        -0.40, -0.90, 1.18,   // Rear left leg
                                                        0.40,  -0.90, 1.18};  // Rear right leg

  movement_decomposition(motors_target_pos_2, 4);
}

/*************************** START OF MY CODE ***********************/ 

static void walk_forward(double duration) {
  const double step_duration = duration / 8; // More phases for finer control
  
  // Step 1: Lift front left and rear right legs slightly and move them forward
  double target_positions_1[NUMBER_OF_JOINTS] = {
    0.1, -0.2, 0.1,  // Lift and move front left leg forward
    0.0, 0.0, 0.0,   // Front right leg stays
    0.0, 0.0, 0.0,   // Rear left leg stays
    0.1, -0.2, 0.1}; // Lift and move rear right leg forward
  movement_decomposition(target_positions_1, step_duration);

  // Step 2: Place front left and rear right legs down, lifting front right and rear left legs slightly and moving them forward
  double target_positions_2[NUMBER_OF_JOINTS] = {
    -0.1, 0.0, 0.0,  // Return front left leg
    0.1, -0.2, 0.1,  // Lift and move front right leg forward
    0.1, -0.2, 0.1,  // Lift and move rear left leg forward
    -0.1, 0.0, 0.0}; // Return rear right leg
  movement_decomposition(target_positions_2, step_duration);
  
}

static void walk_forward_2(double duration) {
  const int steps = duration / 0.5; // Assuming each step cycle (4 phases) takes about 0.5 seconds
  
  for (int step = 0; step < steps; ++step) {
    // Phases are expanded to repeat the cycle, making walking continuous
    // Each call to movement_decomposition now represents a quarter of a step cycle

    // Phase 1: Lift front left and rear right legs slightly and move them forward
    double target_positions_1[NUMBER_OF_JOINTS] = {
      0.1, -0.2, 0.1,  // Lift and move front left leg forward
      0.0, 0.0, 0.0,   // Front right leg stays
      0.0, 0.0, 0.0,   // Rear left leg stays
      0.1, -0.2, 0.1}; // Lift and move rear right leg forward
    movement_decomposition(target_positions_1, 0.125 * duration);

    // Phase 2: Place front left and rear right legs down, lifting front right and rear left legs slightly and moving them forward
    double target_positions_2[NUMBER_OF_JOINTS] = {
      -0.1, 0.0, 0.0,  // Return front left leg
      0.1, -0.2, 0.1,  // Lift and move front right leg forward
      0.1, -0.2, 0.1,  // Lift and move rear left leg forward
      -0.1, 0.0, 0.0}; // Return rear right leg
    movement_decomposition(target_positions_2, 0.125 * duration);

  }
}


// Some parts of this main function were from the demo, such as get code to get cameras
int main(int argc, char **argv) {
  wb_robot_init();

  const double time_step = wb_robot_get_basic_time_step();

  // Get cameras
  for (int i = 0; i < NUMBER_OF_CAMERAS; ++i)
    cameras[i] = wb_robot_get_device(camera_names[i]);

  // enable the two front cameras
  wb_camera_enable(cameras[0], 2 * time_step);
  wb_camera_enable(cameras[1], 2 * time_step);

  // Get the LEDs and turn them on
  for (int i = 0; i < NUMBER_OF_LEDS; ++i) {
    leds[i] = wb_robot_get_device(led_names[i]);
    wb_led_set(leds[i], 1);
  }

  // Get the motors (joints) and set initial target position to 0
  for (int i = 0; i < NUMBER_OF_JOINTS; ++i)
    motors[i] = wb_robot_get_device(motor_names[i]);

  while (true) {
    walk_forward(4.0); // Example: walk forward for 4 seconds
  }

  wb_robot_cleanup();
  return EXIT_FAILURE;
}

/*************************** END OF MY CODE ***********************/ 