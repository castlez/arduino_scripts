#include <IRremote.hpp>
#include <Servo.h>

int IR_RECEIVE_PIN = 8;
int LED_PIN=12;
int ledState;
unsigned long lastReleased = millis();  // current time in milliseconds (i think since start of execution)
unsigned long cdMillis;  // how long after releasing the button before the action stops
unsigned long cdMAX = 300;

// potentiometer
int potPin = A3;
unsigned long potVal = 0;
unsigned long pMAX = 1023;

// servo
int pos = 0;  // current position
int lastPos = 0;
int dest = 0; // desired position when button is on
int sStep = 2;
bool moving = false;
bool goingUp = false; 
int destChanged = 0;  // -1 down, 0 no change, 1 up           
Servo servo_9;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
  pinMode(LED_PIN, OUTPUT);
  pinMode(IR_RECEIVE_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);
  ledState=LOW;
  potVal = analogRead(potPin);
  cdMillis = 120;  // magic number found using potentiometer
  servo_9.attach(9, 500, 2500);
  servo_9.write(0);
  dest = int((float(potVal)/float(pMAX)) * float(180));
}

void loop() {
  potVal = analogRead(potPin);
  int newDest = int((float(potVal)/float(pMAX)) * float(180));
  if (pos == dest && newDest < dest) {
    destChanged = -1;
    dest = newDest;
    moving = true;
  }
  else if (pos == dest && newDest > dest){
    destChanged = 1;
    dest = newDest;
    moving = true;
  }
  else {
    destChanged = 0;
  }
  if (millis() - lastReleased > cdMillis) {
    if (IrReceiver.decode()) {
      if (!moving && 3125149440 == IrReceiver.decodedIRData.decodedRawData) {
        lastReleased = millis();
        moving = true;
        goingUp = !goingUp;
      }
      IrReceiver.resume(); // Ready for next code
    }
  }
  Serial.println(moving);
  
  if (moving){
    if (goingUp){
      if (pos >= dest) {
        moving = false;
      }
      else {
        pos = pos + sStep;
      }
    }
    else {  // going down
      if (pos <= 0){
        moving = false;
      }
      else {
        pos = pos - sStep;
      }
    }
  }
  // else {
  //   if (destChanged != 0 && goingUp) {
  //     moving = true;
  //     goingUp = destChanged > 0;
  //   }
  // }
  servo_9.write(pos);
  // Serial.print("Current POS: ");
  // Serial.println(pos);
  // Serial.print("Last Hit: ");
  // Serial.println(lastPos);

}