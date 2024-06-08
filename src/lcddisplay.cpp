/*
 *#######################################################
 * Beschreibung.: Funktion für das 7Segment LED-Display
 * Dateiname....: lcddisplay.cpp
 *#######################################################
 */

// lcd.setCursor(2, 0); // Set the cursor on the third column and first row.
// lcd.print("Hello World!"); // Print the string "Hello World!"
#include <bits/stdc++.h>
#include <LiquidCrystal_I2C.h>
const int LCD_COLUMNS = 16;
const int LCD_ROWS = 2;

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);
void fctLcdAn(boolean an = true)
{
  lcd.begin(LCD_COLUMNS, LCD_ROWS, LCD_5x8DOTS);
}
void fctLcdHintergrund(boolean an = true)
{
  if (an == false)
  {
    lcd.noBacklight();
  }
  else
  {
    lcd.backlight();
  }
}

void fctLcdText(std::string lcdtext, int spalte, int zeile)
{
  int stringlaenge = 0;

  //  LCD Test  -----------------
  /*while (lcd.begin(conflcd.COLUMNS, conflcd.ROWS, LCD_5x8DOTS) != 1) // colums, rows, characters size
  {
    Serial.print(F("PCF8574 is not connected or lcd pins declaration is wrong. Only pins numbers: 4,5,6,16,11,12,13,14 are legal.\n"));
    delay(5000);
  }*/
  stringlaenge = (lcdtext.length());
  char *charkette = const_cast<char *>(lcdtext.c_str());
  lcd.setCursor(spalte, zeile);
  // lcd.print(lcdtext);
  lcd.print(F(charkette)); // lcd.print(F("PCF8574 is OK...")); //(F()) saves string to flash & keeps dynamic memory free
}
void fctLcdDelete()
{
  lcd.clear();
}
std::string fctFloatString(float input, int stellen)
{
  std::string raw = std::to_string(input);
  return raw.substr(0, raw.find('.')) + '.' + raw.substr(raw.find('.') + 1, 2);
}
