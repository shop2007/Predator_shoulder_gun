/*
Template MENU for Arduino by Luciano Pautasso
This software is freeware and is released under the "MIT license" rules.
It is requested that the phrase "Template MENU for Arduino by Luciano Pautasso" is not removed.
Nunnchuck. 
  red wire= 5v : 
  white wire=ground: 
  green wire =pin analog 4: 
  yellow wire= pin analog 5

Il connettore del Nunchuck ha 4 pin principali:
Pin	Colore filo (di solito)	Funzione
1	Rosso	VCC (3.3V o 5V)
2	Nero	GND
3	Verde	Data (SDA, I2C)
4	Bianco	Clock (SCL, I2C)

*/
#include "Parametri.h"
#include <Wire.h>
#include <Servo.h>
//#include <ArduinoNunchuk.h>

Servo servo1;
Servo servo2;

bool debug= false;


String Versione = "Predator_Menu_08";
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
bool ExitFromMenu = false;
char Cestino;

int DelayPowerSupply = 100; //millisecondi ritardo per evitare overcurrent

//valori da mandase al servo (range 0..180)
int ServoDxSx;
int ServoHiLo;

int joyX;
int joyY;
int accelX;
int accelY;
int accelZ;
int buttons;
bool buttonC; // Il bit 1 rappresenta il pulsante C
bool buttonZ; // Il bit 0 rappresenta il pulsante Z
 

//------------------------
//filtri per "tranquillizzare" i motori
const int TapFiltri = 10;  // Profondità del filtro 
int index = 0;  //puntatore filtro

int BufferFilterHiLo[TapFiltri];   // Array per memorizzare le letture
int indexHiLo = 0;               // Indice per l'array
int totalHiLo = 0;               // Somma delle letture
int averageHiLo = 0;             // Media delle letture

int BufferFilterDxSx[TapFiltri];   // Array per memorizzare le letture
int indexDxSx = 0;               // Indice per l'array
int totalDxSx = 0;               // Somma delle letture
int averageDxSx = 0;             // Media delle letture







// constants won't change. Used here to set a pin number:
const int ledPin =  LED_BUILTIN;// the number of the LED pin

// Variables will change:
int ledState = LOW;             // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 250;           // interval at which to blink (milliseconds)

