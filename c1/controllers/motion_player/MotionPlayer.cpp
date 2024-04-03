#include <webots/Robot.h>
#include <webots/Motor.h>
#include <cmath>
#include <iostream>

#define TIME_STEP 32

using namespace webots;
using namespace std;

class MotionPlayer : public Robot {
public:
  MotionPlayer();
  virtual ~MotionPlayer();
  void run();

private:
  Motor *headPitchMotor; // Head pitch motor for nodding
  void nodHead();
};

MotionPlayer::MotionPlayer() {
  // Assuming the motor's name is "HeadPitch". Please replace it with the correct name if different.
  headPitchMotor = getMotor("HeadPitch");
}

MotionPlayer::~MotionPlayer() {
}

void MotionPlayer::nodHead() {
  // Perform a simple nodding motion
  double amplitude = 0.3; // Adjust this value as needed
  double period = 2.0; // Adjust for the duration of one nod cycle
  for (int i = 0; i < 50; ++i) {
    double phase = (double)i / 50.0;
    double position = amplitude * sin(2.0 * M_PI * phase / period);
    headPitchMotor->setPosition(position);
    this->step(TIME_STEP);
  }
}

void MotionPlayer::run() {
  nodHead(); // Perform nodding at the start
  // Place your existing motion play logic here
}

int main(int argc, char **argv) {
  MotionPlayer *player = new MotionPlayer();
  player->run();
  delete player;
  return 0;
}
