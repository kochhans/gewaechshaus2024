/*
 *#######################################################
 * Beschreibung.: Gewächshaus-Steuerung
 * Projektname..: GEWAECHSHAUS2024
 * Dateiname....: main.cpp
 * Autor........: Koch, Hans
 * Datum/Zeit...: 2024-05-28 - 09:00
 * Test
 *
 * Controller...: ESP32 WROOM auf nodemcu-32s
 * Framework....: VSCode PlatformIO Arduino
 *#######################################################
Beschreibung:
Die Steuerung schaltet über eine Doppel-H-Brücke zwei Stellmotoren für
die Fenster im Gewächshaus. Zunächst ist nur das vollständige Öffnen und Schließen vorgesehen.
Abhängig vom Messwert des Temperatursensors DS18B20 erfolgt die Ansteuerung der H-Brücke
und die entsprechende Drehrichtung der Motoren.

Der Handbetrieb ist durch Betätigen eines Druckschalters (S1 Hand/Auto) möglich.
Anschließend kann jeder Motor einzeln per Taste (S1AUF, S1ZU, S2AUF, S2ZU angesteuert werden.
Die Ansteuerzeiten entsprechen denen des Automatikbetriebs.
S1AUF, S1ZU : Steuerung von Fenster 1
S2AUF, S2ZU : Steuerung von Fenster 2

Anzeigensteuerung:
Im Automatikbetrieb kann durch Drücken von S1AUF die Temperatur auf der Anzeige sichtbar gemacht werden.
***********************************************************
Dateien:
+src  main.cpp
      main.h
***********************************************************
  Bibliotheken einbinden
*********************************************************** */
#include "main.h"

/**********************************************************
  Instanzen anlegen
*********************************************************** */
// ## Temperatursensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
// LedControl(int dataPin, int clkPin, int csPin, int numDevices=1);
// DATA= GPIO13, CL=GPIO14, CS=15, 1 Anzeige)
//LedControl lc = LedControl(SPIMOSI, SPICLK, SPICS, 1);
/* we always wait a bit between updates of the display */
//unsigned long displaydelaytime = 250;

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





// ## Onboard-LED soll leuchten, wenn ein Fenster beliebig bewegt wird
void fctLedOnboard(boolean schalten)
{

}


/***********************************************************************
       SETUP des Programms - einmalige Einstellungen bei Programmstart
************************************************************************/
void setup()
{
  Serial.begin(115200); // Serieller Monitor Start
  Serial.println("--- void setup() Start ---");
  Serial.println("//\n// Start oneWireSearch \n//");

  for (uint8_t pin = startPin; pin <= endPin; pin++)
  {
    fctFindOneWireDevices(pin);
  }
  Serial.println("\n//\n// End oneWireSearch \n//");

  //Ausgänge und Eingänge festlegen
  //pinMode(LED_ONBOARD, OUTPUT);
  pinMode(MOTOR1PLUS, OUTPUT);
  pinMode(MOTOR1MINUS, OUTPUT);
  pinMode(MOTOR2PLUS, OUTPUT);
  pinMode(MOTOR2MINUS, OUTPUT);
  

  pinMode(SCHALTER, INPUT_PULLUP);

  pinMode(S1AUF, INPUT_PULLUP);
  pinMode(S1ZU, INPUT_PULLUP);
  pinMode(S2AUF, INPUT_PULLUP);
  pinMode(S2ZU, INPUT_PULLUP);

  // Dallas Temperatursensor init
  sensors.begin();
  Serial.println("--- Einschaltphase Start ---");
  // Fenster 1 aufmachen
  fctMotor(1, 1, motordauerAuf);
  delay(schaltpause);
  fctMotor(0, 0, 0);
  fensterstand1 = 1;
  // Fenster 2 aufmachen
  fctMotor(2, 1, motordauerAuf);
  fctMotor(0, 0, 0);
  fensterstand2 = 1;
  delay(schaltpause);
  Serial.println("--- void setup() Ende  ---");
}

void loop()
{
  Serial.println("--- void loop()  Start---");
  Serial.println("--- Betrieb Start ---");
  s1 = digitalRead(SCHALTER);
  if (s1 == LOW)
  {
    delay(prellzeit);
    fctHandbetrieb();
  }
  else if (s1 == HIGH)
  {
    delay(prellzeit);
    fctAutomatikbetrieb();
  }
  Serial.println("--- void loop() Ende ---");
}
