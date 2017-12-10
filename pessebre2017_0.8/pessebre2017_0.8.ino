// Pessebre 2017 v0.8

// include the library code:
#include <LiquidCrystal.h>
#include <DMXSerial.h>
#include <Wire.h>
#include "RTClib.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

String versio = "v0.9";
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

const int Estats = 22;
const int u = 250; //valor màxim llum  //2
const int d = 0; //valor apagat  //1
const int b = 75;
const int m = 120; //4
const int o = 200; //5
const int x = 1000 ;//No fer res en el llum  //3
int estatActual = 0;
int comptaSegons = 0;

int MatriuEstats[Estats][60] = {


  // 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
  //31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60

  { d, d, d, d, d, d, d, u, d, m, u, d, m, u, d, m, d, m, m, m, m, u, m, m, m, m, m, m, u, m, //de l'1 al 3o
    d, d, d, d, d, d, d, d, d, d, d, d, d, u, o, u, u, m, d, u, d, d, d, d, d, d, d, d, d, d //del 31 al 6o
  },
  //Nit
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, d, o, o, o, o, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, b, x, x, x, m, d, u, m, x, m, x, x, b, x, x, b, x, x, b, b
  },
  //medes on
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, d, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, d, x, x, x, m, d, u, m, x, m, x, x, d, x, x, d, u, x, d, d
  },
  //medes off
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, d, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, b, x, x, x, m, d, u, m, x, m, x, x, b, x, x, b, x, x, b, b
  },
  //Santa Caterina
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, d, o, o, o, b, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, d, u, x, x, m, d, u, m, x, m, x, x, d, x, x, d, x, x, d, d
  },
  //nit
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, d, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, b, x, x, x, m, d, u, m, x, m, x, x, b, x, x, b, x, x, b, b
  },
  //Naixement
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, d, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, d, x, x, x, m, d, u, m, x, b, x, x, d, x, x, d, x, u, d, d
  },
  //nit
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, d, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, b, x, x, x, m, d, u, m, x, m, x, x, b, x, x, b, x, x, b, b
  },
  //angel
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, d, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, d, x, x, x, m, d, u, m, u, m, x, x, d, x, x, d, x, x, d, d
  },
  //nit
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, d, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, b, x, x, x, m, d, u, m, x, m, x, x, b, x, x, b, x, x, b, b
  },
  //creu
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, u, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, d, x, x, x, m, d, u, m, x, m, x, x, d, x, x, d, x, x, d, d
  },
  //nit
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, x, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, b, x, x, x, m, d, u, m, x, m, x, x, b, x, x, b, x, x, b, b
  },
  //musics
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, x, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, d, x, x, x, m, d, u, m, x, m, x, x, d, x, u, d, x, x, d, d
  },
  //nit
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, x, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, b, x, x, x, m, d, u, m, x, m, x, x, b, x, x, b, x, x, b, b
  },
  //ovelles
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, x, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, u, d, x, x, x, m, d, u, m, x, m, x, x, d, x, x, d, x, x, d, d
  },
  //nit
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, x, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, b, x, x, x, m, d, u, m, x, m, x, x, b, x, x, b, x, x, b, b
  },
  //cascada
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, x, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, d, x, x, u, m, d, u, m, x, m, x, x, d, x, x, d, x, x, d, d
  },
  //nit
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, x, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, b, x, x, x, m, d, u, m, x, m, x, x, b, x, x, b, x, x, b, b
  },
  //castell
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, x, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, d, x, x, x, m, d, u, m, x, m, x, x, d, u, x, d, x, x, d, d
  },
  //nit
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, x, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, b, x, x, x, m, d, u, m, x, m, x, x, b, x, x, b, x, x, b, b
  },
  //infern
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, x, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, b, x, x, x, m, d, u, m, x, m, x, x, b, x, x, b, x, x, b, b
  },
  //nit
  { x, x, x, x, x, x, x, m, m, x, m, m, x, m, m, x, x, o, o, o, x, u, o, o, o, o, o, o, x, m,
    x, x, x, x, x, x, x, x, x, x, b, x, x, x, m, d, u, m, x, m, x, x, b, x, x, b, x, x, b, b
  }
  // 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30
  //31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60
};

