/*
 *#######################################################
 * Beschreibung.: Funktion für den AD-Wandler
 * Dateiname....: adwandler.cpp
 *#######################################################
 */
#include <Arduino.h>
//#include "adwandler.h"
int ldrWert = 0;
int potiWert = 0;
const int DELAY_ADWANDLER=3000;
const int LDR_PIN = 39;
const int POTI_PIN = 36;

int fctLdrLesen()
{
    ldrWert = analogRead(LDR_PIN);
    delay(DELAY_ADWANDLER);
    return ldrWert;
}
int fctPotiLesen()
{
    potiWert = analogRead(POTI_PIN);
    delay(DELAY_ADWANDLER);
    return potiWert;
}
