#ifndef debouncebutton_h
#define debouncebutton_h

#include "Arduino.h"
struct DebounceButton {
    int pin;
    int stableState;
    boolean hasNewStableState;
    int lastReadState;
    unsigned long lastDebounceTime;
};

#endif
