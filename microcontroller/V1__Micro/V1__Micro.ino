// Bodenfeuchtigkeitssensor liegt auf A0
//I2C auf A4 (SDA) und A5 (SCL)


#include <Wire.h>

//Libary from: https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
#include <LiquidCrystal_I2C.h>


//GieÃŸpin ist 11
#define GIESPIN 11

//Feuchtigkeitssensor 
#define MOISTURE_A A3

LiquidCrystal_I2C lcd(0x27, 16, 2, LCD_5x8DOTS);  // Set the LCD I2C address



void setup()
{
  Serial.begin(9600);

  lcd.begin();
  digitalWrite(GIESPIN, LOW);


}
int messung = 20;
int giesTest;
int minFeuchtigkeit = 200;


unsigned long loopcounter;
unsigned long timer = 10000; //10 Sekunden //24 * 60 * 60000; //1 Tag
unsigned long loopMillis;
unsigned long giesTimer;
unsigned long millisMessung;

void loop() {
  programm();
}

void programm() {

  measurement();
  printLCD();
  if (giesTest == 1) {
    if (messung <= minFeuchtigkeit) {
      giesTimer = millis() + 1500;
      printLCD();
      giesTest = 0;
    }
  } 
  
  if (millis() <= giesTimer) {
    pouring(1);
  } else {
    pouring(0);
  }

  if (millis() >= timer) {
    giesTest = 1;
    timer += timer;
  }
}


String line1;
String line1old;
String line2;
String line2old;

void printLCD() {
  line1 = "Bodenfeuchtigkeit";
  line2 = String(messung);

  if (line1 != line1old || line2 != line2old) {
    // lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
    line1old = line1;
    line2old = line2;
  }
}

void pouring(int on) {

  if (on == 1) {
    digitalWrite(GIESPIN, HIGH);

  } else {
    digitalWrite(GIESPIN, LOW);
  }


}

void measurement(){
  if (millisMessung <= millis()) {
    messung = analogRead(MOISTURE_A);
    millisMessung = millis()+ 200;
  }  
}


