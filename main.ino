#include <Arduino.h>
#include <Servo.h>


#include "gameManager.hpp"



/**************************************************************************************************
**                                            Macros                                             **
**************************************************************************************************/
#define trigger 9   // Distance sensor's input
#define echo 8      // Distance sensor's output
#define buzz 33     // Sound element
#define servoPin 7  // Token door lock
#define joyX A4     // Joystick horizontal   --   Help
#define joyY A5     // Joystick vertical
#define button 31   // Fire button
#define button2 32

// HUB75e -- the matrix
#define R1 25
#define G1 26
#define BL1 24
// Ground      //
#define R2 29
#define G2 28
#define BL2 27
#define PIN_A A0
#define PIN_B A1
#define PIN_C A2
#define PIN_D A3
#define PIN_E 30
#define CLK 12  // USE THIS ON ARDUINO MEGA
#define LAT 11
#define OE 10
// Ground      //


/**************************************************************************************************
**                                           Globals                                             **
**************************************************************************************************/
uint8_t gamestate = 0;
bool realGame = false;
bool rotorToggle = false;

Servo rotor;

RGBmatrixPanel* matrix = new RGBmatrixPanel(PIN_A, PIN_B, PIN_C, PIN_D, CLK, LAT, OE, false, 64);

GameManager* gameMan;

void setup() {
    Serial.begin(115200);
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(button, INPUT_PULLUP);
    pinMode(button2, INPUT);
    pinMode(buzz, OUTPUT);
    rotor.attach(servoPin);

    matrix->begin();
    matrix->setFont(NULL);
    gameMan = new GameManager(matrix);
    rotor.write(0);
    delay(500);
}

unsigned long senseTime = 0;
unsigned long oldTime = 0;
unsigned long nowSensed = 0;
unsigned long oldSensed = measure(senseTime, oldTime);

void loop() {
    switch (gamestate) {
        case 0:
            if(millis() - (oldTime / 1000) > 100){
                nowSensed = measure(senseTime, oldTime);
            }

            if (nowSensed <= oldSensed *.9) {
                gamestate++;
            } else {
                oldSensed = nowSensed;
                gameMan->drawStandby();
            }

            break;


        case 1:
            Serial.println("Game manager takes over\n\n");
            gameMan->onStart();
            gameMan->zoomScope();
            gamestate++;

            break;


        case 2:
            if (!rotorToggle){
                rotor.write(179);
                delay(1000);
                rotorToggle = true;
            }

            break;


        default:
            return;
    }
}

unsigned long measure(unsigned long & senseTime, unsigned long & oldTime){
    senseTime = micros();

    if (senseTime - oldTime >= 10){
        digitalWrite(trigger, HIGH);
    }

    if (senseTime - oldTime >= 20){
        oldTime = senseTime;
        digitalWrite(trigger, LOW);
        auto temp = pulseIn(echo, HIGH);

        Serial.print("Distance is: ");
        Serial.println(temp);
        return temp;
    }
}