//---
void InitializeNunchuck(){
  //Wire.begin();
  //setup
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
//-------------------------


//--******************************************************************************************

void setup() {
   // set the digital pin as output:
   pinMode(ledPin, OUTPUT);
    
  //start serial connection
  Serial.begin(115200); // terminal
  Serial.println();
  Serial.println("Type Menu to enter the config menu.");
  Serial.print("Version ");Serial.println(Versione);
  Serial.println();

  Wire.begin();

  
  servo1.attach(9);  // Collegato al pin 9
  servo2.attach(10); // Collegato al pin 10

  Serial.println("CHART Wii Nenchuck - reset button to end");
  
  InitializeNunchuck();
  /*
  Wire.begin();
  //setup
  // Inizializza il Nunchuck99
  Wire.beginTransmission(0x52); // Indirizzo I2C del Nunchuck
  Wire.write(0xF0);
  Wire.write(0x55);
  Wire.endTransmission();

  Wire.beginTransmission(0x52);
  Wire.write(0xFB);
  Wire.write(0x00);
  Wire.endTransmission();
  */
}

//--******************************************************************************************

void(* resetFunc) (void) = 0; //declare reset function @ address 0

//-------------------------------------------
//-------------------------------------------
//-------------------------------------------
//-------------------------------------------
//-------------------------------------------
void loop3(){
 ReadNunChuck();
 
  delay(100);  

}
//---------------------------------------

void ReadNunChuck(){

  Wire.beginTransmission(0x52);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(0x52, 6);

  if (Wire.available() == 6) {
    joyX = Wire.read();
    joyY = Wire.read();
    accelX = (Wire.read() << 2) | ((Wire.peek() >> 6) & 3);
    accelY = (Wire.read() << 2) | ((Wire.peek() >> 4) & 3);
    accelZ = (Wire.read() << 2) | ((Wire.peek() >> 2) & 3);
    buttons = Wire.read(); // Legge il sesto byte

    // Decodifica i pulsanti
    buttonC = !(buttons & 0x02); // Il bit 1 rappresenta il pulsante C
    buttonZ = !(buttons & 0x01); // Il bit 0 rappresenta il pulsante Z

    if (debug){
      Serial.print("JoyX: "); Serial.print(joyX);
      Serial.print(" | JoyY: "); Serial.print(joyY);
      Serial.print(" | AccelX: "); Serial.print(accelX);
      Serial.print(" | AccelY: "); Serial.print(accelY);
      Serial.print(" | AccelZ: "); Serial.print(accelZ);
      Serial.print(" | Button C: "); Serial.print(buttonC ? "Pressed" : "Released");
      Serial.print(" | Button Z: "); Serial.println(buttonZ ? "Pressed" : "Released");

    }
  }


}
//-------------------------------------------
//-------------------------------------------
//-------------------------------------------
//-------------------------------------------
//-------------------------------------------

void loop() {

  ChiComandaDestraSinistra = true; // Mettere true per inclinazione , false per MiniMouse
  ChiComandaAltoBasso = true;  //  Mettere true per inclinazione , false per MiniMouse

  CicloClosedLoop();

  //--------------------------------------
  // BLINK MANAGEMENT EVERY 1 SECOND, TO SHOW THAT THE LOOP IS ACTIVE
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
    Serial.print(".");
  }
  //--------------------------------------
  // print the string when a newline arrives:
  if (stringComplete) {
    inputString.trim();
    Serial.println(inputString);
    //parse commands
    if ( (inputString.substring(0, 4) ) == "Menu" ) {
      Menu();
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

//*******************************************************

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//print the list of available commands
void TypeMenuList(void){
    debug=false;
    Serial.println();
    Serial.println("|************************|");
    Serial.println("|  ʘ‿ʘ   Menu   (◡_◡)   |");
      Serial.print("|  Ver. ");Serial.println(Versione);
    Serial.println("|************************|");
    Serial.println("  0 Reset");
    Serial.println("  1 Servo 1 a 0");
    Serial.println("  2 Servo 1 a 180");
    Serial.println("  3 Servo 2 a 0"); 
    Serial.println("  4 Servo 2 a 180");
    Serial.println("  5 Servo 1 e 2 loop");
    Serial.println("  6 I2c bus scanner");
    Serial.println("  7 Read Wii Nenchuck");
    Serial.println("  8 Test con Inclinazione ");
    Serial.println("  9 Test con MiniMouse");
    Serial.println(" 10 Action 10");
    Serial.println(" 99 Return to loop without reset");
}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void Menu() {

  //stay in the menu until you press 0
  while (!ExitFromMenu) {

    TypeMenuList();

    //clear the buffer
    while (Serial.available()) {
      Cestino = Serial.read();
    }

    Serial.println(" ");

    // here the waiting and LED blinking loop until there is a character on the serial
    while (!Serial.available()) {
      
      //--------------------------------------
      // FAST BLINK MANAGEMENT TO SHOW THAT THE MENU IS WAITING FOR COMMANDS
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval/4) {
        // save the last time you blinked the LED
        previousMillis = currentMillis;

        // if the LED is off turn it on and vice-versa:
        if (ledState == LOW) {
          ledState = HIGH;
        } else {
          ledState = LOW;
        }

        // set the LED with the ledState of the variable:
        digitalWrite(ledPin, ledState);
    }     
   }

   // a character has arrived
   int CmdMenu = Serial.parseInt();
   Serial.print("CmdMenu received ");Serial.println(CmdMenu);

   //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

   //process the received command 
	switch (CmdMenu){

      //-------------------------------------------------
      case 0:
        Serial.println("resetting in 1 sec");
        delay(1000);               // wait for a second
        resetFunc();  //call reset

        delay(100);
        Serial.println("Reset did not work");
      break;      //-------------------------------------------------
      case 1:
        //Action1();
        servo1.write(0);
      break;
      //-------------------------------------------------
      case 2:
        //Action2();
        servo1.write(180);
      break;
      //-------------------------------------------------
      case 3:
        //Action3();
        servo2.write(0);
      break;
      //-------------------------------------------------
      case 4:
        //Action4();
        servo2.write(180);
      break;
      //-------------------------------------------------
      case 5:
        Action5();
      break;
      //-------------------------------------------------
      case 6:
        Action6();
      break;
      //-------------------------------------------------
      case 7:
        Action7();
      break;
      //-------------------------------------------------
      case 8:
        Action8();
      break;
      //-------------------------------------------------
      case 9:
        Action9();
      break;
      //-------------------------------------------------
      case 10:
        Action10();
      break;
      //-------------------------------------------------
      case 99:
        Action99();
      break;
      //-------------------------------------------------
      default:
        Serial.println("Invalid command. Try again!");
    } //Serial.available
  } // ExitFromMenu
} // Menu

//*******************************************************

//--------------------------------------------------------------------------
//action n. 1
void Action1(){
  Serial.println("Action no. 1 selected");
  //insert your program lines here
  //insert your program lines here  
  delay(2000);
  Serial.println("Action terminated, returning to the menu'");
  delay(2000);
  
}
//--------------------------------------------------------------------------
//action n. 2
void Action2(){
  Serial.println("Action no. 2 selected");
  //insert your program lines here
  //insert your program lines here
  delay(2000);
  Serial.println("Action terminated, returning to the menu'");
  delay(2000);
}
//--------------------------------------------------------------------------
//action n. 3
void Action3(){
  Serial.println("Action no. 3 selected");
  //insert your program lines here
  //insert your program lines here
  delay(2000);
  Serial.println("Action terminated, returning to the menu'");
  delay(2000);
}
//--------------------------------------------------------------------------
//action n. 4
void Action4(){
  Serial.println("Action no. 4 selected");
  //insert your program lines here
  //insert your program lines here
  delay(2000);
  Serial.println("Action terminated, returning to the menu'");
  delay(2000);
}
//--------------------------------------------------------------------------
//action n. 5
void Action5(){
  Serial.println("Action no. 5 selected");
  for (int i=1; i<5; i++){
      Serial.print("Ciclo ");
      Serial.println(i);
      servo1.write(0);
        delay(DelayPowerSupply);
      servo2.write(0);
      delay(2000);
      servo1.write(180);
        delay(DelayPowerSupply);
      servo2.write(180);
      delay(2000);
  }
  Serial.println("Action terminated, returning to the menu'");
}
//--------------------------------------------------------------------------
//action n. 6
void Action6(){
  Serial.println("Action no. 6 selected");
  Serial.println("i2c bus scanner - reset button to exit");
  //insert your program lines here
  while(1){
    int nDevices = 0;

    Serial.println("Scanning...");

    for (byte address = 1; address < 127; ++address) {
      // The i2c_scanner uses the return value of
      // the Wire.endTransmission to see if
      // a device did acknowledge to the address.
      Wire.beginTransmission(address);
      byte error = Wire.endTransmission();

      if (error == 0) {
        Serial.print("I2C device found at address 0x");
        if (address < 16) {
          Serial.print("0");
        }
        Serial.print(address, HEX);
        Serial.println("  !");

        ++nDevices;
      } else if (error == 4) {
        Serial.print("Unknown error at address 0x");
        if (address < 16) {
          Serial.print("0");
        }
        Serial.println(address, HEX);
      }
    }
    if (nDevices == 0) {
      Serial.println("No I2C devices found\n");
    } else {
      Serial.println("done\n");
    }
    delay(2000); // Wait 5 seconds for next scan
  }
  
  //insert your program lines here
  delay(2000);
  Serial.println("Action terminated, returning to the menu'");
  delay(2000);
}
//--------------------------------------------------------------------------
//action n. 7
void Action7(){
  Serial.println("Action no. 7 selected");
  Serial.println("Read Wii Nenchuck - reset button to end");
  //insert your program lines here
  debug=true;
  //InitializeNunchuck();


  //int i=0;
  while(1){
    ReadNunChuck();

    delay(100);

  }

  //loop
  //insert your program lines here
  delay(2000);
  Serial.println("Action terminated, returning to the menu'");
  delay(2000);
}
//--------------------------------------------------------------------------
//action n. 8
void Action8(){
  Serial.println("Test Inclinazioni- reset button to exit");
  
  //seleziona inclinazioni
  ChiComandaDestraSinistra = true; // Mettere true per inclinazione , false per MiniMouse
  ChiComandaAltoBasso = true;  //  Mettere true per inclinazione , false per MiniMouse

  //InitializeNunchuck();

   //insert your program lines here
  while(1){
    CicloClosedLoop();
  }

  //insert your program lines here
  delay(2000);
  Serial.println("Action terminated, returning to the menu'");
  delay(2000);
}
//--------------------------------------------------------------------------
//action n. 9
void Action9(){
  Serial.println("Test MiniMouse- reset button to exit");
  
  //seleziona inclinazioni
  ChiComandaDestraSinistra = false; // Mettere true per inclinazione , false per MiniMouse
  ChiComandaAltoBasso = false;  //  Mettere true per inclinazione , false per MiniMouse

  //InitializeNunchuck();

   //insert your program lines here
  while(1){
    CicloClosedLoop();
  }

  //insert your program lines here
  delay(2000);
  Serial.println("Action terminated, returning to the menu'");
  delay(2000);
}
//--------------------------------------------------------------------------
//action n. 10
void Action10(){
  Serial.println("Action no. 10 selected");
  //insert your program lines here
  //insert your program lines here
  delay(2000);
  Serial.println("Action terminated, returning to the menu'");
  delay(2000);  
}
//--------------------------------------------------------------------------
//action n. 99
void Action99(){
  Serial.println("Action no. 99 selected");
  delay(2000);
  Serial.println("Returning to loop'");
  delay(2000);
  ExitFromMenu = true;  
}
//--------------------------------------------------------------------------
void CicloClosedLoop(){

  ReadNunChuck();

  //processo DestraSinistra DxSx
  if (ChiComandaDestraSinistra){
    Serial.print("DxSx Inclinazione ");
    ServoDxSx = map(accelX, ValInclinazioneSx, ValInclinazioneDx, ServoSx, ServoDx);
  } else {
    Serial.print("DxSx MiniMouse ");
    ServoDxSx = map(joyX, ValMouseSx, ValMouseDx, ServoSx, ServoDx);
  }
  Serial.print(" CalcDxSx:");Serial.print(ServoDxSx);
  //
  if (FiltroMotori){
    //applico un filtro per tranquillizzare il motore
    totalDxSx = totalDxSx - BufferFilterDxSx[index];        // Sottrai il valore più vecchio
    BufferFilterDxSx[index] = ServoDxSx;       // 
    totalDxSx = totalDxSx + BufferFilterDxSx[index];        // Aggiungi il nuovo valore
    averageDxSx = totalDxSx / TapFiltri;          // Calcola la media
    ServoDxSx = averageDxSx;
    Serial.print(" FiltDxSx:");Serial.print(ServoDxSx);
  }
  ServoDxSx = constrain(ServoDxSx, 0, 180);  //evita valori fuori range per il servo
  Serial.print(" ClampDxSx:");Serial.print(ServoDxSx);
  servo1.write(ServoDxSx);


  //processo AltoBasso HiLo
  if (ChiComandaAltoBasso){
    Serial.print("   |||  HiLo Inclinazione ");
    ServoHiLo = map(accelY, ValInclinazioneHi, ValInclinazioneLo, ServoHi, ServoLo);
  } else {
    Serial.print("   |||  HiLo MiniMouse ");
    ServoHiLo = map(joyY, ValMouseHi, ValMouseLo, ServoHi, ServoLo);
  }
  Serial.print(" CalcHiLo:");Serial.print(ServoHiLo);

  //
  if (FiltroMotori){
    //applico un filtro per tranquillizzare il motore
    totalHiLo = totalHiLo - BufferFilterHiLo[index];        // Sottrai il valore più vecchio
    BufferFilterHiLo[index] = ServoHiLo;       // 
    totalHiLo = totalHiLo + BufferFilterHiLo[index];        // Aggiungi il nuovo valore
    averageHiLo = totalHiLo / TapFiltri;          // Calcola la media
    ServoHiLo = averageHiLo;
    Serial.print(" FiltHiLo:");Serial.print(ServoHiLo);
  }
  ServoHiLo = constrain(ServoHiLo, 0, 180); //evita valori fuori range per il servo
  Serial.print(" ClampHiLo:");Serial.print(ServoHiLo);
  servo2.write(ServoHiLo);

  Serial.println();
  delay(TempoCicloAggiornamentoServo); //introduce un ritardo nell'aggiornamento dei servo msec


  //incrementa indice filtro
  index = index + 1;                      // Avanza l'indice
  if (index >= TapFiltri) {             // Se l'indice supera il numero di letture
    index = 0;                            // Resetta l'indice
  }
  
}
