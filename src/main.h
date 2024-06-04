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
#include <string>
#include <iostream>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "OneWire.h" // Temperatursensor
//#include "DallasTemperature.h"
//
#include <Wire.h>
//#include "LiquidCrystal_I2C.h"

/***********************************************************
    Pinbelegung/Sensortyp/Serveradresse definieren
************************************************************/


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
#define SCHALTER 34 // Schalter Hand/Automatik
#define S1AUF 26   // Taste
#define S1ZU 25    // Taste
#define S2AUF 33   // Taste
#define S2ZU 35    // Taste

//AUSGABEN
#define MOTOR1PLUS 2 // Steuerung der H-Brücke
#define MOTOR1MINUS 4
#define MOTOR2PLUS 5
#define MOTOR2MINUS 18

#define OUTD0 2
#define OUTD1 4
#define OUTD2 5
#define OUTD3 18
#define OUTD4 19
#define OUTD5 23
#define OUTD6 27

#define SEGMENTA 2
#define SEGMENTB 4
#define SEGMENTC 5
#define SEGMENTD 18
#define SEGMENTE 19
#define SEGMENTF 23
#define SEGMENTG 27

//#define LED_ONBOARD 2

//BUSSE 
#define ONE_WIRE_BUS 32 // 1Wire für Temperatursensor







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
boolean displayeinflag=false; // Flipflop, um das Display an und aus zu machen
const int startPin = ONE_WIRE_BUS; //OneWire-Scan
const int endPin = ONE_WIRE_BUS;

uint8_t s1 = LOW;          // Logikzustand des Schalters
uint8_t fensterstand1 = 0; // 0=zu, 1=offen
uint8_t fensterstand2 = 0; // 0=zu, 1=offen

float tempAktuell = 999; // Aktuelle Temperatur vom Sensor

std::string lcdText="";





/************************************************************
    Funktionsdeklaration
************************************************************/
void fctMotor(uint8_t motorauswahl, uint8_t richtung, uint16_t dauer);
float fctSensorenLesen(uint adresse);
int8_t fctAutomatikbetrieb();
uint8_t fctHandbetrieb();

//SPI
void fctLedOnboard(boolean);
void fct7SegWrite(uint8_t zustand, float messwertaktuell);
void fct7SegAktiv(int adresse, boolean ein);
void fct7SegHelligkeit(int adresse, int wert);
void fct7SegLeeren(int adresse);

//OneWire
uint8_t fctFindOneWireDevices(int pin);
void fctOneWireSensorsStart();




//I2C
void i2ctest();
void fctLcdText(std::string lcdtext, int spalte, int zeile);
void fctLcdDelete();
std::string fctFloatString(float input, int stellen);


#endif