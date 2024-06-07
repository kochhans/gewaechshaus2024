/*
 *#######################################################
 * Beschreibung.: Gewächshaus-Steuerung
 * Projektname..: GEWAECHSHAUS2024
 * Dateiname....: main.cpp
 * Autor........: Koch, Hans
 * Datum/Zeit...: 2024-05-28 - 09:00
 * Controller...: ESP32 WROOM auf nodemcu-32s
 * Framework....: VSCode PlatformIO Arduino
 *#######################################################
 */
#include "main.h"
// #include "Siebensegment.h"
#include "LedControl.h"

// ## Onboard-LED soll leuchten, wenn ein Fenster beliebig bewegt wird
void fctLedOnboard(boolean schalten)
{
}

// ## Automatikbetrieb, Messung der Temperatur und Fenster bei Bedarf öffnen/schließen
// ## Rückgabe 0, wenn auf Handbetrieb geschaltet wurde
int8_t fctAutomatikbetrieb()
{
  int messwertzahl = 0;
  // Siebensegment *my7seg = new Siebensegment(); // neues Objekt my7seg
  // my7seg->aktivieren(0, 1);                    // Methode start aufrufen
  //   delay(1000);

  do
  { // Automatikbetrieb
    delay(prellzeit);
    // Serial.println("LDR und Poti:");
    // Serial.println(fctLdrLesen());
    // Serial.println(fctPotiLesen());

    tempAktuell = fctSensorenLesen(0);

    if ((displayaktiv == 1))
    { // Wenn im Automatikbetrieb S1  gedrückt ist,
      // wird das Display Stromsparmodus angesteuert.
      Serial.println("LCD einschalten ");
      lcdText = "Temp: ";
      lcdText = lcdText + fctFloatString(tempAktuell, 3);
      fctLcdText(lcdText, 0, 0);
      Serial.println("LED mit Funktionen ");
      // The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
      fct7SegAktiv(0, true);
      // Helligkeit
      fct7SegHelligkeit(0, 1);
      // Anzeige mit aktueller Temperatur ansteuern
      fct7SegWrite(0, tempAktuell);
      // fct7SegAktiv(0, false);
      delay(2000);
      /*Serial.println("LDR AD lesen");
      werthelligkeit = (fctLdrLesen());
      Serial.println("LDR: " + werthelligkeit);
      lcdText = "Helligkeit: ";
      lcdText = lcdText + fctFloatString(werthelligkeit, 3);
      fctLcdText(lcdText, 0, 0);

      delay(2000);
      Serial.println("Poti AD lesen");
      wertwiderstand = (fctPotiLesen());
      Serial.println("Wid:: " + wertwiderstand);
      lcdText = "Wid: ";
      lcdText = lcdText + fctFloatString(wertwiderstand, 3);
      fctLcdText(lcdText, 0, 0);*/


      /*_-----------------------------------------------------
            my7seg->setIntens(0, 1); // Helligkeit einstellen
            // my7seg.blank(1);
            my7seg->write(0, tempAktuell + 0.5); // schreiben
            // fctLcdDelete();
            my7seg->setIntens(0, 9); // Helligkeit ändern
            // my7seg.blank(1);
            delay(500);
            my7seg->write(0, tempAktuell - 0.5);
            delay(500);
            my7seg->setIntens(0, 3); // Helligkeit ändern
            delay(500);
      ------------------------------------------------------------*/
    }
    else
    { // Wenn im Automatikbetrieb S1 nicht gedrückt ist,
      // wird das Display in den Stromsparmodus gesetzt.
      Serial.println("S1 nicht gedrückt... ");
      fct7SegAktiv(0, false);
      // lc.shutdown(0, true);
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
  // lc.clearDisplay(0);
  fct7SegAktiv(0, false);
  // lc.shutdown(0, true);
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
 
  
  // Ausgänge und Eingänge festlegen
  fctMotorpins();
  pinMode(SCHALTER, INPUT_PULLUP);
  pinMode(S1AUF, INPUT_PULLUP);
  pinMode(S1ZU, INPUT_PULLUP);
  pinMode(S2AUF, INPUT_PULLUP);
  pinMode(S2ZU, INPUT_PULLUP);
  // pinMode(36, INPUT);
  pinMode(39, INPUT);

  // Dallas Temperatursensor init
  fctOneWireSensorsStart();

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
  //Serial.println("--- void loop()  Start---");
  //Serial.println("--- Betrieb Start ---");
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
  //Serial.println("--- void loop() Ende ---");
}
