#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#define RST_PIN 9
#define SS_PIN 10 //SDA

#define FAR 0
#define NEAR 1
#define OPEN 2

//MOSI: Pin 51
//MISO: Pin 50
//SCK: Pin 52
Servo myServo;
int servoPin = 11;
MFRC522 mfrc(SS_PIN, RST_PIN); //객체 생성
int trigPin = 3;
int echoPin = 2;
LiquidCrystal lcd(44,45,46,47,48,49);
int flag = 0;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  lcd.begin(16,2);
  SPI.begin();
  myServo.attach(servoPin);
  mfrc.PCD_Init();
  Serial.println();
  Serial.println(mfrc.PCD_PerformSelfTest() ? "ok" : "ERROR!");
  mfrc.PCD_Init();
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

bool cardExists(){
  return mfrc.PICC_IsNewCardPresent() && mfrc.PICC_ReadCardSerial();
}

bool rightCard(){
  return mfrc.uid.uidByte[0] == 20 && mfrc.uid.uidByte[1] == 41 && mfrc.uid.uidByte[2] == 180 && mfrc.uid.uidByte[3] == 89;
}

void loop() {
  
  float distance = checkDistance();

  if(distance < 20) {
    if (flag == FAR){
      lcd.print("Hello!");
      flag = NEAR;
    }

    if(!cardExists()) {
      delay(500);
      Serial.println("No Card Exists");
      return;
    }

    if(rightCard()) {
      flag = OPEN;
      lcd.clear();
      lcd.print("open!");
      Serial.println("Open");
      myServo.write(90);

      delay(2000);
      lcd.clear();
      myServo.write(0);
    }
    else{
      Serial.println("Not Right Card");
    }
      
  }
  else {
    lcd.clear();
    flag = FAR;
    Serial.println("Too Far Away");
  }
  Serial.println(mfrc.PCD_PerformSelfTest() ? "ok" : "ERROR!");
  delay(500);

}