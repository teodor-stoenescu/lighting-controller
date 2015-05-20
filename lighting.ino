#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>

#define LCD_RS    2
#define LCD_EN    3
#define LCD_D4    4
#define LCD_D5    5
#define LCD_D6    6
#define LCD_D7    7

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
Adafruit_MCP4725 dac;

#undef HAS_DC_GONE

typedef unsigned char byte;

#include "AnalogData.h"

#include "PersistentData.h"
#include "AbstractScreen.h"
#include "HomeScreen.h"
#include "MenuScreen.h"
#include "SetTimeScreen.h"
#include "ManualOutputScreen.h"
#include "AboutScreen.h"

PersistentData pData;
Display virtualDisp;

HomeScreen hs(&pData);
MenuScreen ms(&pData);
SetTimeScreen sts(&pData);
ManualOutputScreen mos(&pData);
AboutScreen as(&pData);
AbstractScreen *screens[] = { &hs, &ms, &sts, &mos, &as };

#define BUTTON_LEFT_PIN      A0
#define BUTTON_CENTER_PIN    A1
#define BUTTON_RIGHT_PIN     A2

const byte clockInt = 0; // digital pin 2 is now interrupt 0
const byte analogClockPin = 1; // the analog clock pin

byte masterClock, timeChanged;

void ClockCounter() {
    pData.second++;          // after one 490Hz cycle add 1 second ;)
    timeChanged = 1;
}

void ToggleAnalogOut() {
    if (pData.active) {
        pData.active = 0;
    } else {
        pData.active = 1;
    }
}

/*void StartClock() {
    attachInterrupt(clockInt, ClockCounter, RISING);
    pinMode(analogClockPin, OUTPUT);
    analogWrite(analogClockPin, 127);
    pData.active = 1;
}

void StopClock() {
    pData.active = 0;
    detachInterrupt(clockInt);
    analogWrite(analogClockPin, 0);    
    pinMode(analogClockPin, INPUT);
}*/

void setup() {
    Serial.begin(9600);
  
    pData.ReadFromEEPROM();
    
    lcd.begin(16, 2);
    dac.begin(0x62);
    
    // was 1000000
    // 1st  998652
    // 2nd  998678
    Timer1.initialize(998678);
    Timer1.attachInterrupt(ClockCounter);
}

byte currentScreen = HOME_SCREEN;

#define DAC_PWR_DOWN        0xC000
void SetOutput() {
    if (pData.active) {
        dac.setVoltage(pData.analogValue, false);
    } else {
        dac.setVoltage(DAC_PWR_DOWN, false);
    }
}

void UpdateTime() {
    if (60 == pData.second) {
        pData.minute++;
        pData.second = 0;
        if (60 == pData.minute) {
            pData.hour++;
            pData.minute = 0;
            if (24 == pData.hour) {
                pData.day++;
                pData.hour = 0;
                if (7 == pData.day) {
                    pData.day = 0;
                    // update values on a weekly basis

                }
            }
            // update values on a per hour basis
                
        }
        // update values on a per minute basis
        pData.analogValue = pgm_read_word_near(&analogData[pData.day][pData.hour][pData.minute]);
    } 
    // update values on a per second basis
    SetOutput();
    HandleInput(INPUT_TICK);
    Draw(&virtualDisp);
}

bool CheckButton(byte *lastState, byte pin) {
    byte oldState = *lastState;
    
    *lastState = digitalRead(pin);
    return (HIGH == oldState) && (*lastState == LOW);
}


byte lastPressLeft = 0, lastPressCenter, lastPressRight;
void loop() {
    // check if time has changed
    if (1 == timeChanged) {
        // change time
        timeChanged = 0;
        UpdateTime();
    }
    
    // check if any buttons have been pressed
    if (CheckButton(&lastPressLeft, BUTTON_LEFT_PIN)) {
        // emit a press event
        HandleInput(INPUT_BUTTON_LEFT);
        Draw(&virtualDisp);
    }
    
    if (CheckButton(&lastPressCenter, BUTTON_CENTER_PIN)) {
        HandleInput(INPUT_BUTTON_CENTER);
        Draw(&virtualDisp);
    }
    
    if (CheckButton(&lastPressRight, BUTTON_RIGHT_PIN)) {
        HandleInput(INPUT_BUTTON_RIGHT);
        Draw(&virtualDisp);
    }
    
    virtualDisp.Show(lcd);
    delay(30);
}


