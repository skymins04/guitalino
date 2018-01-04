//Digital Pin
#define ECHO 11
#define TRIG 10
#define SPEAKER 6
//Analog Pin
#define JOYSTICK 0
#define VIBRATO 1
#define ECHO_TIME 2
//MPU-6050 I2C Address
#define MPU 0x68

float duration;
int sound;

#include <Wire.h>

void setup() {
  Wire.begin();
 
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x0);
  Wire.endTransmission();
  
  Serial.begin(9600);
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(SPEAKER, OUTPUT);
}

void loop() { 
  int AX = 0, AY = 0;
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission();

  Wire.requestFrom(MPU, 14);
  AX=Wire.read()<<8|Wire.read();
  AY=Wire.read()<<8|Wire.read();
  
  int sonic = ReadSonic();
  if(analogRead(0) > 520 || analogRead(0) < 490) {
    sound = 1;
    int amplitude =  map(analogRead(1), 0, 1023, 10, 40);
    if(AY > 10000) {
      tone(SPEAKER,sonic+amplitude,1000000);
    } else if(AY > 5000) {
      tone(SPEAKER,sonic,1000000);
    } else {
      tone(SPEAKER,sonic-amplitude,1000000);
    }   
  } else {
    if(sound == 1) {
      sound = 0;  
    }  
    if(sound == 0) {
      if(analogRead(2) > 20) {
        int echo = sonic;
        for(;;) {
          int echoTime = analogRead(2);
          echo -= 1;
          tone(6, echo, 0);
          if(analogRead(0) > 520 || analogRead(0) < 490) {
            break;  
          }
          if(echo == 0) {
            for(;;) {
              noTone(6);
              if(analogRead(0) > 520 || analogRead(0) < 490) {
                break;  
              }
            }  
          }
          echoTime -= 10;
          if(echoTime < 0) {echoTime = 0;}
          delay(echoTime);  
        }
      } 
      for(;;) {
        noTone(6);
        sonic = 0;
        if(analogRead(0) > 520 || analogRead(0) < 490) {
          break;  
        }
      }  
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int ReadSonic() {
  digitalWrite(TRIG, LOW);
  digitalWrite(ECHO, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  unsigned long duration = pulseIn(ECHO, HIGH);
  return duration;
}
