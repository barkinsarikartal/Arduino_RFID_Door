#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "ValidCards.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

MFRC522DriverPinSimple ss_pin(10);
MFRC522DriverSPI driver{ss_pin};
MFRC522 reader{driver};

String okunanKartID = "";

int counter = 0;
int resetCounter = 0;

void setup() {
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);

  Serial.begin(115200);
  while(!Serial);
  
  reader.PCD_Init();
  Serial.print(F("Reader "));
  static uint8_t i = 0;
  i++;
  Serial.print(i);
  Serial.print(F(": "));
  MFRC522Debug::PCD_DumpVersionToSerial(reader, Serial);
  lcd.init(); 
  lcd.backlight();
  mainLCDScreen();
}

void loop() {
  if (reader.PICC_IsNewCardPresent() && reader.PICC_ReadCardSerial()) {
    
    okunanKartID = "";

    for (byte i = 0; i < reader.uid.size; i++) {
      okunanKartID += String(reader.uid.uidByte[i] < 0x10 ? " 0" : " ");
      okunanKartID += String(reader.uid.uidByte[i], HEX);
    }

    okunanKartID.toUpperCase();

    Serial.print(okunanKartID);
    Serial.println(" okunan kart");

    if (isCardIDValid(okunanKartID)) {
      validCard();
  } else {
      declinedCard();
  }

    okunanKartID = "";
      
    mainLCDScreen();
      
    reader.PICC_HaltA();
    reader.PCD_StopCrypto1();
  }
  Serial.print(counter);
  Serial.println(". kontrol yapıldı");
  counter++;
  resetCounter++;
  
  if((resetCounter % 600) == 0){
    lcd.clear();
    lcd.print("LUTFEN");
    lcd.setCursor(0,1);
    lcd.print("BEKLEYIN");
    reader.PCD_Reset();
    Serial.print(resetCounter/600);
    Serial.println(". reset done.");
    delay(500);
    reader.PCD_Init();
    delay(500);
    Serial.println("initted reader again.");
    mainLCDScreen();
  }
  

  delay(500);
}

bool isCardIDValid(String okunanKartID) {
  for (int j = 0; j < NUM_VALID_CARD_IDS; j++) {
    if (okunanKartID == validCardIDs[j]) {
      return true;
    }
  }
  return false;
}

void mainLCDScreen(){
  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("KART");
  lcd.setCursor(5,1);
  lcd.print("OKUTUN");
}

void validCard(){
  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("KAPI");
  lcd.setCursor(4,1);
  lcd.print("ACILIYOR");
  digitalWrite(3, HIGH);
  delay(50);
  digitalWrite(3, LOW);
  delay(20);
  digitalWrite(3, HIGH);
  delay(50);
  digitalWrite(3, LOW);
  digitalWrite(5, HIGH);
  delay(3000);
  digitalWrite(5, LOW);
}
  
void declinedCard(){
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("YETKISIZ");
  lcd.setCursor(6,1);
  lcd.print("KART");
  analogWrite(3, 155);
  delay(600);
  digitalWrite(3, LOW);
  delay(100);
  analogWrite(3, 155);
  delay(600);
  digitalWrite(3, LOW);
  delay(100);
  analogWrite(3, 155);
  delay(600);
  digitalWrite(3, LOW);
  delay(100);
}