const int intervalMinuts = 10;

int MatriuTemps[Estats] = {
  0,
  300,  //Nit
  314, 328, //Medes
  342, 356, //Sta. Caterina
  370, 384, //Naixement
  398, 412, //Angel
  426, 440, //Creu
  454, 468, //Musics
  482, 496, //Ovelles
  510, 524, //cascada
  538, 556, // Castell
  566, 580  //Infern
};






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
  if (intervalMinuts == 3)
  {
    MatriuTemps[0] = 0;
    MatriuTemps[1] = 30;
    MatriuTemps[2] = 35;
    MatriuTemps[3] = 45;
    MatriuTemps[4] = 50;
    MatriuTemps[5] = 60;
    MatriuTemps[6] = 65;
    MatriuTemps[7] = 75;
    MatriuTemps[8] = 80;
    MatriuTemps[9] = 90;
    MatriuTemps[10] = 95;
    MatriuTemps[11] = 105;
    MatriuTemps[12] = 110;
    MatriuTemps[13] = 115;
    MatriuTemps[14] = 120;
    MatriuTemps[15] = 125;
    MatriuTemps[16] = 130;
    MatriuTemps[17] = 135;
    MatriuTemps[18] = 140;
    MatriuTemps[19] = 145;
    MatriuTemps[20] = 150;
    MatriuTemps[21] = 155;


  }

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
  estatActual = 0;
  resetDmx();
  executaEstat();
  lcd.setCursor(5, 1);
  lcd.print("Reset");
}

void musica()
{

  //myDFPlayer.stop();  //Play the  mp3

  myDFPlayer.play(1);
}
void pintaMenu()
{

}
void enviaValor()
{

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
    lcd.print(cadh + ':' + cadm + ':' + cads + "    " + versio);
  }
  if (mi >= intervalMinuts)
  {
    int factor = mi / intervalMinuts;
    mi = mi - factor * intervalMinuts;
  }

  comptaSegons = mi * 60 + se;
  lcd.setCursor(0, 1);
  lcd.print(comptaSegons);
  lcd.print("  ");
}
void boto()
{


}
void resetDmx()
{
  for (int y = 1; y < 64; y++)
  {
    DMXSerial.write(y, 0);
    //setChannelRGB (y+32, a);

  }

}
void executaEstat()
{
  for (int i = 0; i < 61; i++)
  {


    int v = MatriuEstats[estatActual] [i];

    if (v != 1000)
    {
      if ((v >= 0) && (v < 256))
      {
        DMXSerial.write(i + 1, v);
        lcd.setCursor(5, 1);
        lcd.print(v);
        delay(25);
      }

    }



  }
  if (estatActual == 0)
  {

    //encen llums si estan apagats
   
    
    digitalWrite(R2, LOW);
    digitalWrite(R1, LOW);

    musica();

  }
  if (estatActual == 1)  //estat = 1 és la nit
  {
    //apaga llums

   
    digitalWrite(R2, HIGH);
    digitalWrite(R1, HIGH);
    
  }

  if (estatActual==20)
  {
    digitalWrite(R3, HIGH);
  }
  if (estatActual==21)
  {
    digitalWrite(R3, LOW);
  }
}
void resetEstat()
{
  estatActual = 0;
  executaEstat();

  estatActual++;
  lcd.setCursor(5, 1);
  lcd.print(String(estatActual, DEC) + " a " + String(MatriuTemps[estatActual], DEC) + " s.");

}
void miraEstat()
{
  int finalitza = 0;
  if (estatActual < Estats)
  {
    finalitza = MatriuTemps[estatActual];
  }
  else
  {
    finalitza = 0;
  }
  if (comptaSegons == finalitza)
  {
    executaEstat();
    estatActual++;
    finalitza = MatriuTemps[estatActual];
    if (estatActual == Estats + 1)
    {
      resetEstat();
      finalitza = MatriuTemps[estatActual];
    }
    lcd.setCursor(5, 1);
    lcd.print(String(estatActual, DEC) + " a " + String(finalitza , DEC) + " s.");
  }
}
void loop() {

  pintaHora();



  miraEstat();

  delay(200);
}
