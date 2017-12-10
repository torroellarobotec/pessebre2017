

// include the library code:
#include <LiquidCrystal.h>
#include <DMXSerial.h>
#include <Wire.h>
#include "RTClib.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

RTC_DS3231 rtc;
uint8_t OK_PIN = 53;
uint8_t DOWN_PIN = 51;
uint8_t BACK_PIN = 49;

uint8_t R1 = 20;
uint8_t R2 = 22;
uint8_t R3 = 24;
uint8_t R4 = 26;
uint8_t R5 = 28;
uint8_t R6 = 30;
uint8_t R7 = 32;
uint8_t R8 = 34;


int menu = 1;
int menu1 = 1;
int menu2 = 1;
int menu3 = 1;
int menuglobal = 1;
int menuvalue = 0;
int segons_abans = 0;
int premut_ok = 0;
int premut_down = 0;
int premut_back = 0;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 39, d4 = 41, d5 = 43, d6 = 45, d7 = 47;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void arrancaDmx()
{
  for (int y = 1; y < 64; y++)
  {
    DMXSerial.write(y, 0);
    //setChannelRGB (y+32, a);

  }
}

void setup() {
  pinMode(OK_PIN, INPUT);
  pinMode(DOWN_PIN, INPUT);
  pinMode(BACK_PIN, INPUT);

  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(R5, OUTPUT);
  pinMode(R6, OUTPUT);
  pinMode(R7, OUTPUT);
  pinMode(R8, OUTPUT);

  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(R3, LOW);
  digitalWrite(R4, LOW);
  digitalWrite(R5, LOW);
  digitalWrite(R6, LOW);
  digitalWrite(R7, LOW);
  digitalWrite(R8, LOW);

  //Arrancar amb +5 el relotge
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);

  if (!rtc.begin()) {

    while (1);
  }
  if (rtc.lostPower()) {

    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  DMXSerial.init(DMXController);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  // lcd.print("  Menu activat:");
  arrancaDmx();

  mySoftwareSerial.begin(9600);
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.volume(20);  //Set volume value. From 0 to 30

}

