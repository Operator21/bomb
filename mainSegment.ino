#include <TM1638plus.h>

#define  STROBE_TM 4
#define  CLOCK_TM 6
#define  DIO_TM 7

float seconds = 0;
bool explodeState = false;

Password passwordInitial;
Password passwordSave;

TM1638plus tm(STROBE_TM, CLOCK_TM , DIO_TM, false);

void setup() {
    tm.displayBegin();
}

void loop() {   
    uint8_t buttons = tm.readButtons();
    switch(buttons) {
        case 1: increaseTime(100); break;
        case 2: decreaseTime(100); break;
        case 4: 
            if(passwordInitial.checkPasswordSet()) {
                resetLeds();
                explodeState = true; 
            }
            break;
        case 8: deactivate(); break;
        case 16:
        case 32:
        case 64:
        case 128:
            if(!explodeState) {
                passwordInitial.setCurrentArrayIndexValue(buttons);
            } else {
                passwordSave.setCurrentArrayIndexValue(buttons);
            }
            break;
    }
    if(explodeState) {
        delay(50);
        seconds -= 0.05f;
        if(seconds < 0.1) {
            explode();
        }
        passwordSave.displayPasswordSize(tm);
    } else {
        passwordInitial.displayPasswordSize(tm);
    }
    tm.displayIntNum(seconds, false);
}

void increaseTime(int time) {
    seconds++;
    delay(time);
    tm.reset();
}

void decreaseTime(int time) {
    if(seconds > 0) {
        seconds--;
        delay(time);
        tm.reset();
    }
}
void explode() {
    tm.displayText("BOOOOMMMM");
    delay(3000);
    explodeState = false;
    tm.reset();
}

void reset() {
    passwordInitial.setDefaultValues();
    passwordSave.setDefaultValues();
    explodeState = false;
    seconds = 0;
    resetLeds();
    tm.reset();
}


void resetLeds() {
    for(int x = 0; x < 8; x++) {
        tm.setLED(x, 0);
    }
}

void deactivate() {
    if(!passwordInitial.compareWith(passwordSave)){
        resetLeds();
        passwordSave.setDefaultValues();
        return;
    }
    tm.displayText("Deactivated");
    delay(3000);
    reset();
}