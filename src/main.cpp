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


/**********************************************************
  Funktionen (Funktionsdefinitionen)
*********************************************************** */

// ## Onboard-LED soll leuchten, wenn ein Fenster beliebig bewegt wird
void fctLedOnboard(boolean schalten)
{

}

// ## Fenstermotoren ansteuern (Motor, Richtung, Dauer)
void fctMotor(uint8_t motorauswahl, uint8_t richtung, uint16_t dauer)
{
  // Übergabewerte - Bedeutung
  //-.-.-.-.-.-.-.-.-.-.-.-.-.-
  // motorauswahl=1=MOTOR1
  // motorauswahl=2=MOTOR2
  // richtung:
  //  0 = stoppen
  //  1 = rechts (auf)
  //  2 = links (zu)
  // dauer in ms (max. 16 Bit) kommt von der Variablen motordauerAuf oder motordauerZu
  Serial.print("fctMotorStart MOTOR:  ");
  Serial.println(motorauswahl);
  Serial.print("Richtung:  ");
  Serial.println(richtung);
  Serial.print("Dauer:  ");
  Serial.println(dauer);

  uint8_t plus = MOTOR1PLUS;
  uint8_t minus = MOTOR1MINUS;
  if (motorauswahl == 1)
  {
    fctLedOnboard(true);
    plus = MOTOR1PLUS;
    minus = MOTOR1MINUS;
  }
  else if (motorauswahl == 2)
  {
    fctLedOnboard(true);
    plus = MOTOR2PLUS;
    minus = MOTOR2MINUS;
  }
  else
  { // alles aus
    digitalWrite(MOTOR1MINUS, 0);
    digitalWrite(MOTOR1PLUS, 0);
    digitalWrite(MOTOR2MINUS, 0);
    digitalWrite(MOTOR2PLUS, 0);
  }

  switch (richtung)
  {
  case 0:
    break;
  case 1:
    digitalWrite(plus, 1); // rechts rum ein
    digitalWrite(minus, 0);
    delay(dauer);

    break;
  case 2:
    digitalWrite(plus, 0); // links rum ein
    digitalWrite(minus, 1);
    delay(dauer);
    break;
  default:
    digitalWrite(plus, 0); // aus
    digitalWrite(minus, 0);
  }
  fctLedOnboard(false);
  Serial.println("fctMotor Ende");
}

// ## Automatikbetrieb, Messung der Temperatur und Fenster bei Bedarf öffnen/schließen
// ## Rückgabe 0, wenn auf Handbetrieb geschaltet wurde
int8_t fctAutomatikbetrieb()
{
  int messwertzahl = 0;
  Serial.println("... Automatik gewählt");
  do
  { // Automatikbetrieb
    delay(prellzeit);
    tempAktuell = fctSensorenLesen(0);
    if(digitalRead(S1ZU)==LOW)
    {
    }
    if ((displayaktiv==1) )//&& (digitalRead(S1AUF) == LOW))
    { // Wenn im Automatikbetrieb S1  gedrückt ist,
      // wird das Display Stromsparmodus angesteuert.
      Serial.println("S1 gedrückt... ");
      // The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
      fct7SegAktiv(0, true);
      // Helligkeit
      fct7SegHelligkeit(0,2);
      // Anzeige mit aktueller Temperatur ansteuern
      fct7SegWrite(0, tempAktuell);

      fctLcdDelete();
      //fctLcdText("Temperatur...", 0, 0);
      lcdText="Temp: ";
      lcdText=lcdText + fctFloatString(tempAktuell, 3);
      fctLcdText(lcdText, 0,0);
 
    }
    else
    { // Wenn im Automatikbetrieb S1 nicht gedrückt ist,
      // wird das Display in den Stromsparmodus gesetzt.
      Serial.println("S1 nicht gedrückt... ");
      fct7SegAktiv(0, false);
      //lc.shutdown(0, true);
    }
    // Entscheidung, ob und welche Motoren angesteuert werden müssen.
    if ((tempAktuell > tempAuf) && fensterstand1 == 0)
    { // Fenster 1 aufmachen
      fctMotor(1, 1, motordauerAuf);
      delay(schaltpause);
      fctMotor(0, 0, 0); // Motoren aus
      fensterstand1 = 1; // 1=offen
    }
    if ((tempAktuell > tempAuf) && fensterstand2 == 0)
    { // Fenster 2 aufmachen
      fctMotor(2, 1, motordauerAuf);
      delay(schaltpause);
      fctMotor(0, 0, 0); // Motoren aus
      fensterstand2 = 1; // 1=offen
    }
    if ((tempAktuell < tempZu) && fensterstand1 == 1)
    { // Fenster 1 zumachen
      fctMotor(1, 2, motordauerZu);
      delay(schaltpause);
      fctMotor(0, 0, 0); // Motoren aus
      fensterstand1 = 0; // 0=zu
    }
    if ((tempAktuell < tempZu) && fensterstand2 == 1)
    { // Fenster 1 zumachen
      fctMotor(2, 2, motordauerZu);
      delay(schaltpause);
      fctMotor(0, 0, 0); // Motoren aus
      fensterstand2 = 0; // 0=zu
    }
    if ((tempAktuell <= tempAuf) && (tempAktuell >= tempZu))
    {
    }
    s1 = digitalRead(SCHALTER);
  } while (s1 == HIGH);
  delay(prellzeit); // Prellzeit bei Betätigung des Hand/Auto-Schalters
  Serial.println("Automatikbetrieb Ende");
  return 0;
}

// ## Handbetrieb, Steuerung der Motoren durch Taster
uint8_t fctHandbetrieb()
{
  Serial.println("... Handbetrieb gewählt");
  // Display in den Stromsparmodus
  fct7SegLeeren(0);
  //lc.clearDisplay(0);
  fct7SegAktiv(0, false);
  //lc.shutdown(0, true);
  do
  { // Handbetrieb
    // Festlegen der Aktionen aufgrund der Tastenbetätigung
    if ((digitalRead(S1AUF) == LOW) && fensterstand1 == 0)
    { // Fenster 1 AUF
      fctMotor(1, 1, motordauerAuf);
      delay(schaltpause);
      fensterstand1 = 1; // 1=offen
      fctMotor(0, 0, 0); // Motoren aus
    }
    else if ((digitalRead(S1ZU) == LOW) && fensterstand1 == 1)
    { // Fenster 1 ZU
      fctMotor(1, 2, motordauerZu);
      delay(schaltpause);
      fensterstand1 = 0; // 0=zu
      fctMotor(0, 0, 0); // Motoren aus
    }
    else if ((digitalRead(S2AUF) == LOW) && fensterstand2 == 0)
    { // Fenster 2 AUF
      fctMotor(2, 1, motordauerAuf);
      delay(schaltpause);
      fensterstand2 = 1; // 1=offen
      fctMotor(0, 0, 0); // Motoren aus
    }
    else if ((digitalRead(S2ZU) == LOW) && fensterstand2 == 1)
    { // Fenster 2 ZU
      fctMotor(2, 2, motordauerZu);
      delay(schaltpause);
      fensterstand2 = 0; // 0=zu
      fctMotor(0, 0, 0); // Motoren aus
    }
    else
    {
    }

    s1 = digitalRead(SCHALTER);
  } while (s1 == LOW);
  delay(prellzeit);
  Serial.println("Handbetrieb Ende");
  return 1; // weiter im Automatikbetrieb
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
  fctOneWireSensorsStart();
  //sensors.begin();
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
