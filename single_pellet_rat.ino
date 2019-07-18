/*
    This program is a server for training rats.

    Author: Julian Pitney, Junzheng Wu
    Email: JulianPitney@gmail.com, jwu220@uottawa.ca
    Organization: University of Ottawa (Silasi Lab)

*/

#include <Servo.h>

// Config
const int servo1Pin = 4;
Servo servo1;

bool servo1_up_flag = false;
const int SERVO_SETTLE_DELAY = 3500;
// Lower numbers make the arm go higher
int SERVO1_UP_POS = 32;
// Higher numbers makehe arm go lower
int SERVO1_DOWN_POS = 100;
int SERVO_PULSE_DELAY = 16;
int servo1Pos = SERVO1_DOWN_POS;

const int ledPin = 13;
const int switchPin = 2;

boolean current_flag = false;
volatile byte working_flag = false;

void handleSwitchChange() {
  working_flag = current_flag;
  digitalWrite(ledPin, working_flag);
}

int zeroServos() {

  servo1.attach(servo1Pin);

  for (int i = servo1Pos; i <= SERVO1_DOWN_POS; i += 1) {
      servo1.write(i);
      delay(SERVO_PULSE_DELAY);
    }   
  
  servo1Pos = SERVO1_DOWN_POS;
  
  servo1.detach();
  delay(SERVO_SETTLE_DELAY);
  return 0;

}

int lowerServo1(){
 
  servo1.attach(servo1Pin);
   
  for (int i = servo1Pos; i <= SERVO1_DOWN_POS; i += 1) {
      servo1.write(i);
      delay(SERVO_PULSE_DELAY);
    }   
  delay(SERVO_SETTLE_DELAY);
  servo1Pos = SERVO1_DOWN_POS;
  
  servo1.detach();
  return 0;
}

int displayPellet() {
    
  servo1.attach(servo1Pin);

  // Lower arm to grab pellet.
  for (int i = servo1Pos; i <= SERVO1_DOWN_POS; i += 1) {
    servo1.write(i);
    delay(SERVO_PULSE_DELAY);
  }   

  // Raise arm to display pellet
  for (int i = SERVO1_DOWN_POS; i >= SERVO1_UP_POS; i -= 1) {
    servo1.write(i);
    delay(SERVO_PULSE_DELAY);
  }
  
  servo1Pos = SERVO1_UP_POS;
  servo1_up_flag = true;
  servo1.detach();
  delay(SERVO_SETTLE_DELAY);
  return 0;
}


void setup() {
  zeroServos();
  pinMode(switchPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(switchPin), handleSwitchChange, HIGH);

  // Set LED control pin
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.write("READY\n");
}


void test_switch(){
  while(!digitalRead(switchPin)){
    digitalWrite(ledPin, HIGH);
  }
  digitalWrite(ledPin, LOW);
}


void loop(){
  
  if(working_flag){ displayPellet(); current_flag = false;}
  else{ current_flag = true;}
  delay(1500);
}
