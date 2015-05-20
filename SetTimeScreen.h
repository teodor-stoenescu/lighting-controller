#ifndef _SET_TIME_SCREEN_H
#define _SET_TIME_SCREEN_H

#include "AbstractScreen.h"

class SetTimeScreen : public AbstractScreen {
private :
    byte setDay, setHour, setMinute, setSecond, cPos;
public :
    SetTimeScreen(PersistentData *pdd) : AbstractScreen(pdd) {
        cPos = 0;
    }

    virtual void Draw(Display *disp) {
        DrawTime(setDay, setHour, setMinute, setSecond, false, disp->disp[0]);
        
        if (4 == cPos) {
            strcpy(disp->disp[1], "       >>   done");
        } else {
            strcpy(disp->disp[1], " -     >>     + ");
        }
        
        disp->cursorVisible = 1;
        disp->cursorRow = 0;
        switch (cPos) {
            case 0: disp->cursorColumn =  4; break;
            case 1: disp->cursorColumn =  7; break;
            case 2: disp->cursorColumn = 10; break;
            case 3: disp->cursorColumn = 13; break;
            case 4: disp->cursorColumn = 15; break;
        }    
    }
    
    virtual void Enter() {
        cPos = 0;
        setDay = pd->day;
        setHour = pd->hour;
        setMinute = pd->minute;
        setSecond = pd->second;
    }
    
    byte ClampInc(byte val, byte maxVal) {
        if (val == maxVal - 1) {
            return 0;
        }
        return val + 1;
    }
    
    byte ClampDec(byte val, byte maxVal) {
        if (0 == val) {
            return maxVal - 1;
        }
        return val - 1;
    }
    
    byte ClampOp(byte val, byte maxVal, bool inc) {
        if (inc) {
            return ClampInc(val, maxVal);
        }
        return ClampDec(val, maxVal);
    }
    
    void UpdateTime(bool increase) {
        switch (cPos) {
            case 0 :
                // day
                setDay = ClampOp(setDay, 7, increase);
                break;
            case 1 :
                // hour
                setHour = ClampOp(setHour, 24, increase);
                break;
            case 2 :
                // minute
                setMinute = ClampOp(setMinute, 60, increase);
                break;
            case 3 :
                // second
                setSecond = ClampOp(setSecond, 60, increase);
                break;
            case 4 :
                if (increase) {
                    pd->day = setDay;
                    pd->hour = setHour;
                    pd->minute = setMinute;
                    pd->second = setSecond;
                    pd->analogValue = pgm_read_word_near(&analogData[pd->day][pd->hour][pd->minute]);
                    SwitchScreen(HOME_SCREEN);
                }
        }
    }
    
    virtual void HandleInput(byte input) {
        switch (input) {
            case INPUT_TICK :
                break;
            case INPUT_BUTTON_LEFT :
                UpdateTime(false);
                break;
            case INPUT_BUTTON_CENTER :
                if (cPos == 4) {
                    cPos = 0;
                } else {
                    cPos++;
                }
                break;
            case INPUT_BUTTON_RIGHT :
                UpdateTime(true);
                break;
        }
    }
};

#endif

