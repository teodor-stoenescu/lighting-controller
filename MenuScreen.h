#ifndef _MENU_SCREEN_H
#define _MENU_SCREEN_H

#include "AbstractScreen.h"

#define MENU_SELECT_SET_TIME     0
#define MENU_SELECT_MANUAL       1
#define MENU_SELECT_ABOUT        2
#define MENU_SELECT_EXIT         3

class MenuScreen : public AbstractScreen {
private :
    byte menuSelection;
public :
    MenuScreen(PersistentData *pdd) : AbstractScreen(pdd) {
        menuSelection = 0;
    }

    void Draw(Display *disp) {
        DrawTime(pd->day, pd->hour, pd->minute, pd->second, pd->active, disp->disp[0]);
        strcpy(disp->disp[1], " < ");
        switch (menuSelection) {
            case MENU_SELECT_SET_TIME :
                strcpy(&disp->disp[1][3], " SET TIME ");
                break;
            case MENU_SELECT_MANUAL :
                strcpy(&disp->disp[1][3], "  MANUAL ");
                break;
            case MENU_SELECT_ABOUT :
                strcpy(&disp->disp[1][3], "   ABOUT  ");
                break;
            case MENU_SELECT_EXIT :
                strcpy(&disp->disp[1][3], "   EXIT   ");
                break;
        }
        strcpy(&disp->disp[1][13], " > ");
    }
    
    void Enter() {
        menuSelection = 0;
    }
    
    void HandleInput(byte input) {
        switch (input) {
            case INPUT_TICK :
                // timeout
                break;
            case INPUT_BUTTON_LEFT :
                if (MENU_SELECT_SET_TIME == menuSelection) {
                    menuSelection = MENU_SELECT_EXIT;
                } else {
                    menuSelection--;
                }
                break;
            case INPUT_BUTTON_CENTER :
                switch (menuSelection) {
                    case MENU_SELECT_SET_TIME :
                        SwitchScreen(MENU_SET_TIME);
                        break;
                    case MENU_SELECT_MANUAL :
                        SwitchScreen(MENU_MANUAL_OUTPUT);
                        break;
                    case MENU_SELECT_ABOUT :
                        SwitchScreen(MENU_ABOUT);
                        break;
                    case MENU_SELECT_EXIT :
                        SwitchScreen(HOME_SCREEN);
                        break;
                }
                break;
            case INPUT_BUTTON_RIGHT :
                if (MENU_SELECT_EXIT == menuSelection) {
                    menuSelection = MENU_SELECT_SET_TIME;
                } else {
                    menuSelection++;
                }
                break;
        }
    }
};

#endif


