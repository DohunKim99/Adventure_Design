#include <LiquidCrystal.h>

int trigPin = 3;
int echoPin = 2;
LiquidCrystal lcd(44,45,46,47,48,49);
bool flag = false;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  lcd.begin(16,2);

}

void loop() {
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  float distance = duration * 340 / 10000 / 2;

  Serial.println(String("Distance(cm): ") + distance);

  if(distance < 20) {
    if(!flag) {
      flag = true;
      lcd.print("Hello!");
    }
  }
  else {
    lcd.clear();
    flag = false;
  }

  delay(500);

}
