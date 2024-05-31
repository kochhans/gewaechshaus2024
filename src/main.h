#pragma once

#ifndef MAIN_H
#define MAIN_H

/***********************************************************
    Bibliotheken einbinden
************************************************************/
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "OneWire.h" // Temperatursensor
#include "DallasTemperature.h"
#include "LedControl.h"
/***********************************************************
    Pinbelegung/Sensortyp/Serveradresse definieren
************************************************************/
#define LED_ONBOARD 2

/* Pins alt

#define MOTOR1PLUS 18 // Steuerung der H-Brücke
#define MOTOR1MINUS 19
#define MOTOR2PLUS 23
#define MOTOR2MINUS 22
#define SCHALTER 4 // Schalter Hand/Automatik
#define S1AUF 32   // Taste
#define S1ZU 33    // Taste
#define S2AUF 25   // Taste
#define S2ZU 26    // Taste

#define ONE_WIRE_BUS 5 // 1Wire für Temperatursensor

*/

//Pins neu:
//EINGABEN
#define SCHALTER 35 // Schalter Hand/Automatik
#define S1AUF 32   // Taste
#define S1ZU 33    // Taste
#define S2AUF 25   // Taste
#define S2ZU 26    // Taste

#define MOTOR1PLUS 4 // Steuerung der H-Brücke
#define MOTOR1MINUS 5
#define MOTOR2PLUS 18
#define MOTOR2MINUS 19
#define SEGMENTG 2



#define ONE_WIRE_BUS 34 // 1Wire für Temperatursensor
/***********************************************************
    Variablen und Konstanten
*************************************************************/
const float tempAuf = 28.0;           // Bei dieser Temperatur: Fenster öffnen
const float tempZu = 23.0;            // Bei dieser Temperatur: Fenster schließen
const uint16_t schaltpause = 1000;    // minimale Pause zwischen Schaltvorgängen
const uint16_t motordauerAuf = 1000;//45000; // 1000; // 45000 --alt:40000; // Dauer für Öffnungsvorgang
const uint16_t motordauerZu = 2000;//50000;  // 1000;  // 50000- alt: 40000;  // Dauer für Schließvorgang in ms
const uint16_t prellzeit = 500;
const uint8_t displayaktiv = 1; // Soll das achtstellige 7-Segment-Display angesteuert werden? 0=nein, 1=ja

const int startPin = ONE_WIRE_BUS; //OneWire-Scan
const int endPin = ONE_WIRE_BUS;

uint8_t s1 = LOW;          // Logikzustand des Schalters
uint8_t fensterstand1 = 0; // 0=zu, 1=offen
uint8_t fensterstand2 = 0; // 0=zu, 1=offen

float tempAktuell = 999; // Aktuelle Temperatur vom Sensor

/************************************************************
    Funktionsdeklaration
************************************************************/
void fctMotor(uint8_t motorauswahl, uint8_t richtung, uint16_t dauer);
float fctSensorenLesen(uint adresse);
int8_t fctAutomatikbetrieb();
uint8_t fctHandbetrieb();

void fctLedOnboard(boolean);
void fctSiebensegmentanzeige(uint8_t zustand, float messwertaktuell);
uint8_t fctFindOneWireDevices(int pin);
#endif