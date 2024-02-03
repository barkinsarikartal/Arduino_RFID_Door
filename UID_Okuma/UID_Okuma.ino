#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

MFRC522DriverPinSimple ss_pin(10);

MFRC522DriverSPI driver{ss_pin};

MFRC522 reader{driver};

String validcard = "c3 31 c5 06";

String okunanKartID = "";


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
  lcd.clear();
  lcd.print("Kart Okutun");
  lcd.setCursor(11,1);
  lcd.print("13:15");
}


void loop() {

  if (reader.PICC_IsNewCardPresent() && reader.PICC_ReadCardSerial()) {
      Serial.print(F("Reader "));
      static uint8_t i = 0;
      i++;
      Serial.print(i);
      
      Serial.print(F(": Card UID:"));
      MFRC522Debug::PrintUID(Serial, reader.uid);
      Serial.println();

      okunanKartID = "";

      for (byte i = 0; i < reader.uid.size; i++) {
        okunanKartID += String(reader.uid.uidByte[i] < 0x10 ? " 0" : " ");
        okunanKartID += String(reader.uid.uidByte[i], HEX);
      }

      Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = reader.PICC_GetType(reader.uid.sak);
      Serial.println(MFRC522Debug::PICC_GetTypeName(piccType));

      lcd.clear();
      lcd.print("Kart ID:");
      lcd.setCursor(0,1);
      MFRC522Debug::PrintUID(lcd, reader.uid);

      
      Serial.print(validcard);
      Serial.println("hafızadaki kart: ");
      Serial.print(okunanKartID);
      Serial.print("okunan kart: ");

      if(okunanKartID.equals(validcard)){
          lcd.clear();
          lcd.print("Kapi Aciliyor");
          buzzerValid();
          digitalWrite(5, HIGH);
          delay(3000);
          digitalWrite(5, LOW);
      }
      else if(okunanKartID != validcard){
          lcd.clear();
          lcd.print("Yetkisiz Kart");
          buzzerDecline();
      }

      okunanKartID = "";
      
      lcd.clear();
      lcd.print("Kart Okutun");
      lcd.setCursor(11,1);
      lcd.print("13:15");
      
      reader.PICC_HaltA();

      reader.PCD_StopCrypto1();
    }
  }

  void buzzerValid(){
      digitalWrite(3, HIGH);
      delay(200);
      digitalWrite(3, LOW);
      delay(100);
      digitalWrite(3, HIGH);
      delay(200);
      digitalWrite(3, LOW);
  }
  
  void buzzerDecline(){
      digitalWrite(3, HIGH);
      delay(600);
      digitalWrite(3, LOW);
      delay(100);
      digitalWrite(3, HIGH);
      delay(600);
      digitalWrite(3, LOW);
      delay(100);
      digitalWrite(3, HIGH);
      delay(600);
      digitalWrite(3, LOW);
      delay(100);
  }