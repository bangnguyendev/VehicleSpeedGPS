/*
  DJI Naza (v1, v1 Lite, V2) data decoder library example
  (c) Pawelsky 20141109
  Not for commercial use

  Refer to naza_decoder_wiring.jpg diagram for proper connection
*/

#include "NazaDecoderLib.h"
#include <LiquidCrystal_I2C.h>
#include "THUVIEN_LCD.h"
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

long int numsat = 0, k;
int spd = 0;
int b = 0;

#ifndef ATTITUDE_SENSING_DISABLED
uint32_t currTime, attiTime;
#endif

void setup()
{

  Serial.begin(115200);

  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.createChar(0, LT);
  lcd.createChar(1, UB);
  lcd.createChar(2, RT);
  lcd.createChar(3, LL);
  lcd.createChar(4, LB);
  lcd.createChar(5, LR);
  lcd.createChar(6, MB);
  lcd.createChar(7, block);

  // sets the LCD's rows and colums:
  lcd.setCursor(7, 1); lcd.print("km/h");

  printDigits(0, 12, 0); //BAO SO

  lcd.setCursor(19, 0); lcd.print("F"); //BAO XANG
  lcd.setCursor(19, 3); lcd.print("E");
  lcd.setCursor(16, 0); lcd.write(3); lcd.write(7); lcd.write(7);
  lcd.setCursor(17, 1); lcd.write(3); lcd.write(7);
  lcd.setCursor(18, 2); lcd.write(7);
  lcd.setCursor(18, 3); lcd.write(3);
  lcd.setCursor(10, 3); lcd.write(3); lcd.print("DU"); lcd.write(4); lcd.print("BA"); lcd.write(5);

  lcd.setCursor(1, 2);
  lcd.print("Alt: ");
  lcd.print(NazaDecoder.getGpsAlt(), 0);
  lcd.print("m  ");
  lcd.setCursor(1, 3);
  lcd.print("Sat: ");
  lcd.print(NazaDecoder.getNumSat());
  lcd.print("  ");

  for (int i = 0; i < 5; i++)
  {
    printDigits(i, 0, 0);
    printDigits(i, 4, 0);
    delay(100);
  }
  for (int i = 5; i < 10; i++)
  {
    printDigits(i, 0, 0);
    printDigits(i, 4, 0);
    delay(100);
  }

}

void custom0(int x, int y) {

  lcd.setCursor(x, y);
  lcd.write(0);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(x, y + 1);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
}

void custom1(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(1);
  lcd.write(2);
  lcd.print(" ");
  lcd.setCursor(x, y + 1);
  lcd.write(4);
  lcd.write(7);
  lcd.write(4);
}

void custom2(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(6);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(x, y + 1);
  lcd.write(3);
  lcd.write(4);
  lcd.write(4);
}

void custom3(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(6);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(x, y + 1);
  lcd.write(4);
  lcd.write(4);
  lcd.write(5);
}

void custom4(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(3);
  lcd.write(4);
  lcd.write(7);
  lcd.setCursor(x, y + 1);
  lcd.print(" ");
  lcd.print(" ");
  lcd.write(7);
}

void custom5(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(3);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(x, y + 1);
  lcd.write(4);
  lcd.write(4);
  lcd.write(5);
}

void custom6(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(0);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(x, y + 1);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
}

void custom7(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(1);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(x, y + 1);
  lcd.print(" ");
  lcd.print(" ");
  lcd.write(7);
}

void custom8(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(0);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(x, y + 1);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
}

void custom9(int x, int y) {
  lcd.setCursor(x, y);
  lcd.write(0);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(x, y + 1);
  lcd.print(" ");
  lcd.write(4);
  lcd.write(5);

}
void printDigits(int digits, int x, int y) {

  switch (digits) {
    case 0:
      custom0(x, y);
      break;
    case 1:
      custom1(x, y);
      break;
    case 2:
      custom2(x, y);
      break;
    case 3:
      custom3(x, y);
      break;
    case 4:
      custom4(x, y);
      break;
    case 5:
      custom5(x, y);
      break;
    case 6:
      custom6(x, y);
      break;
    case 7:
      custom7(x, y);
      break;
    case 8:
      custom8(x, y);
      break;
    case 9:
      custom9(x, y);
      break;

  }
}
void loop()
{

  if (Serial.available())
  {
    uint8_t decodedMessage = NazaDecoder.decode(Serial.read());
    b = random(0, 4);
    switch (decodedMessage)
    {
      case NAZA_MESSAGE_GPS:
        numsat = NazaDecoder.getNumSat();
        spd = NazaDecoder.getSpeed() * 3.6 * 10 * 1.075;

        printDigits(spd / 100 % 10, 0, 0);
        printDigits(spd / 10 % 10, 4, 0);
        lcd.setCursor(7, 1);
        lcd.print("km/h");

        lcd.setCursor(1, 2);
        lcd.print("Alt: ");
        lcd.print(NazaDecoder.getGpsAlt(), 0);
        lcd.print("m   ");
        lcd.setCursor(1, 3);
        lcd.print("Sat: ");
        lcd.print(NazaDecoder.getNumSat());
        lcd.print("   ");

        printDigits(b, 12, 0); //BAO SO

        lcd.setCursor(19, 0); lcd.print("F"); //BAO XANG
        lcd.setCursor(19, 3); lcd.print("E");
        lcd.setCursor(16, 0); lcd.write(3); lcd.write(7); lcd.write(7);
        lcd.setCursor(17, 1); lcd.write(3); lcd.write(7);
        lcd.setCursor(18, 2); lcd.write(7);
        lcd.setCursor(18, 3); lcd.write(3);
        lcd.setCursor(10, 3); lcd.write(3); lcd.print("DU"); lcd.write(4); lcd.print("BA"); lcd.write(5);

        break;

      case NAZA_MESSAGE_COMPASS:
        // Serial.print("Heading: "); Serial.println(NazaDecoder.getHeadingNc(), 2);
        break;
    }
  }



#ifndef ATTITUDE_SENSING_DISABLED
  currTime = micros();

  // Display attitude at 5Hz rate so every 200000 microseconds
  if (attiTime < currTime)
  {
    attiTime = currTime + 200000;
    // Serial.print("Pitch: "); Serial.print(NazaDecoder.getPitch());
    // Serial.print(", Roll: "); Serial.println(NazaDecoder.getRoll());
  }
#endif

}
