#include "MeMegaPi.h"

MeMegaPiDCMotor motor1(PORT1B);
MeMegaPiDCMotor motor2(PORT2B);
MeMegaPiDCMotor motor3(PORT3B);
MeMegaPiDCMotor motor4(PORT4B);

MeUltrasonicSensor ultraSensor(PORT_7);
MeLineFollower lineFinder(PORT_8);

uint8_t motorMaxSpeed = 255;
uint8_t motorSpeed = 255;
uint8_t motorCarSpeed = 35;

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
  while (distance < 33) {
    motor2.run(-motorCarSpeed);
    distance = ultraSensor.distanceCm();
  }
  motor2.stop();

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
        //      delay(10000);
        linePosition();
        motor3.run(255);
        while (turnCount < 25) {
          lineTrace();
          if (turnCount == 7 || turnCount == 14) {
            upHeight();
          }
        }
        motor3.stop();
        motor2.stop();
        delay(10000);
        homePosition();
        setDownHeight();
        turnCount = 0;
        break;
      case '2':
        //        delay(14000);
        linePosition();
        motor3.run(120);
        while (turnCount < 40) {
          lineTrace();
          if (turnCount == 7 || turnCount == 14 || turnCount == 18 || turnCount == 22 || turnCount == 27 || turnCount == 33) {
            upHeight();
          }
        }
        motor3.stop();
        motor2.stop();
        delay(10000);
        homePosition();
        setDownHeight();
        turnCount = 0;
        break;
      case '3':
        linePosition();
        motor3.run(130);
        while (turnCount < 55) {
          lineTrace();
          if (turnCount == 7 || turnCount == 16 || turnCount == 24 || turnCount == 32 || turnCount == 40 || turnCount == 46 || turnCount == 55) {
            upHeight();
          }
          if (turnCount > 18) {
            motor3.run(100);
          }
        }
        motor3.stop();
        motor2.stop();
        delay(10000);
        homePosition();
        setDownHeight();
        turnCount = 0;
        break;
      case '4':
        linePosition();
        motor3.run(120);
        while (turnCount < 70) {
          lineTrace();
          if (turnCount == 7 || turnCount == 16 || turnCount == 24 || turnCount == 32 || turnCount == 40 || turnCount == 46 || turnCount == 55 || turnCount == 62) {
            upHeight();
          }
          if (turnCount > 14) {
            motor3.run(100);
          }
        }
        motor3.stop();
        motor2.stop();
        delay(10000);
        homePosition();
        setDownHeight();
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
  motor4.run(-50);
  delay(200);
  motor4.stop();
}

void setDownHeight() {
  motor4.run(100);
  delay(500);
  motor4.stop();
}

