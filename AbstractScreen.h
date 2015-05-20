#ifndef _ABSTRACT_SCREEN_H
#define _ABSTRACT_SCREEN_H

#include "Display.h"
#include "PersistentData.h"

#define INPUT_TICK               0
#define INPUT_BUTTON_LEFT        1
#define INPUT_BUTTON_CENTER      2
#define INPUT_BUTTON_RIGHT       3

/* The main menu screen
  - 30s timeout
  - first row shows the current time
  - second row shows prev, current selection and next button
*/
#define HOME_SCREEN              0
/* The set time screen
  - 30s timeout
  - first row shows a modifiable time
  - second row shows menu < + >
*/
#define MAIN_MENU_SCREEN         1
/* The set backlight screen
  - 10s timeout
  - first row shows a potentiometer
  - second row < done >
*/
#define MENU_SET_TIME            2
/* The about screen
  - 10s timeout
  - rows show about screen
  - press of any button leads to homescreen
*/
#define MENU_MANUAL_OUTPUT       3
#define MENU_ABOUT               4


class AbstractScreen {
protected :
    PersistentData *pd;
public :
    AbstractScreen(PersistentData *pdd) {
        pd = pdd;
    }

    virtual void Draw(Display *disp) = 0;
    virtual void Enter() { }
    virtual void HandleInput(byte input) = 0;
    
    void DrawTime(byte d, byte h, byte m, byte s, byte a, char *where) {
        strcpy(where, "DAY D HH:MM:SS v");
        where[ 4] = d + '0';
        where[ 6] = (h / 10) + '0';
        where[ 7] = (h % 10) + '0';
        where[ 9] = (m / 10) + '0';
        where[10] = (m % 10) + '0';
        where[12] = (s / 10) + '0';
        where[13] = (s % 10) + '0';
        where[15] = (1 == a) ? '*' : ' ';
    }
};

extern AbstractScreen *screens[];
byte activeScreen = 0;

void SwitchScreen(byte screenId) {
    activeScreen = screenId;
    screens[activeScreen]->Enter();
}

void HandleInput(byte input) {
    screens[activeScreen]->HandleInput(input);
}

void Draw(Display *disp) {
    screens[activeScreen]->Draw(disp);
}

#endif

