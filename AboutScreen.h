#ifndef _ABOUT_SCREEN_H
#define _ABOUT_SCREEN_H

#include "AbstractScreen.h"

class AboutScreen : public AbstractScreen {
private :
    byte txt, clk;
public :
    AboutScreen(PersistentData *pdd) : AbstractScreen(pdd) {
        txt = 0;
        clk = 0;
    }

    virtual void Draw(Display *disp) {
        switch (txt) {
            case 0:
                strcpy(disp->disp[0], "Lighting control");
                strcpy(disp->disp[1], "            v0.9");
                break;
            case 1:
                strcpy(disp->disp[0], "Designed by:    ");
                strcpy(disp->disp[1], "Teodor Stoenescu");
                break;       
        }
    }
    
    virtual void Enter() {
        txt = 0;
        clk = 0;
    }
    
    virtual void HandleInput(byte input) {
        switch (input) {
            case INPUT_TICK :
                clk++;
                if (clk == 4) {
                    txt++;
                    clk = 0;
                    
                    if (txt == 2) {
                        SwitchScreen(HOME_SCREEN);
                    }
                }
                break;
                
            case INPUT_BUTTON_LEFT :
            case INPUT_BUTTON_CENTER :
            case INPUT_BUTTON_RIGHT :
                SwitchScreen(HOME_SCREEN);
                break;
        }
    }
};

#endif

