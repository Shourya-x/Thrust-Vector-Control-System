#include <RotaryEncoder.h>
#include <stdlib.h>
#include <Servo.h>
#include <Wire.h>
#include <MPU6050.h>

int* ServoLimit = (int*) calloc(sizeof(int), 4);
int* ServoLimit_predefined = (int*) calloc(sizeof(int), 4);


// RotaryEncode pins
#define R1_CLK 7 
#define R1_DT 6
#define R1_SW 5
RotaryEncoder encoder(R1_CLK, R1_DT, RotaryEncoder::LatchMode::TWO03);

// Servo pinout
#define SERVO_S1 8
#define SERVO_S2 9

Servo servo1;
Servo servo2;

// joystick
#define JOY_SW1 4
#define JOY_X1 A0
#define JOY_Y1 A1


void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
  encoder.setPosition(0);
  servo1.attach(SERVO_S1);
  servo2.attach(SERVO_S2);

  //ServoLimit[0] = 180;
  //ServoLimit[2] = 180;
  ServoLimit_predefined[0]=135;
  ServoLimit_predefined[1]=46;
  ServoLimit_predefined[2]=184;
  ServoLimit_predefined[3]=5;
  pinMode(R1_SW, INPUT_PULLUP);
  servo1.write(90);
  servo2.write(90);

  pinMode(JOY_SW1, INPUT_PULLUP);
  pinMode(JOY_X1, INPUT);
  pinMode(JOY_Y1, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
     int x, y;
  int xVar, yVar;
  while(digitalRead(JOY_SW1) == HIGH){
    x = analogRead(JOY_X1);
    y = analogRead(JOY_Y1);
    xVar = map(x, 0, 1023, ServoLimit_predefined[1], ServoLimit_predefined[0]);
    yVar = map(y, 0, 1023, ServoLimit_predefined[3], ServoLimit_predefined[2]);
    servo1.write(xVar);
    servo2.write(yVar);
  }
  servo1.write(90);
  servo2.write(90);
}
