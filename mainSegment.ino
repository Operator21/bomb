
#include <TM1638plus.h>

#define  STROBE_TM 4
#define  CLOCK_TM 6
#define  DIO_TM 7
#define BUZZER 3

float seconds = 0;
bool explodeState = false;
int countdown = 0;

Password passwordInitial;
Password passwordSave;

TM1638plus tm(STROBE_TM, CLOCK_TM , DIO_TM, false);

void setup() {
    tm.displayBegin();
}

void loop() {   
    uint8_t buttons = tm.readButtons();
    switch(buttons) {
        case 1: if(!explodeState) increaseTime(100, 1); break;
        case 2: if(!explodeState) decreaseTime(100, 1); break;
        case 4: 
            if(passwordInitial.checkPasswordSet() && !explodeState) {
                resetLeds();
                explodeState = true; 
                delay(1000);
            } else if(explodeState) deactivate();
            else inputCodeError();
            break;
        case 9: if(!explodeState) increaseTime(100, 10); break;
        case 10: if(!explodeState) decreaseTime(100, 10); break;
        case 8: 
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
        case 192:
            if(!explodeState) {
                reset();
                delay(500);
            }
            break;
    }
    if(explodeState) {
        noTone(BUZZER);
        delay(50);
        seconds -= 0.05f;
        if(countdown == 20){
            countdown = 0;
            tone(BUZZER, 10000);
            tm.reset();
        }
        if(seconds < 0.1) {
            explode();
        }
        countdown++;
        passwordSave.displayPasswordSize(tm);
    } else {
        passwordInitial.displayPasswordSize(tm);
    }  
    tm.displayIntNum(seconds, false);
}

void increaseTime(int time, int value) {
    seconds += value;
    delay(time);
    tm.reset();
}

void decreaseTime(int time, int value) {
    if(seconds - value >= 0) {
        seconds -= value;
        delay(time);
        tm.reset();
    }
}
void explode() {
    seconds = 0;
    explosionSound();
    for(int x = 0; x < 10; x++){
        tm.displayText("------------");
        delay(100);
        tm.reset();
        tm.displayText("            ");
        delay(100);
    }
    delay(1000);
    reset();
    tm.reset();
}

void reset() {
    passwordInitial.setDefaultValues();
    passwordSave.setDefaultValues();
    explodeState = false;
    seconds = 0;
    countdown = 0;
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
        decreaseTime(0, 5);
        resetLeds();
        passwordSave.setDefaultValues();
        delay(200);
        return;
    }
    tm.displayText("SUCCESS");
    playDoom();
    delay(1000);
    reset();
}

void explosionSound() {
    int delayMulitplier = 2; 
    for(int x = 0; x < 300; x++) {
        tone(BUZZER, random(500, 1000), random(1, 5));
        //digitalWrite(BUZZER, HIGH);
        delay(1);
        //delay(random(1 * 10, 5 * 10));
    }
    noTone(BUZZER);
}

void inputCodeError() {
    tm.displayText("CODE");
    tone(BUZZER, 10000, 200);
    delay(1000);
    tm.reset();
}