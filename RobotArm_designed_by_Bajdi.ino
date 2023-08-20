/*
Copyright (c) 2023 Inukai
Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <Stepper.h>
#include <Servo.h>
#define STEPS 2048

// please attach the pins to your circuit
#define Stepper1_x    2
#define Stepper1_y    3
#define Stepper1_xber 6
#define Stepper1_yber 7
#define Stepper2_x    A1
#define Stepper2_y    A0
#define Stepper2_xber A3
#define Stepper2_yber A2
#define Stepper3_x    10
#define Stepper3_y    12
#define Stepper3_xber 9
#define Stepper3_yber 8
#define ServoMotor    13
#define Limit1        11
#define Limit2        5
#define Limit3        4
#define LeftPotentiometer  A4
#define RightPotentiometer A5

Stepper stepper1(STEPS, Stepper1_x, Stepper1_xber, Stepper1_y, Stepper1_yber);
Stepper stepper2(STEPS, Stepper2_x, Stepper2_xber, Stepper2_y, Stepper2_yber);
Stepper stepper3(STEPS, Stepper3_x, Stepper3_xber, Stepper3_y, Stepper3_yber);
Servo sv;

int stepper1_old_val = 0;
int stepper2_old_val = 0;
int stepper3_old_val = 0;

bool sv_state = false;

void setup() {
  // please adjust the serial communication speed to your liking.
  Serial.begin(9600);

  sv.attach(ServoMotor);

  // Since the torque of 28-BYJ-48 stepper is weak, the speed should be slow.
  stepper1.setSpeed(5);
  stepper2.setSpeed(5);
  stepper3.setSpeed(5);

  // I recommend using limit switches, but they are unnecessary if you don't have them installed.
  pinMode(Limit1, INPUT_PULLUP);
  pinMode(Limit2, INPUT_PULLUP);
  pinMode(Limit3, INPUT_PULLUP);

  // Connect to the controller
  pinMode(LeftPotentiometer, INPUT);
  pinMode(RightPotentiometer, INPUT);

  // Please adjust the initial position to your servo motor.
  sv.write(87);

  // I use limit switches to initialize the stepper motor positions. 
  // If you are not installing limit switches, remove this code.
  // I think this initialization needs to be adjusted according to your robot arm.
  //initialize stepper motor 1
  for(int i = 0; i<25; i++){
    if(!digitalRead(Limit1)){
      stepper_off();
      break;
    }
    stepper1.step(-50);
    if(!digitalRead(Limit2)){
      stepper2.step(200);
    }
  }
  delay(500);
  
  stepper1.step(100);
  stepper1_old_val = 10;
  stepper_off();

  delay(1000);

  //initialize stepper motor 2
  for(int i = 0; i<25; i++){
    if(!digitalRead(Limit2)){
      break;
    }
    stepper2.step(-50);
  }
  stepper_off();
  stepper2_old_val = 0;
  delay(1000);

  //initialize stepper motor 3
  for(int i = 0; i<25; i++){
    if(!digitalRead(Limit3)){
      break;
    }
    stepper3.step(-50);
  }
  stepper3.step(500);
  stepper3_old_val = 500;
  stepper_off();
}

void loop() {
  int stepper1_val = 0;
  int stepper2_val = 0;
  int stepper3_val = 0;
  int val;
  int x; 
  int y;
  int buf;

  buf = 0;
  for(int i = 0; i < 10; i++){
    buf += analogRead(RightPotentiometer); 
  }
  x = buf / 10;
  buf = 0;
  for(int i = 0; i < 10; i++){
    buf += analogRead(LeftPotentiometer); 
  }
  y = buf / 10; 
  
  Serial.println(x);
  Serial.println(y);

  if(x > 247 && x < 493){
    stepper1_val = -1.6194 * x + 800;
    if(stepper1_val < 0) stepper1_val = 0;
    val =  stepper1_val - stepper1_old_val; 
    stepper1_old_val = stepper1_val; 
    if(val > 2 || val < -2) stepper1.step(val);
    stepper_off();
  }
  
  if(y > 247 && y < 493){
    stepper2_val = -1.6194 * y + 800;
    if(stepper2_val < 0) stepper2_val = 0;
    val =  stepper2_val - stepper2_old_val; 
    stepper2_old_val = stepper2_val; 
    if(val > 2 || val < -2) stepper2.step(val);
  }

  if(x > 1000 && y < 800 && digitalRead(Limit3)){
    stepper3.step(-100);
    stepper3_old_val -= 100;
  }
  else if(y > 1000 && x < 800 && stepper3_old_val < 1000){
    stepper3.step(100);
    stepper3_old_val += 100;
  }

  if(x > 1000 && y > 1000){
    if(sv_state){
      sv.write(87);
      sv_state = false;
    }
    else{
      sv.write(62);
      sv_state = true;
    }
  }

  // Check limit switches
  if(!digitalRead(Limit1)){
    stepper1_old_val = 0;
  }
  if(!digitalRead(Limit2)){
    stepper2_old_val = 0;
  }
  if(!digitalRead(Limit3)){
    stepper3_old_val = 0;
  }

  // Display status on serial monitor
  Serial.print("Limit Switch: ");
  Serial.print(digitalRead(Limit1));
  Serial.print(", ");
  Serial.print(digitalRead(Limit2));
  Serial.print(", ");
  Serial.println(digitalRead(Limit3));

  Serial.print("Stepper position: ");
  Serial.print(stepper1_old_val);
  Serial.print(", ");
  Serial.print(stepper2_old_val);
  Serial.print(", ");
  Serial.println(stepper3_old_val);

  delay(500);
}

// I don't like stepper motor heat. However, this code causes the motor 
// to lose its holding power when stopped. It is up to you whether 
// or not to supply power to the motor when stopped.
void stepper_off(){
  digitalWrite(Stepper1_x,0);
  digitalWrite(Stepper1_xber,0);
  digitalWrite(Stepper1_y,0);
  digitalWrite(Stepper1_yber,0);
  
  digitalWrite(Stepper2_x,0);
  digitalWrite(Stepper2_xber,0);
  digitalWrite(Stepper2_y,0);
  digitalWrite(Stepper2_yber,0);
      
  digitalWrite(Stepper3_x,0);
  digitalWrite(Stepper3_xber,0);
  digitalWrite(Stepper3_y,0);
  digitalWrite(Stepper3_yber,0);       
}