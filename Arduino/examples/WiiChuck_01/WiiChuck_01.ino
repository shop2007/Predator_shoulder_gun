#include <Wire.h>
#include <WiiChuck.h>

Nunchuck nunchuck;

void setup() {
  Serial.begin(115200);
  nunchuck.begin();
}

void loop() {
  nunchuck.update(); // Aggiorna i dati

  // Lettura joystick
  int joyX = nunchuck.readJoyX();
  int joyY = nunchuck.readJoyY();

  // Lettura accelerometro
  int accelX = nunchuck.readAccelX();
  int accelY = nunchuck.readAccelY();
  int accelZ = nunchuck.readAccelZ();

  // Lettura pulsanti
  bool buttonC = nunchuck.buttonC();
  bool buttonZ = nunchuck.buttonZ();

  // Stampa su seriale
  Serial.print("Joy X: "); Serial.print(joyX);
  Serial.print(" | Joy Y: "); Serial.print(joyY);
  Serial.print(" | Accel X: "); Serial.print(accelX);
  Serial.print(" | Accel Y: "); Serial.print(accelY);
  Serial.print(" | Accel Z: "); Serial.print(accelZ);
  Serial.print(" | C: "); Serial.print(buttonC);
  Serial.print(" | Z: "); Serial.println(buttonZ);

  delay(100);
}
