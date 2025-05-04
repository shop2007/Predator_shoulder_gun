//*********************************************
// imposta modi di uso


//scelta di chi comanda i movimenti
bool ChiComandaDestraSinistra = true; // Mettere true per inclinazione , false per MiniMouse
bool ChiComandaAltoBasso = true;  //  Mettere true per inclinazione , false per MiniMouse

//Lettura AccelY
int ValInclinazioneHi = 360; //Alto 
int ValInclinazioneLo = 710; //Basso

//Lettura AccelX
int ValInclinazioneSx = 300; //sinistra
int ValInclinazioneDx = 700; //destra

//Lettura JoyY
int ValMouseHi = 10; //Alto
int ValMouseLo = 245; //Basso

//Lettura JoyX
int ValMouseDx = 245; //destra
int ValMouseSx = 10; //sinistra

//Valori Servo AltoBasso
int ServoHi = 170; //Alto
int ServoLo = 10; //Basso

//Valori Sefvo DestraSinistra
int ServoSx = 5; //Sinistra
int ServoDx = 160; //Destra

bool FiltroMotori = true; //true filtro presente   false assente
int TempoCicloAggiornamentoServo = 50; //introduce un ritardo nell'aggiornamento dei servo msec
