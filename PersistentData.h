#ifndef _PERSISTENT_DATA_H
#define _PERSISTENT_DATA_H

#ifdef HAS_DC_GONE
#include <EEPROM.h>
#endif

class PersistentData {
public :
    byte day, hour, minute, second;
    byte bklight;
    
    byte active;
    word analogValue;
    
    void ReadFromEEPROM() {
    #ifdef HAS_DC_GONE
        day = EEPROM.read(0);
        hour = EEPROM.read(1);
        minute = EEPROM.read(2);
        second = EEPROM.read(3);
        bklight = EEPROM.read(4);
    #else
        day = hour = minute = second = 0;
    #endif
    
        if ((day > 6) || (hour > 23) || (minute > 59) || (second > 59)) {
            day = hour = minute = second = 0;
        }
        analogValue = pgm_read_word_near(&analogData[day][hour][minute]);
    }
    
    void WriteToEEPROM() {
    #ifdef HAS_DC_GONE
        EEPROM.update(0, day);
        EEPROM.update(1, hour);
        EEPROM.update(2, minute);
        EEPROM.update(3, second);
        EEPROM.update(4, bklight);
    #endif
    }
  
};

#endif

