/*
  This code sequence is written to read RFID cards and trigger an electronic door lock if card ID is valid.

  8658 bytes (28%) of program storage space, 478 bytes (23%) of dynamic memory.
  "LiquidCrystal functions are commented because they were decided not to be necessary by the project provider."

  Contributors:
    Arda YILDIZ ardayildiz029@gmail.com
    Barkın SARIKARTAL barkinsarikartal51@gmail.com
  Supporters:
    Burçin TUNA btuna@cankaya.edu.tr
    H. Hakan MARAŞ hhmaras@cankaya.edu.tr
*/

/* #include <LiquidCrystal_I2C.h> //https://github.com/johnrickman/LiquidCrystal_I2C */
#include <MFRC522v2.h> //using version: 2.0.4
#include <MFRC522DriverSPI.h> //comes with MFRC522v2.h version: 2.0.4
#include <MFRC522DriverPinSimple.h> //comes with MFRC522v2.h version: 2.0.4
#include <MFRC522Debug.h> //comes with MFRC522v2.h version: 2.0.4
#include <Wire.h> //comes installed with Arduino IDE
#include "ValidCards.h" //can be found in this project's github repository.

/* LiquidCrystal_I2C lcd(0x27, 16, 2); */

MFRC522DriverPinSimple ss_pin(10);
MFRC522DriverSPI driver{ss_pin};
MFRC522 reader{driver};
String readCardID = "";
int resetCounter = 0;

void setup() {
  DDRD |= (1 << PD3); //defined digital 3 and 5 as output by port manipulation to save memory.
  DDRD |= (1 << PD5);
  Serial.begin(115200);
  while(!Serial);
  reader.PCD_Init();
  /* 
    lcd.init(); 
    lcd.backlight();
    mainLCDScreen();
  */
}

void loop() {
  if (reader.PICC_IsNewCardPresent() && reader.PICC_ReadCardSerial()) { //checks if a new card is present and if card serial can be read

    readCardID = "";

    for (byte i = 0; i < reader.uid.size; i++) {
      readCardID += String(reader.uid.uidByte[i] < 0x10 ? " 0" : " ");
      readCardID += String(reader.uid.uidByte[i], HEX);
    }

    readCardID.toUpperCase();

    if (isCardIDValid(readCardID)) {
      validCard();
    } 
    else {
      declinedCard();
    }

    readCardID = "";
    /* mainLCDScreen(); */
    reader.PICC_HaltA(); //halts the RC522's operation
    reader.PCD_StopCrypto1(); //stops cryptographic functions to secure communication
  }

  resetCounter++;

  if((resetCounter % 600) == 0){ //resets the RC522 module in every 10 minutes to prevent it from falling asleep
    /*
      lcd.clear();
      lcd.print("LUTFEN");
      lcd.setCursor(0,1);
      lcd.print("BEKLEYIN");
    */
    reader.PCD_Reset();
    resetCounter = 0;
    delay(500);
    reader.PCD_Init();
    delay(500);
    /* mainLCDScreen(); */
  }

  delay(500);
}

bool isCardIDValid(String readCardID) { //checks if the card ID is valid or not.
  for (int j = 0; j < NUM_VALID_CARD_IDS; j++) {
    if (readCardID == validCardIDs[j]) {
      return true;
    }
  }
  return false;
}

/*
  void mainLCDScreen(){
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("KART");
    lcd.setCursor(5,1);
    lcd.print("OKUTUN");
  }
*/

void validCard(){ //triggers the relay if card ID is valid.
/*
  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("KAPI");
  lcd.setCursor(4,1);
  lcd.print("ACILIYOR");
*/
  PORTD |= (1 << PD3); //instead of using digitalWrite funciton, used port manipulation to save memory.
  delay(50);
  PORTD &= ~(1 << PD3);
  delay(20);
  PORTD |= (1 << PD3);
  delay(50);
  PORTD &= ~(1 << PD3);
  PORTD |= (1 << PD5);
  delay(2000);
  PORTD &= ~(1 << PD5);
}

void declinedCard(){ //triggers the buzzer with a warning sound if card ID is not valid.
/*
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("YETKISIZ");
  lcd.setCursor(6,1);
  lcd.print("KART");
*/
  PORTD |= (1 << PD3);
  delay(600);
  PORTD &= ~(1 << PD3);
  delay(100);
  PORTD |= (1 << PD3);
  delay(600);
  PORTD &= ~(1 << PD3);
  delay(100);
  PORTD |= (1 << PD3);
  delay(600);
  PORTD &= ~(1 << PD3);
}