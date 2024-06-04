/*
 *#######################################################
 * Beschreibung.: Funktion für das 7Segment LED-Display
 * Dateiname....: lcddisplay.cpp
 *#######################################################
*/

#include <Arduino.h>
#include <bits/stdc++.h>
#include <LiquidCrystal_I2C.h>

#define COLUMS    16 //LCD columns
#define ROWS      2  //LCD rows
#define L_CHANNEL A0 //left audio input pin
#define R_CHANNEL A1 //right audio input pin
LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

void fctLcdText(std::string lcdtext, int spalte, int zeile)
{
  //  LCD Test  -----------------
  while (lcd.begin(COLUMS, ROWS, LCD_5x8DOTS) != 1) //colums, rows, characters size
  {
    Serial.println(F("PCF8574 is not connected or lcd pins declaration is wrong. Only pins numbers: 4,5,6,16,11,12,13,14 are legal."));
    delay(5000);   
  }

  //lcd.print(F("PCF8574 is OK...")); //(F()) saves string to flash & keeps dynamic memory free
  //delay(2000);
  //lcd.setCursor(1,1);
  lcd.setCursor(spalte,zeile);
  char *charkette = const_cast<char *>(lcdtext.c_str()); 
  lcd.print(charkette); //(F()) saves string to flash & keeps dynamic memory free

/*
    char *charkette = const_cast<char *>(lcdtext.c_str()); // string für LCD in char-Kette  wandeln
    lcd.setCursor(spalte, zeile);
    lcd.print(charkette);*/
    }
void fctLcdDelete()
{


  //lcd.clear();
  // Ende
}
std::string fctFloatString(float input, int stellen)
{
    std::string raw = std::to_string(input);
    return raw.substr(0, raw.find('.')) + '.' + raw.substr(raw.find('.') + 1, 2);
}