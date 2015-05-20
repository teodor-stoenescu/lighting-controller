#ifndef _DISPLAY_H
#define _DISPLAY_H

struct Display {
    char disp[2][16];
    byte cursorVisible;
    byte cursorRow, cursorColumn;
    
    void Show(LiquidCrystal &lcd) {
        lcd.setCursor(0, 0);
        lcd.print(disp[0]);
        lcd.setCursor(0, 1);
        lcd.print(disp[1]);
        
        if (cursorVisible) {
            lcd.setCursor(cursorColumn, cursorRow);
            lcd.cursor();
        } else {
            lcd.noCursor();
        }
    }
};

#endif

