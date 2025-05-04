/*
VCC (Pin 1, Rosso): Collegalo al pin 3.3V dell'Arduino. Alcuni Nunchuck possono funzionare anche a 5V, ma è più sicuro utilizzare 3.3V per evitare possibili danni.
GND (Pin 6, Bianco): Collegalo al pin GND dell'Arduino.
SDA (Pin 5, Verde): Collegalo al pin A4 dell'Arduino Uno/Nano (SDA).
SCL (Pin 2, Giallo): Collegalo al pin A5 dell'Arduino Uno/Nano (SCL).
*/


#include <Wire.h>




void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Inizializza il Nunchuck99
  Wire.beginTransmission(0x52); // Indirizzo I2C del Nunchuck
  Wire.write(0xF0);
  Wire.write(0x55);
  Wire.endTransmission();

  Wire.beginTransmission(0x52);
  Wire.write(0xFB);
  Wire.write(0x00);
  Wire.endTransmission();
}

void loop() {
  Wire.beginTransmission(0x52);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(0x52, 6);

  if (Wire.available() == 6) {
    int joyX = Wire.read();
    int joyY = Wire.read();
    int accelX = (Wire.read() << 2) | ((Wire.peek() >> 6) & 3);
    int accelY = (Wire.read() << 2) | ((Wire.peek() >> 4) & 3);
    int accelZ = (Wire.read() << 2) | ((Wire.read() >> 2) & 3);
    
    Serial.print("JoyX: "); Serial.print(joyX);
    Serial.print(" | JoyY: "); Serial.print(joyY);
    Serial.print(" | AccelX: "); Serial.print(accelX);
    Serial.print(" | AccelY: "); Serial.print(accelY);
    Serial.print(" | AccelZ: "); Serial.println(accelZ);
  }

  delay(100);
}
