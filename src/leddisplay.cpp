/*
 *#######################################################
 * Beschreibung.: Funktion für das 7Segment LED-Display
 * Dateiname....: leddisplay.cpp
 *#######################################################
 */
#include "LedControl.h"
#include "leddisplay.h"

LedControl lc = LedControl(confled.SPI_MOSI, confled.SPI_CLK, confled.SPI_CS, 1);
/* we always wait a bit between updates of the display */
//unsigned long displaydelaytime = 250;

// Siebensegmentanzeige ansteuern (adresse, messwert)
void fct7SegWrite(uint8_t adresse, float messwertaktuell)
{
    // Float-Messwert in String wandlen
    std::string messwertaktuell_str = std::to_string(messwertaktuell);
    // Einzelne Chars für die Ziffernanzeige extrahieren
    std::string ziffer3 = messwertaktuell_str.substr(0, 1);
    std::string ziffer2 = messwertaktuell_str.substr(1, 1);
    std::string zifferdot = messwertaktuell_str.substr(2, 1);
    std::string ziffer1 = messwertaktuell_str.substr(3, 1);
    std::string ziffer0 = messwertaktuell_str.substr(4, 1);
    char z3 = ziffer3[0];
    char z2 = ziffer2[0];
    char zdot = zifferdot[0];
    char z1 = ziffer1[0];
    char z0 = ziffer0[0];
    // Testausgaben der Chars
    /*std::cout << "\nChar3" << z3;
    std::cout << "\nChar2" << z2;
    std::cout << "\nChardot" << zdot;
    std::cout << "\nChar1" << z1;
    std::cout << "\nChar0" << z0;*/

    // Display setzen
    lc.setChar(adresse, 7, z3, false);
    delay(confled.DISPLAYTIME);
    lc.setChar(adresse, 6, z2, true);
    delay(confled.DISPLAYTIME);
    lc.setChar(adresse, 5, z1, false);
    delay(confled.DISPLAYTIME);
    lc.setChar(adresse, 4, z0, false);
    delay(confled.DISPLAYTIME);
    lc.setChar(adresse, 3, 'x', false);
    delay(confled.DISPLAYTIME);
    lc.setChar(adresse, 2, 'o', false);
    delay(confled.DISPLAYTIME);
    lc.setChar(adresse, 1, 'C', false);
    delay(confled.DISPLAYTIME);
}
void fct7SegAktiv(int adresse, boolean ein)
{
    if (ein == true)
    {
        lc.shutdown(adresse, false);
    }
    else
    {
        lc.shutdown(adresse, true);
    }
    delay(confled.DISPLAYTIME);
}
void fct7SegHelligkeit(int adresse, int wert)
{
    lc.setIntensity(adresse, wert);
    delay(confled.DISPLAYTIME);
}
void fct7SegLeeren(int adresse)
{
    lc.clearDisplay(adresse);
        delay(confled.DISPLAYTIME);
}