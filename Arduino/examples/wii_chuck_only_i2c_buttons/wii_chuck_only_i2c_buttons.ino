#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Inizializza il Nunchuck
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
    int accelZ = (Wire.read() << 2) | ((Wire.peek() >> 2) & 3);
    int buttons = Wire.read(); // Legge il sesto byte

    // Decodifica i pulsanti
    bool buttonC = !(buttons & 0x02); // Il bit 1 rappresenta il pulsante C
    bool buttonZ = !(buttons & 0x01); // Il bit 0 rappresenta il pulsante Z
    
    Serial.print("JoyX: "); Serial.print(joyX);
    Serial.print(" | JoyY: "); Serial.print(joyY);
    Serial.print(" | AccelX: "); Serial.print(accelX);
    Serial.print(" | AccelY: "); Serial.print(accelY);
    Serial.print(" | AccelZ: "); Serial.print(accelZ);
    Serial.print(" | Button C: "); Serial.print(buttonC ? "Pressed" : "Released");
    Serial.print(" | Button Z: "); Serial.println(buttonZ ? "Pressed" : "Released");
  }

  delay(100);
}