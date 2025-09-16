#include <Ticker.h>
#include <Arduino.h>

int ylwButPin = 18;    //Changes
int grnButPin = 17;    //Changes
int redButPin = 16;    //Changes
int bluButPin = 15;    //Changes
int ylwLEDPin = 5;     //Changes
int grnLEDPin = 6;     //Changes
int redLEDPin = 9;     //Changes
int bluLEDPin = 10;    //Changes
float stage = 0;
bool prevylwReading = 1;
bool prevgrnReading = 1;
bool prevredReading = 1;
bool prevbluReading = 1;

Ticker myTimer;
volatile bool timerFired = false;

void onTimer() {
  timerFired = true;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32-S3!");
  pinMode(ylwLEDPin, OUTPUT);
  pinMode(grnLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(bluLEDPin, OUTPUT);
  pinMode(ylwButPin, INPUT_PULLUP);
  pinMode(grnButPin, INPUT_PULLUP);
  pinMode(redButPin, INPUT_PULLUP);
  pinMode(bluButPin, INPUT_PULLUP);

}

void reset(){
  myTimer.detach();
  digitalWrite(ylwLEDPin, 0);
  digitalWrite(grnLEDPin, 0);
  digitalWrite(redLEDPin, 0);
  digitalWrite(bluLEDPin, 0);
  stage = 0;
  Serial.println("Resetting...");
}

void loop() {  
  if (stage == 0) {
    bool yBReading = digitalRead(ylwButPin);
    if (!yBReading && prevylwReading){
      stage = 1;
      myTimer.once(3, onTimer);
    }
    prevylwReading = yBReading;
  }

  if (stage == 1){
    if (timerFired){
      stage = 2;
      timerFired = false;
    } else {
      bool yBReading = digitalRead(ylwButPin);
      if (!yBReading && prevylwReading){
        reset();
      } else {
        prevylwReading = yBReading;
        delay(200);
        digitalWrite(ylwLEDPin, 0);
        delay(250);
        digitalWrite(ylwLEDPin, 1);
      }
      prevylwReading = yBReading;

    }
  }

  if (stage == 2){
    bool gBReading = digitalRead(grnButPin);
    if (!gBReading && prevgrnReading){
      stage = 2.5;
      digitalWrite(grnLEDPin, 1);
    }
    prevgrnReading = gBReading;
  }

  if (stage == 2.5){
    bool rBReading = digitalRead(redButPin);
    if (!rBReading && prevredReading){
      stage = 3;
      myTimer.once(5, onTimer);
    }
    prevredReading = rBReading;
  }

  if (stage == 3){
    if (timerFired){
      stage = 4;
      timerFired = false;
    } else {
      delay(200);
      digitalWrite(redLEDPin, 0);
      delay(250);
      digitalWrite(redLEDPin, 1);
    }
  }

  if (stage == 4){
    bool bBReading = digitalRead(bluButPin);
    if (!bBReading && prevbluReading){
      digitalWrite(bluLEDPin, 1);
      delay(5000);
      reset();
    }
    prevbluReading = bBReading;
  }
  
  Serial.print("Stage: ");
  Serial.println(stage);
  delay(50);
}
