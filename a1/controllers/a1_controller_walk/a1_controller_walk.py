"""a1_controller_walk controller."""

# You may need to import some classes of the controller module. Ex:
#  from controller import Robot, Motor, DistanceSensor
from controller import Robot

# create the Robot instance.
robot = Robot()

# get the time step of the current world.
timestep = int(robot.getBasicTimeStep())

# You should insert a getDevice-like function in order to get the
# instance of a device of the robot. Something like:
#  motor = robot.getDevice('motorname')
#  ds = robot.getDevice('dsname')
#  ds.enable(timestep)


# Setup leg motors
right_foreleg_J1 = robot.getDevice('PRM:/r4/c1-Joint2:41')
left_foreleg_J1 = robot.getDevice('PRM:/r2/c1-Joint2:21')
right_hindleg_J1 = robot.getDevice('PRM:/r5/c1-Joint2:51')
left_hindleg_J1 = robot.getDevice('PRM:/r3/c1-Joint2:31')

# Set initial position for all motors to ensure starting from a neutral stance
initial_position = 0.0  # Adjust based on your robot's neutral standing position
motors = [right_foreleg_J1, left_foreleg_J1, right_hindleg_J1, left_hindleg_J1]
for motor in motors:
    motor.setPosition(initial_position)

# Variables for the walking loop
step_count = 0
max_steps = 10
step_phase = 0  # 0: moving to forward position, 1: returning to initial position


# Main loop:
# - perform simulation steps until Webots is stopping the controller
while robot.step(timestep) != -1:
    # Read the sensors:
    # Enter here functions to read sensor data, like:
    #  val = ds.getValue()

    # Process sensor data here.

    # Enter here functions to send actuator commands, like:
    #  motor.setPosition(10.0)
    # Example adjustment to increase range of motion
    if step_phase == 0:
        right_foreleg_J1.setPosition(1.0)  # Increase from 0.5
        left_foreleg_J1.setPosition(-1.0)  # Increase from -0.5
        right_hindleg_J1.setPosition(1.0)
        left_hindleg_J1.setPosition(-1.0)
        step_phase = 1
    
    # Add a delay between steps for visual clarity and to simulate time taken for a step
    robot.step(timestep * 10)

# Enter here exit cleanup code.
