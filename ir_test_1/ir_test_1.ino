#include <IRremote.hpp>

int IR_RECEIVE_PIN = 8;
int LED_PIN=12;
int ledState;
//int irbuttoncode = 0;
IRRawDataType irbuttoncode = 3125149440;
IRRawDataType irzero = 0;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
  pinMode(LED_PIN, OUTPUT);
  pinMode(IR_RECEIVE_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);
  ledState=LOW;
}

void loop() {
  if (IrReceiver.decode()) {
    if (3125149440 == IrReceiver.decodedIRData.decodedRawData || 0 == IrReceiver.decodedIRData.decodedRawData){
      Serial.println(millis());
      digitalWrite(LED_PIN, HIGH);
    }
  }
  else {
      digitalWrite(LED_PIN, LOW);
  }
  IrReceiver.resume(); // Ready for next code
}