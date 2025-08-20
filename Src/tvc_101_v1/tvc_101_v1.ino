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

// Accelerometer and gyro Address 0X68
MPU6050 mpu(0x69);

// TO Keep in mind: IMPORTENT!!!
// Set the serial moniter option to "No line ending" and "9600 baud"


void setServoLimits();
void testWithServo();
void clearAllConfig();
void servo_predefined_limits();



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  encoder.setPosition(0);
  servo1.attach(SERVO_S1);
  servo2.attach(SERVO_S2);

  ServoLimit[0] = 180;
  ServoLimit[2] = 180;
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
  
  // Wire.begin();
  // delay(200);
  // mpu.initialize();
  // delay(200);
  // Serial.println(mpu.testConnection());
  // if(mpu.testConnection()){
  //   Serial.println("✅ Connected successfully ");

  // }
  // else{
  //   Serial.println("❌ Connection failed");
  //   while(1);
  // }


}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("\n\n\n*************************************");
  Serial.println("Please enter what you want to do: ");
  Serial.println("1. Set servo limits");
  Serial.println("2. Test with joystick");
  Serial.println("3. Gyro");
  Serial.println("0. Clear all config");
  Serial.println("Enter your choice: ");
  char ch;

  while(true){
    if(Serial.available()){
      delay(100);
      input:
      if(Serial.available() > 1){
        while(Serial.available() != 0){
          Serial.read();
        }
        Serial.println("Please enter a valid input (1/2/3/4)");
      }
      if (Serial.available() == 1){
        ch = (char) Serial.read(); 
        break;
      }
    }    
  }
  Serial.print(" ::: ");
  Serial.print(ch);
  Serial.println(" ::: ");
  

  switch(ch){
    case '1':
      setServoLimits();
      Serial.println("Hello!");
      break;

    case '2':
      testWithServo();  
      break;

    case '3':
      int16_t gx, gy, gz;
      mpu.getRotation(gx, gy, gz);
      Serial.println(String(gx) + ", " + String(gy) + ", " + String(gz));
      delay(50);
      break;

    case '4':
      servo_predefined_limits();
      break;

    case '0':
      clearAllConfig();
      break;

    default:
      Serial.println("Please enter a valid input");
      goto input;
  }


  // ServoLimits(encoder);
  Serial.println("Code ended Successfullt!");
}

void clearAllConfig(){
  // servo config clearning
  ServoLimit[0] = 180;
  ServoLimit[1] = 0;
  ServoLimit[2] = 180;
  ServoLimit[3] = 0;

  Serial.println("Config cleared successfully !");
}


void testWithServo(){
  int x, y;
  int xVar, yVar;
  while(digitalRead(JOY_SW1) == HIGH){
    x = analogRead(JOY_X1);
    y = analogRead(JOY_Y1);
    xVar = map(x, 0, 1023, ServoLimit[1], ServoLimit[0]);
    yVar = map(y, 0, 1023, ServoLimit[3], ServoLimit[2]);
    servo1.write(xVar);
    servo2.write(yVar);
  }
  servo1.write(90);
  servo2.write(90);
}

void servo_predefined_limits(){
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




void setServoLimits(){
  Serial.println("Taking servo limits now ");
  Serial.println("Press the encoder button to save state");

  encoder.setPosition(0);

  delay(100);

  String servoStates[2] = {"High", "LOW"};
  int servoStateCount = 2;
  int servoCount = 2;



  for(int i = 0; i < servoCount; i++){

    for (int j = 0; j < servoStateCount; j++){

      encoder.setPosition(0);

      int value = encoder.getPosition();
      int initValue = value;
      int angle = 90;

      Serial.println("Servo " + String(i) + " state " + String(servoStates[j]));
      

      while(true){
        encoder.tick();

        value = encoder.getPosition();
        if(initValue != value){
          initValue = value;
          angle = 90 + value;
          if (i == 0){
            servo1.write(angle);
          }
          if (i == 1){
            servo2.write(angle);
          }
        }

        if (digitalRead(R1_SW) == LOW){
          ServoLimit[i*2 + j] = angle;
          if (i == 0){
            servo1.write(90);
          }
          if (i == 1){
            servo2.write(90);
          }

          delay(200);
          break;
        }
        
      }
      Serial.println(String(angle));
    }
  }

  for(int i = 0; i < servoCount; i += 2){
    if(ServoLimit[i] < ServoLimit[i+1]){
      int temp = ServoLimit[i];
      ServoLimit[i] = ServoLimit[i+1];
      ServoLimit[i+1] = temp;
    }
  }


  for(int i = 0; i < servoCount; i++){
    for (int j = 0; j < servoStateCount; j++){
      Serial.println("Servo " + String(i) + " state " + String(servoStates[j]) + ": " + String(ServoLimit[i*2 + j]));
    }
  }

}