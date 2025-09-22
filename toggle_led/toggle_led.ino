int led=13;
int button=12;

int led_state = HIGH;
int cur_button;
int prev_button = LOW;

void setup() {
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  Serial.println("Starting butt");

}

void loop() {
  cur_button = digitalRead(button);
  if (cur_button == HIGH && prev_button == LOW) {
    Serial.println("POOSH");
    if (led_state == HIGH){
      led_state = LOW;
      Serial.println("LOW");
    }
    else {
      led_state = HIGH;
      Serial.println("HIGH");
    }
  }
  digitalWrite(led, led_state);
  prev_button = cur_button;
}
