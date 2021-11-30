
#include <Sleep_n0m1.h>
#define ma_1 2
#define ma_2 3
#define ma_3 4
#define ma_4 5
#define limTop 7
#define limBot 8
#define sensor A0      // 0=day  1=night
int machState = 0;
int dly = 2000; //delay in microseconds for motorspeed
int topState = 0;
int botState = 0;
int sensorState = 0;
int oldSensorState = 0;
int count = 0;
Sleep sleep;
unsigned long sleepTime;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ma_1, OUTPUT);
  pinMode(ma_2, OUTPUT);
  pinMode(ma_3, OUTPUT);
  pinMode(ma_4, OUTPUT);
  pinMode(limTop, INPUT_PULLUP);
  pinMode(limBot, INPUT_PULLUP);
  pinMode(sensor, INPUT_PULLUP);
  sleepTime = 300000;               //time between readings in standBy
  digitalWrite(13, HIGH);
  delay(30000);   // powerup time of sensor to calibrate

  // Look if the door needs to be opened or closed

  if (digitalRead(sensor) == 0) {   //if it is day
    while (digitalRead(limTop) == 1)  {       // if the door is not fully open
      openDoor();
    }
    sensorState = 0;
    oldSensorState = 0;
    stopMotor();
  }

  if (digitalRead(sensor) == 1) {         // if it is night
    while (digitalRead(limBot) == 1)  {       // if the door is not fully closed
      closeDoor();
    }
    sensorState = 1;
    oldSensorState = 1;
    stopMotor();
  }
  digitalWrite(13, LOW);
}

void loop() {

  // check if sensor changed from day to night or vice versa
  digitalWrite(13, HIGH);
  delay(100);             // delay to stabilise sensorreadings
  sensorState = digitalRead(sensor);
  if (sensorState == oldSensorState)  {

    // if no change goto sleep for x minutes
    digitalWrite(13, LOW);
    sleep.pwrDownMode(); //set sleep mode
    sleep.sleepDelay(sleepTime);
  }

  // if yes, go into transition mode and open or close the door
  if (sensorState != oldSensorState) {
    //   oldSensorState = sensorState;
    count  ++;
    digitalWrite(13, LOW);
    delay(30000);            // time to recheck sensorstate
    digitalWrite(13, HIGH);    Serial.print("count  ");
    Serial.println( count);
  }


  if (count == 10) {       // how many times we want to check

    topState = digitalRead(limTop);
    botState = digitalRead(limBot);
    if (sensorState == 1) {
      while (botState == 1 && count == 10)  {
        closeDoor();
        botState = digitalRead(limBot);
        if (botState == 0)  {
          oldSensorState = sensorState;
          stopMotor();
        }
      }
    }
    if (sensorState == 0) {
      while (topState == 1 && count == 10 )  {
        openDoor();
        topState = digitalRead(limTop);
        if (topState == 0)  {
          oldSensorState = sensorState;
          stopMotor();
        }
      }
    }
  }
} //end of loop

void closeDoor() {
  digitalWrite(ma_1, HIGH);
  digitalWrite(ma_2, HIGH);
  digitalWrite(ma_3, LOW);
  digitalWrite(ma_4, LOW);
  delayMicroseconds(dly);

  digitalWrite(ma_1, LOW);
  digitalWrite(ma_2, HIGH);
  digitalWrite(ma_3, HIGH);
  digitalWrite(ma_4, LOW);
  delayMicroseconds(dly);

  digitalWrite(ma_1, LOW);
  digitalWrite(ma_2, LOW);
  digitalWrite(ma_3, HIGH);
  digitalWrite(ma_4, HIGH);
  delayMicroseconds(dly);

  digitalWrite(ma_1, HIGH);
  digitalWrite(ma_2, LOW);
  digitalWrite(ma_3, LOW);
  digitalWrite(ma_4, HIGH);
  delayMicroseconds(dly);
}

void openDoor() {
  digitalWrite(ma_1, HIGH);
  digitalWrite(ma_2, LOW);
  digitalWrite(ma_3, LOW);
  digitalWrite(ma_4, HIGH);
  delayMicroseconds(dly);

  digitalWrite(ma_1, LOW);
  digitalWrite(ma_2, LOW);
  digitalWrite(ma_3, HIGH);
  digitalWrite(ma_4, HIGH);
  delayMicroseconds(dly);

  digitalWrite(ma_1, LOW);
  digitalWrite(ma_2, HIGH);
  digitalWrite(ma_3, HIGH);
  digitalWrite(ma_4, LOW);
  delayMicroseconds(dly);

  digitalWrite(ma_1, HIGH);
  digitalWrite(ma_2, HIGH);
  digitalWrite(ma_3, LOW);
  digitalWrite(ma_4, LOW);
  delayMicroseconds(dly);
}

void stopMotor() {
  digitalWrite(ma_1, LOW);
  digitalWrite(ma_2, LOW);
  digitalWrite(ma_3, LOW);
  digitalWrite(ma_4, LOW);
  digitalWrite(13, LOW);
  count = 0;
  sleep.pwrDownMode(); //set sleep mode
  sleep.sleepDelay(sleepTime);
}
