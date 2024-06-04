/*
 *#######################################################
 * Beschreibung.: Motoren über H-Brücke ansteuern
 * Dateiname....: motor.cpp
 *#######################################################
*/
#include <Arduino.h>

#define MOTOR1PLUS 2 // Steuerung der H-Brücke
#define MOTOR1MINUS 4
#define MOTOR2PLUS 5
#define MOTOR2MINUS 18

void fctLedOnboard(boolean schalten);

//##############################################
void fctMotorpins()
{
    pinMode(MOTOR1PLUS, OUTPUT);
    pinMode(MOTOR1MINUS, OUTPUT);
    pinMode(MOTOR2PLUS, OUTPUT);
    pinMode(MOTOR2MINUS, OUTPUT);
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