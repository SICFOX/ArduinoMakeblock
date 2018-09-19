#include "MeMegaPi.h"

MeMegaPiDCMotor motor1(PORT1B);
MeMegaPiDCMotor motor2(PORT2B);
MeMegaPiDCMotor motor3(PORT3B);
MeMegaPiDCMotor motor4(PORT4B);

MeUltrasonicSensor ultraSensor(PORT_7);
MeLineFollower lineFinder(PORT_8);

uint8_t motorMaxSpeed = 255;
uint8_t motorSpeed = 255;
uint8_t motorCarSpeed = 30;

char inputkey;

int turnCount;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  sizeInput();
}

//Makeblockを所定の位置に戻す関数
void homePosition() {
  //超音波センサの起動直後の無視するために空読み
  float distance = ultraSensor.distanceCm();
  for (int i = 0; i < 20; i++) {
    distance = ultraSensor.distanceCm();
    //    Serial.println(distance);
    delay(50);
  }
  while (distance < 30) {
    motor2.run(-motorCarSpeed);
    distance = ultraSensor.distanceCm();
  }
  motor2.stop();
  setDownHeight();
}

//Makeblockをラインの上まで移動させる関数
void linePosition() {
  //超音波センサの起動直後の無視するために空読み
  float distance = ultraSensor.distanceCm();
  for (int i = 0; i < 20; i++) {
    distance = ultraSensor.distanceCm();
    delay(50);
  }
  //ラインの端に来たらストップ
  while (distance > 18) {
    motor2.run(motorCarSpeed);
    distance = ultraSensor.distanceCm();
  }
  motor2.stop();
  turnCount = 0;
}

//ライン上を往復させ続ける関数
void lineTrace() {
  int sensorState = lineFinder.readSensors();
  switch (sensorState)
  {
    case S1_IN_S2_IN:
      break;
    case S1_IN_S2_OUT:
      motor2.run(motorCarSpeed);
      turnCount++;
      delay(500);
      break;
    case S1_OUT_S2_IN:
      motor2.run(-motorCarSpeed);
      turnCount++;
      delay(500);
      break;
    case S1_OUT_S2_OUT:
      motor2.stop();
      break;
    default:
      break;
  }
}

void sizeInput() {
  inputkey = Serial.read();
  if (inputkey != -1) {
    switch (inputkey) {
      case '1':
        homePosition();
        linePosition();
        while (turnCount < 5) {
          lineTrace();
          if(turnCount ==2 || turnCount == 4){
            upHeight();
          }
        }
        motor2.stop();
        turnCount = 0;
        break;
      case '2':
        homePosition();
        linePosition();
        while (turnCount < 10) {
          lineTrace();
          if(turnCount ==2 || turnCount == 4 || turnCount == 6){
            upHeight();
          }
        }
        motor2.stop();
        turnCount = 0;
        break;
      case '3':
        homePosition();
        linePosition();
        while (turnCount < 15) {
          lineTrace();
        }
        motor2.stop();
        turnCount = 0;
        break;
      default:
        break;
    }
  }
}

void changeHeight() {
  inputkey = Serial.read();
  if ( inputkey != -1) {
    switch (inputkey) {
      case 'd':
        downHeight();
        Serial.println("高さを上げる");
        break;
      case 'u':
        upHeight();
        Serial.println("高さを下げる");
        break;
    }
  }
}

void downHeight() {
  motor4.run(30);
  delay(200);
  motor4.stop();
}

void upHeight() {
  motor4.run(-80);
  delay(200);
  motor4.stop();
}

void setDownHeight(){
  motor4.run(100);
  delay(500);
  motor4.stop();
}

