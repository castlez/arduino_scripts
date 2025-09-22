#include <IRremote.hpp>

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

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
  pinMode(LED_PIN, OUTPUT);
  pinMode(IR_RECEIVE_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);
  ledState=LOW;
  potVal = analogRead(potPin);
  //cdMillis = (potVal/1023) * cdMAX;
  cdMillis = 123;  // magic number found using potentiometer
}

void loop() {
  potVal = analogRead(potPin);
  //cdMillis = (float(potVal)/float(pMAX)) * float(cdMAX);
  if (IrReceiver.decode()) {
    if (3125149440 == IrReceiver.decodedIRData.decodedRawData || 0 == IrReceiver.decodedIRData.decodedRawData){
      lastReleased = millis();
      digitalWrite(LED_PIN, HIGH);
    }
    IrReceiver.resume(); // Ready for next code
  }
  else{
    if (millis() - lastReleased > cdMillis) {
      digitalWrite(LED_PIN, LOW);
    }
  }
}