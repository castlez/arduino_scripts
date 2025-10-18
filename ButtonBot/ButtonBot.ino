#include <IRremote.hpp>
#include <Servo.h>

int IR_RECEIVE_PIN = 8;
//IRRawDataType irActivated = 4278238976; // tinkercad
IRRawDataType irActivated = 3125149440; // live
unsigned long lastReleased = millis();  // current time in milliseconds (i think since start of execution)
unsigned long cdMillis;  // cooldown milliseconds: button buffer of sorts, to avoid "bouncing" 

// servo
Servo servo_9;
int servoPin = 9;
int pos = 0;  // current position
int dest = 125; // fully pushing button
int sStep = 1; // servo speed
bool goingUp = false;
bool moving = false;
const float shuntResistor = 10.0; // Ohms

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
  pinMode(IR_RECEIVE_PIN, INPUT);
  cdMillis = 120;  // magic number found using potentiometer
  servo_9.attach(servoPin, 500, 2500);
  servo_9.write(0);
}

void loop() {
  // int rawValue = analogRead(currentPin);          // Read analog value (0..1023)
  // float measuredVoltage = rawValue * (5.0 / 1023);// Convert to voltage (assuming 5V reference)
  // float current = measuredVoltage / shuntResistor;// Ohm's Law: I = V / R
  // Serial.println(current)
  if (millis() - lastReleased > cdMillis) {
    if (IrReceiver.decode()) {
      //Serial.println(IrReceiver.decodedIRData.decodedRawData);

      if (!moving && irActivated == IrReceiver.decodedIRData.decodedRawData) {
        lastReleased = millis();
        goingUp = true;
        moving = true;
      }
      IrReceiver.resume(); // Ready for next code
    }
  }
  if (moving) {
    if (goingUp) {
      if (pos >= dest) {
        goingUp = false;
      }
      else {
        pos = pos + sStep;
      }
    }
    else {
      if (pos <= 0) {
        moving = false;
      }
      else {
        pos = pos - sStep;
      }
    }
  }
  Serial.println(pos);
  servo_9.write(pos);
}