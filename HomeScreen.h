#ifndef _HOME_SCREEN_H
#define _HOME_SCREEN_H

#include "AbstractScreen.h"

//void ToggleAnalogOut();

class HomeScreen : public AbstractScreen {
public :
    HomeScreen(PersistentData *pdd) : AbstractScreen(pdd) { }

    virtual void Draw(Display *disp) {
        DrawTime(pd->day, pd->hour, pd->minute, pd->second, pd->active, disp->disp[0]);
        strcpy(disp->disp[1], "      MENU  STOP");
        if (0 == pd->active) {
            strcpy(&disp->disp[1][11], "START");
        }
        
        word tmp = pd->analogValue;
        byte ps = 3;
        do {
            disp->disp[1][ps] = '0' + tmp % 10;
            tmp /= 10;
            ps--;
        } while (tmp > 0);
    }
    
    virtual void HandleInput(byte input) {
        switch (input) {
            case INPUT_TICK :
                // timeout
                break;
            case INPUT_BUTTON_CENTER :
                SwitchScreen(MAIN_MENU_SCREEN);
                break;
            case INPUT_BUTTON_RIGHT :
                // toggle activation
                ToggleAnalogOut();
                break;
        }
    }
};

#endif

