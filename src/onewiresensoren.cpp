/*
 *#######################################################
 * Beschreibung.: OneWire-Sensoren ansteuern
 * Dateiname....: onewiresensor.cpp
 *#######################################################
*/
#include "OneWire.h"    // 
#include "DallasTemperature.h"
#include <Wire.h>
#define ONE_WIRE_BUS 32 // Anschluss am ESP32

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void fctOneWireSensorsStart();


/**********************************************************
  Funktionen (Funktionsdefinitionen)
*********************************************************** */

uint8_t fctFindOneWireDevices(int pin)
{
  OneWire ow(pin);

  uint8_t address[8];
  uint8_t count = 0;

  if (ow.search(address))
  {
    Serial.print("\nuint8_t pin");
    Serial.print(pin, DEC);
    Serial.println("[][8] = {");
    do
    {
      count++;
      Serial.println("  {");
      for (uint8_t i = 0; i < 8; i++)
      {
        Serial.print("0x");
        if (address[i] < 0x10) Serial.print("0");
        Serial.print(address[i], HEX);
        if (i < 7) Serial.print(", ");
      }
      Serial.print("  },");
      // CHECK CRC
      if (ow.crc8(address, 7) == address[7])
      {
        Serial.println("\t\t// CRC OK");
      }
      else
      {
        Serial.println("\t\t// CRC FAILED");
      }
    } while (ow.search(address));

    Serial.println("};");
    Serial.print("// nr devices found: ");
    Serial.println(count);
  }

  return count;
}

void fctOneWireSensorsStart()
{
    sensors.begin();
}

// ## Sensoren am OneWire-Bus lesen und ausgeben
// ## Rückgabe ist der Temperaturwert des Sensors
float fctSensorenLesen(uint adresse)
{
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(adresse);
  if (tempC != DEVICE_DISCONNECTED_C)
  {
    Serial.print("Messung Temperatursensor Adr. ");
    Serial.print(adresse);
    Serial.print("  (in °C)...: ");
    Serial.println(tempC);
  }
  else
  {
    Serial.print("ACHTUNG!!! Sensor ");
    Serial.print(adresse);
    Serial.println(" nicht erreichbar!!!");
    tempC = 999;
  }
  return (tempC);
}
