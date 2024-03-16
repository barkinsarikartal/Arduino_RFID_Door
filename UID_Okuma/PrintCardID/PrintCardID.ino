/*
  This code sequence is written to read RFID cards and print their card IDs to Serial Monitor.

  7916 bytes (25%) of program storage space, 384 bytes (18%) of dynamic memory.

  Contributors:
    Arda YILDIZ ardayildiz029@gmail.com
    Barkın SARIKARTAL barkinsarikartal51@gmail.com
  Supporters:
    Burçin TUNA btuna@cankaya.edu.tr
    H. Hakan MARAŞ hhmaras@cankaya.edu.tr
*/

#include <MFRC522v2.h> //using version: 2.0.4
#include <MFRC522DriverSPI.h> //comes with MFRC522v2.h version: 2.0.4
#include <MFRC522DriverPinSimple.h> //comes with MFRC522v2.h version: 2.0.4
#include <MFRC522Debug.h> //comes with MFRC522v2.h version: 2.0.4
#include <Wire.h> //comes installed with Arduino IDE

MFRC522DriverPinSimple ss_pin(10);
MFRC522DriverSPI driver{ss_pin};
MFRC522 reader{driver};
String readCardID = "";

void setup() {
  Serial.begin(115200);
  while (!Serial);
  reader.PCD_Init();
	Serial.println(F("Kartınızı Okutun: "));
}

void loop() {
	if (reader.PICC_IsNewCardPresent() && reader.PICC_ReadCardSerial()) {

  readCardID = "";

    for (byte i = 0; i < reader.uid.size; i++) {
      readCardID += String(reader.uid.uidByte[i] < 0x10 ? " 0" : " ");
      readCardID += String(reader.uid.uidByte[i], HEX);
    }

    readCardID.toUpperCase();
    Serial.println(readCardID);
    delay(500);
  }
}