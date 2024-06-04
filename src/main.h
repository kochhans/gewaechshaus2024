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
#include "DallasTemperature.h"
#include "LedControl.h"
#include <Wire.h>
#include "LiquidCrystal_I2C.h"

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


//AUSGABEN


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
#define SPIMOSI 13
#define SPIMISO 12
#define SPICS 15
#define SPICLK 14






/***********************************************************
    Variablen und Konstanten
*************************************************************/

const uint8_t displayaktiv = 1; // Soll das achtstellige 7-Segment-Display angesteuert werden? 0=nein, 1=ja
boolean displayeinflag=false; // Flipflop, um das Display an und aus zu machen
const int startPin = ONE_WIRE_BUS; //OneWire-Scan
const int endPin = ONE_WIRE_BUS;



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
void fctSiebensegmentanzeige(uint8_t zustand, float messwertaktuell);


//OneWire
uint8_t fctFindOneWireDevices(int pin);

//I2C
void i2ctest();
void fctLcdText(std::string lcdtext, int spalte, int zeile);
void fctLcdDelete();
std::string fctFloatString(float input, int stellen);


#endif