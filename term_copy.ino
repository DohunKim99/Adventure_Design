#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#define RST_PIN 9
#define SS_PIN 10

//MOSI: Pin 51
//MISO: Pin 50
//SCK: Pin 52
Servo myServo;
int servoPin = 11;
MFRC522 mfrc(SS_PIN, RST_PIN); //객체 생성
int trigPin = 3;
int echoPin = 2;
LiquidCrystal lcd(44,45,46,47,48,49);
bool flag = false;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  lcd.begin(16,2);
  SPI.begin();
  myServo.attach(servoPin);
}

float checkDistance(){
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  float distance = duration * 340 / 10000 / 2;

  Serial.println(String("Distance(cm): ") + distance);
  return distance;
}

void loop() {
  
  float distance = checkDistance();

  if(distance < 30) {
    if(!flag) {
      flag = true;
      lcd.print("Hello!");

      if(!mfrc.PICC_IsNewCardPresent() || !mfrc.PICC_ReadCardSerial()) {
        delay(500);
        return;
      }

      if(mfrc.uid.uidByte[0] == 20 && mfrc.uid.uidByte[1] == 41 && 
      mfrc.uid.uidByte[2] == 180 && mfrc.uid.uidByte[3] == 89) {
        lcd.clear();
        lcd.print("open!");
        myServo.write(90);

        delay(3000);

        myServo.write(0);
      }
      
    }
  }
  else {
    lcd.clear();
    flag = false;
  }

  delay(500);

}
