/*
 *#######################################################
 * Beschreibung.: Funktion für den AD-Wandler
 * Dateiname....: adwandler.cpp
 *#######################################################
 */
#include <Arduino.h>
#include "adwandler.h"
int ldrWert = 10;
int potiWert = 10;

int fctLdrLesen()
{
    ldrWert = analogRead(confanalog.LDR_PIN);
    delay(confanalog.DELAY_AD);
    return ldrWert;
}
int fctPotiLesen()
{
    potiWert = analogRead(confanalog.POTI_PIN);
    delay(confanalog.DELAY_AD);
    return potiWert;
}
