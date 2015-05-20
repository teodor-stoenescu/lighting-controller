#ifndef _MANUAL_OUTPUT_H
#define _MANUAL_OUTPUT_H

#include "AbstractScreen.h"

void SetOutput();

class ManualOutputScreen : public AbstractScreen {
private :
    byte oldActive;
    word oldValue, currentValue;
public :
    ManualOutputScreen(PersistentData *pdd) : AbstractScreen(pdd) {
        oldActive = 0;
        oldValue = 0;
        currentValue = 0;
    }

    virtual void Draw(Display *disp) {
        for (byte i = 0; i < currentValue; ++i) {
            disp->disp[0][i] = '+';
        }
        
        for (byte i = currentValue; i < 16; ++i) {
            disp->disp[0][i] = ' ';
        }
        
        strcpy(disp->disp[1], " -    done    + ");
    }
    
    virtual void Enter() {
        oldActive = pd->active;
        oldValue = pd->analogValue;
        currentValue = (oldValue + 128) >> 8;
        
        pd->active = 1;
        pd->analogValue = currentValue << 8;
    }
    
    virtual void HandleInput(byte input) {
        switch (input) {
            case INPUT_TICK :
                break;
                
            case INPUT_BUTTON_LEFT :
                if (currentValue > 0) {
                    currentValue--;
                    pd->analogValue = currentValue << 8;
                    SetOutput();
                }
                break;
            case INPUT_BUTTON_CENTER :
                pd->active = oldActive;
                pd->analogValue = oldValue;
                SwitchScreen(HOME_SCREEN);
                break;
            case INPUT_BUTTON_RIGHT :
                if (currentValue < 15) {
                    currentValue++;
                    pd->analogValue = currentValue << 8;
                    SetOutput();
                }
                break;
        }
    }
};

#endif

