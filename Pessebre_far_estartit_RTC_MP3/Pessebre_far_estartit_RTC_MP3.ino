// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

#include <SPI.h>

//Add the SdFat Libraries
#include <SdFat.h>
#include <FreeStack.h>

//and the MP3 Shield Library
#include <SFEMP3Shield.h>



RTC_DS3231 rtc;

SdFat sd;
SFEMP3Shield MP3player;
int tocat = 0;


void setup () {


  Serial.begin(9600);
  Serial.println("Inici");

  pinMode(10, OUTPUT);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
Serial.println("Inici RTC1");
 if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  uint8_t result; //result code from some function as to be tested at later time.
// rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
Serial.println("Inici RTC");


  //Initialize the SdCard.
  if (!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
  // depending upon your SdCard environment, SPI_HAVE_SPEED may work better.
  if (!sd.chdir("/")) sd.errorHalt("sd.chdir");
Serial.println("Inici SD");

  //Initialize the MP3 Player Shield
  result = MP3player.begin();
  tocaMusica(15);
}
void tocaMusica (int track)
{
  uint8_t result;
#if USE_MULTIPLE_CARDS
  sd.chvol(); // assign desired sdcard's volume.
#endif
  //tell the MP3 Shield to play a track
  MP3player.setVolume(0, 0); // commit new volume
  result = MP3player.playTrack(track);
}
void miraHora()
{
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  if (now.minute() == 0)
  {
    if (tocat == 0)
    {
      tocaMusica(16);
      tocat = 1;
      Serial.println("punt");
      delay(7000);
      int hora = now.hour();
      if (hora > 12)
      {
        hora = hora - 12;
      }
      tocaMusica(hora);
      delay(1.5 * hora * 1000);
    }
  }
  else
  {
    if (now.minute() == 15)
    {
      if (tocat == 0)
      {
        tocaMusica(13);
        tocat = 1;
        Serial.println("un");
        delay(7000);
      }
    }

    else
    {
      if (now.minute() == 30)
      {
        if (tocat == 0)
        {
          tocaMusica(14);
          tocat = 1;
          Serial.println("dos");
          delay(7000);
        }
      }

      else
      {
        if (now.minute() == 45)
        {
          if (tocat == 0)
          {
            tocaMusica(15);
            tocat = 1;
            Serial.println("tres");
            delay(7000);
          }
        }
        else
        {
          tocat = 0;
        }
      }



    }
  }
}


void loop () {
  miraHora();

  digitalWrite(10, HIGH);
  delay(200);
  digitalWrite(10, LOW);
  delay(800);
  digitalWrite(10, HIGH);
  delay(200);
  digitalWrite(10, LOW);
  delay(800);
  digitalWrite(10, HIGH);
  delay(200);
  digitalWrite(10, LOW);
  delay(800);
  digitalWrite(10, HIGH);
  delay(200);
  digitalWrite(10, LOW);

  delay(10000);
}
