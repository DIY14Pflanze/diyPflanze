

// Bodenfeuchtigkeitssensor liegt auf A0
//I2C auf A4 (SDA) und A5 (SCL)

//fuer i2c
#include <Wire.h>

//Libary from: https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
#include <LiquidCrystal_I2C.h>

#include <Bounce2.h>

//Pindefinition fuer Ardoino_Micoro
//Giespin
#define GIESPIN 10
//Feuchtigkeitssensor 
#define MOISTURE_A A3  //Analog
#define MOUSTURE_D A4  //Digial
//PhotoSensor
#define PHOTO_PIN A5
//ModeTaster
#define MODE 6    //Dispaly An und Mode durchschalten 
//Manuelles Gießen
#define MAN_GIES 7
//Anzahl der Modes die im Display angezeigt werden.
#define ANZAHL_MODE 8

//Definition Funktionen
LiquidCrystal_I2C lcd(0x27, 16, 2, LCD_5x8DOTS);  // Set the LCD I2C address
Bounce tAnzeige = Bounce();
Bounce tManGiesen = Bounce();

void programm(void);
void messung (void);
void modeTaster(void);
void giesTaster(void);
void testGiesen(void);

void giesen(boolean wasser);
void printLCD(int mode);


void setup(){
 Serial.begin(9600);
 lcd.begin();
 
 pinMode(GIESPIN, OUTPUT);
 pinMode(MODE, INPUT_PULLUP);      //Taster fuer Anzeige
 pinMode(MAN_GIES, INPUT_PULLUP);  //Taster fuer mauelles Giesen
 
 tAnzeige.attach(MODE);
 tAnzeige.interval(100);
 tManGiesen.attach(MAN_GIES);
 tManGiesen.interval(100);
 
   
}
//Variablen
int giesInterval = 5000;
int messInterval = 10000; //spaeter alle 4 std
int minFeuchtigkeit = 600;
int minHelligkeit = 200;
int displayTimeout = 30000;
boolean helligkeitOff = true;      //deaktivert den Helligkeitssensor --> giest immer wenn die minFeuchtigkeit unterschirtten wird egal welche Helligkeit.

boolean backlight_OFF = false;

int aktFeuchtigkeit = 0;
int aktHelligkeit = 0;
int mode=1;
boolean neueMesswerte = false;
boolean startGiesIntervall = true;  //Gibt an, ob dies der erste durchlauf in einem Giesintervall ist.
unsigned long prevMessung = 0;
unsigned long startIntervall = 0;
unsigned long prevDisplayTimeout = 0;
//Variablen

void loop(){

programm();
}

void programm(){

messung();
modeTaster();
giesTaster();

testGiesen();
printLCD(mode);
}


void testGiesen(){
//Neues Messergebniss ist vorhanden.
  if (neueMesswerte){
     
       //Messwerte Zeigen, es muss gegossen werden und es ist nicht Nacht.
       //Muss gegossen werden aber es ist nicht Hell genug, so wird neuer Messwert nicht zurückgesetzt,
       //sodass diese Abfrage solange wiederholt wird bis die Helligkeitsbedingung erfüllt ist.
       //helligkeitOff ueberbrueckt den Helligkeitssensor, wenn dies gewuenscht ist.
       if (aktFeuchtigkeit >= minFeuchtigkeit && (aktHelligkeit >= minHelligkeit || helligkeitOff)){  
           //zu beinn jedes Giesintervalls muss der Startwert gesetzt werden
         giesen(true);
         
           if (startGiesIntervall){
             startIntervall = millis();
             startGiesIntervall = false;
           }
           //Stoppt giesen wenn giesIntervall erreicht und qutiert den Messwert als bearbeitet.      
           if((millis()- startIntervall)>=giesInterval){
             giesen(false);
             neueMesswerte = false;
             startGiesIntervall = true;
           }
           
           
         } 
         //Messwert Zeigt, es muss nicht geossen werden
         else if(aktFeuchtigkeit < minFeuchtigkeit){
           neueMesswerte = false;
           giesen(false); 
         }
                
  }

}


//liest Tastendruck, erneuert die Messwerte und übergibt die Werte das LCD-Dispaly
void modeTaster(){
 
  tAnzeige.update();

  if (tAnzeige.fell() ){
    //zuruecksezten des DisplyTimer, damit das Display an beleibt wenn man die taste drueckt.
    prevDisplayTimeout = millis();
    if (backlight_OFF){
      lcd.backlight();
      backlight_OFF = false;
    }
    else{
      lcd.backlight();
      mode = (mode + 1) % ANZAHL_MODE;
    }
  }
}
//giest solange, solange der Taster gedrückt wird.
void giesTaster(){
 tManGiesen.update();
   if (tManGiesen.fell() ){
     giesen(true);
   }
   else if (tManGiesen.rose() ){
     giesen(false); 
   }
}

void giesen(boolean wasser){
  //Starten und Stoppt die Pumpe.
  if (wasser == true){
   digitalWrite(GIESPIN, HIGH);
   //lcd.noBacklight();
  }
  else{
   digitalWrite(GIESPIN, LOW); 
   //lcd.backlight();
  }
}

void messung (){
  //Liest die Sensoren aus und schreibt werte in globale Variablen.
  
  if ((millis()-prevMessung) >= messInterval){
    aktFeuchtigkeit = analogRead(MOISTURE_A);
    aktHelligkeit = analogRead(PHOTO_PIN);
    neueMesswerte = true;    //zeigt an, dass neue Messwerte vorhanden sind
    prevMessung = millis();
  }
}

String line1;
String line1old;
String line2;
String line2old;

void printLCD(int mode) {
  switch (mode){
  case 0:
    lcd.noBacklight();
    line1 = "";
    line2 = "";
    break;
  case 1:
    line1 = "Bodenfeuchte";
    line2 = String(aktFeuchtigkeit);
    break;
  case 2:
    line1 = "Helligkeit";
    line2 = String(aktHelligkeit);
    break;
  case 3:
    line1 = "min_Bodenfeuchte";
    line2 = String(minFeuchtigkeit);
    break;
  case 4:
    line1 = "min_Helligkeit";
    line2 = String(minHelligkeit);
    break;
  case 5:
    line1 = "GiesInterval";
    line2 = String(giesInterval);
    break;
  case 6:
    line1 = "MessInterval";
    line2 = String(messInterval);
    break;
  case 7:
    line1 = "Helligkeitssensor";
    if(helligkeitOff){
      line2 = "Deaktiviert";
    }
    else {
       line2 = "Aktiviert";
    }
    break;
}
  
  if (line1 != line1old || line2 != line2old) {
    //lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
    line1old = line1;
    line2old = line2;
  }
  
  if ((millis()-prevDisplayTimeout)>= displayTimeout){
    lcd.noBacklight();
    backlight_OFF = true;
    prevDisplayTimeout = millis();
  }
}













