/**************************************************************************************************/
// File Name: Golf-BOT.ino                                                            
// Lab/Project: ENGR 3100 Final Project                                                          
// Current Semester:  Fall 2025                                                       
// Author(s):  Dylan Venditti                                                              
// Purpose of Program: Robotic Object-Following and Object-Striking using PixyCam and Servo motors                                                                                                   
/**************************************************************************************************/

#include <Pixy2.h> //PixyCam library
#include <Servo.h> //Servo library

// main Pixy object 
Pixy2 pixy;

Servo leftMotor, rightMotor, puttMotor;

const int rightPin = 7;
const int leftPin = 9;
const int puttPin = 10;

const int SIG_BALL = 1;     //Golf Ball set to SIGNATURE 1 on PixyCam (See PixyCam documentation)

float kp = 1.0;             //Proportional k value for P controller (Tunable)

const int CLOSE_WIDTH = 40; //Experimentally determined pixel width at which ball is "close"

void setup()
{
  Serial.begin(115200);

  pixy.init();

  leftMotor.attach(leftPin, 1000, 2000);
  rightMotor.attach(rightPin, 1000, 2000);
  puttMotor.attach(puttPin, 1000, 2000);
}

void loop()
{ 
    pixy.ccc.getBlocks();   // Get all detected objects

    float x = -1;           // Default: no ball found yet
    int ballWidth =-1;

    // Look for the SIGNATURE 1 among blocks
    for(int i = 0; i < pixy.ccc.numBlocks; i++) {
      if(pixy.ccc.blocks[i].m_signature == SIG_BALL) {
        x = pixy.ccc.blocks[i].m_x;   // Save the x-position of the ball
        ballWidth = pixy.ccc.blocks[i].m_width;
        Serial.println(x);
        break;
              // Stop searching
      }
    }
    // If ball wasn’t seen (x stayed -1), spin to search
    if(x == -1) {
      // Spin the robot to look for the ball
      Spin();
      return;
    }
  
    // drive forward while keeping the ball centered
  float current_x = x;
  int target_x = 158;


  float error = current_x - target_x;
  float u = kp * error;

int nudgeTime = 1120;

  if (ballWidth < CLOSE_WIDTH){
      yawTurn(u);
  }
  else{
      golfShot();
  }

} //Void loop end bracket

void yawTurn(float u) {

  // Servo PWM constants
  const int PWM_MIN  = 1100;
  const int PWM_MAX  = 1900;
  const int PWM_BASE_RIGHT = 1400;
  const int PWM_BASE_LEFT = 1600;
  //NOTE: In my configuration, the Left and Right servos were at forward max speed at 1900 and 1100, respectively.
  //I did not make seperate left max fwd and right max fwd to limit amount of redundant variables

  // Max |u| that gives full speed (tunable)
  const float U_MAX = 100.0f;   // chosen based on kp & expected error

  // Sign of u: -1, 0, +1   (Determines what side of vision field the ball is on)
  int s = (u > 0) - (u < 0);

  // Raw magnitude
  float mag_raw = fabs(u);

  // Clamp mag_raw to U_MAX without if/else:
  // min(a, b) = 0.5 * (a + b - |a - b|)
  float mag_clamped = 0.5f * (mag_raw + U_MAX - fabs(mag_raw - U_MAX));

  // Normalize to [0, 1]
  float mag = mag_clamped / U_MAX;

  // Convert magnitude to PWM delta from STOP to MAX
  int pwm_delta = (int)((300) * mag); //this is 300 because bases are 300 away from max values-- i.e. PWM_BASE_LEFT, 1600, is 300 away from PWM_MAX = 1900

  // Active motor depends on sign:
  // s > 0 (right turn) left motor = STOP + delta, right = STOP
  // s < 0 (left turn)  right motor = STOP - delta, left  = STOP
  int leftPWM  = PWM_BASE_LEFT + pwm_delta * (s >= 0);
  int rightPWM = PWM_BASE_RIGHT - pwm_delta * (s <= 0); //Note: minus sign here because right max fwd is 1100.

  leftMotor.writeMicroseconds(leftPWM);
  rightMotor.writeMicroseconds(rightPWM);
  //Ultimately, one wheel is turning faster in order to direct the robot toward the ball. Turning intensity is inversely-proportional to how centered the ball is.
}

void Spin(){
  rightMotor.writeMicroseconds(1200);
  leftMotor.writeMicroseconds(1500);
}

void stopMotors(){
  rightMotor.writeMicroseconds(1500);
  leftMotor.writeMicroseconds(1500);
}

void strikeBall(){
  // Wind back
  puttMotor.writeMicroseconds(1400);
  delay(1000);   // tune
  
  // Swing forward to hit
  puttMotor.writeMicroseconds(1900);
  rightMotor.writeMicroseconds(1400);
  leftMotor.writeMicroseconds(1600);
  delay(375);   // tune
  stopMotors();

  //Pause 
  puttMotor.writeMicroseconds(1500);
  delay(2000);
  
  // Return to neutral
  puttMotor.writeMicroseconds(1400);
  delay(510);

  puttMotor.writeMicroseconds(1500);
  delay(1000);
}

void nudge(){
  
  rightMotor.writeMicroseconds(1400);
  leftMotor.writeMicroseconds(1600);
  delay(1250);
}

void golfShot(){

    stopMotors();
    delay(250);
    nudge();
    stopMotors();
    delay(500);
    strikeBall();
    delay(3000);
}