void pintaMenu()
{
  lcd.setCursor(0, 1);

  if (menuglobal == 1)
  {
    if (menu == 1) lcd.print("Reles             ");
    if (menu == 2) lcd.print("DMX               ");
    if (menu == 3) lcd.print("Musica            ");
  }
  if (menuglobal == 2)
  {
    if (menu == 1)
    {
      lcd.print("R-> " + String(menu1, DEC) + "         ");
    }
    if (menu == 2)
    {
      lcd.print("DMX-> " + String(menu2, DEC) + "       ");
    }
    if (menu == 3)
    {
      lcd.print("Mus-> " + String(menu3, DEC) + "        ");
      myDFPlayer.stop();
      menuvalue=0;
    }
  }
  if (menuglobal == 3)
  {
    if (menu == 1)
    {
      lcd.print("R-> " + String(menu1, DEC) + " -> " + String(menuvalue, DEC) + "  ");
    }
    if (menu == 2)
    {
      lcd.print("DMX-> " + String(menu2, DEC) + " ->" + String(menuvalue, DEC) + "  ");
    }
    if (menu == 3)
    {
      String cad="";
      if (menuvalue==0) cad="STOP";
      if (menuvalue==1) cad ="PLAY";
      lcd.print("Mus-> " + String(menu3, DEC) + " -> " + cad + " ");
    }
  }
}
void enviaValor()
{
  if (menu == 1)
  {
    if (menu1 == 1) digitalWrite(R1, menuvalue);
    if (menu1 == 2) digitalWrite(R2, menuvalue);
    if (menu1 == 3) digitalWrite(R3, menuvalue);
    if (menu1 == 4) digitalWrite(R4, menuvalue);
    if (menu1 == 5) digitalWrite(R5, menuvalue);
    if (menu1 == 6) digitalWrite(R6, menuvalue);
    if (menu1 == 7) digitalWrite(R7, menuvalue);
    if (menu1 == 8) digitalWrite(R8, menuvalue);
  }
  if (menu == 2)

  {
    DMXSerial.write(menu2, menuvalue);
  }
  if (menu == 3)
  {
    if (menuvalue==0)
    {
      myDFPlayer.stop();  //Play the  mp3
    }
    else
    {
      myDFPlayer.play(menu3);  //Play the  mp3
    }
    
  }
}
void pintaHora()
{
  DateTime now = rtc.now();

  int hora = (int)now.hour();
  int mi = (int)now.minute();;
  int se = (int)now.second();
  if (se != segons_abans)
  {
    segons_abans = se;
    String cadh = '0' + String(hora, DEC);
    String cadm = '0' + String(mi, DEC);
    String cads = '0' + String(se, DEC);
    cadh = cadh.substring(cadh.length() - 2);
    cadm = cadm.substring(cadm.length() - 2);
    cads = cads.substring(cads.length() - 2);
    lcd.setCursor(0, 0);
    lcd.print(cadh + ':' + cadm + ':' + cads + "    v0.1");
  }
}
void boto()
{

  if (digitalRead(DOWN_PIN) == LOW)
  {
    if (premut_down == 0)
    {
      delay(100);
      if (digitalRead(DOWN_PIN) == LOW)
      {
        //Serial.println("down");

        premut_down = 1;
        if (menuglobal == 1)
        {
          menu++;
          if (menu > 3) menu = 1;
        }
        if (menuglobal == 2)
        {
          if (menu == 1)
          {
            menu1++;

            if (menu1 > 8) menu1 = 1;
          }
          if (menu == 2)
          {
            menu2++;
            menuvalue=0;
            if (menu2 > 60) menu2 = 1;
          }
          if (menu==3)
          {
            menu3++;
            if (menu3>5) menu3=1;
            menuvalue=1;
          }
        }
        if (menuglobal == 3)
        {
          if (menu == 1)
          {
            if (menuvalue == 0)

            {
              menuvalue = 1;
            }
            else
            {
              menuvalue = 0;
            }
            enviaValor();
          }
          if (menu == 2)
          {
            menuvalue = menuvalue + 25;
            if (menuvalue > 255)
            {
              menuvalue = 0;
            }
            enviaValor();
          }
          if (menu == 3)
          {
            if (menuvalue == 0)

            {
              menuvalue = 1;
            }
            else
            {
              menuvalue = 0;
            }
            enviaValor();
          }
        }
        // Serial.println(String(menu,DEC) + "-" + String(menuglobal,DEC) + "-" + String(menu1,DEC) + "-" + String(menu2,DEC));
      }
    }
  }
  else
  {
    premut_down = 0;
  }

  if (digitalRead(BACK_PIN) == LOW)
  {
    if (premut_back == 0)
    {
      delay(100);
      if (digitalRead(BACK_PIN) == LOW)
      {
        //Serial.println("Back");

        premut_back = 1;
        menuglobal = menuglobal - 1;
        menuvalue = 0;
        if (menuglobal < 1) menuglobal = 1;
        // Serial.println(String(menu,DEC) + "-" + String(menuglobal,DEC) + "-" + String(menu1,DEC) + "-" + String(menu2,DEC));
      }
    }
  }
  else
  {
    premut_back = 0;
  }

  if (digitalRead(OK_PIN) == LOW)
  {
    if (premut_ok == 0)
    {
      delay(100);
      if (digitalRead(OK_PIN) == LOW)
      {
        //Serial.println("Ok");

        premut_ok = 1;
        menuglobal++;
        if (menuglobal > 3)
        {
          menuglobal = 3;
          
        }
        if (menuglobal==3) 
        {
           menuvalue=1;
          //if (menu==2)  menuvalue=0;
        }
       
        //Serial.println(String(menu,DEC) + "-" + String(menuglobal,DEC) + "-" + String(menu1,DEC) + "-" + String(menu2,DEC));
        enviaValor();
      }
    }
  }
  else
  {
    premut_ok = 0;
  }
}
void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):


  pintaHora();
  boto();
  pintaMenu();
  //  int a=0;
  //  int x=32;
  //  for (int y=1; y<62;y++)
  //{
  //  DMXSerial.write (y, a);
  //  
  //  delay(50);
  //}
  //
  //
  //delay(300);
  //a=255;
  //
  //for (int y=1; y<62;y++)
  //{
  //  DMXSerial.write (y,a);
  //  
  //  delay(50);
  //}

  delay(200);
}
