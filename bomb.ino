
#include <TM1638plus.h>

#define  STROBE_TM 4
#define  CLOCK_TM 6
#define  DIO_TM 7

int seconds = 0;
bool canClick = true;

TM1638plus tm(STROBE_TM, CLOCK_TM , DIO_TM, false);

void setup() {
    tm.displayBegin();
    //tm.displayText("hello world");
}

void loop() {   
    uint8_t buttons = tm.readButtons();
    switch(buttons) {
        case 1: increaseTime(200); break;
        case 2: decreaseTime(200); break;
        case 4: start(); break;
    }
    tm.displayIntNum(seconds, false);
}

void increaseTime(int time) {
    seconds++;
    delay(time);
    tm.reset();
}
void decreaseTime(int time) {
    seconds--;
    delay(time);
    tm.reset();
}
void start() {
    for(int x = seconds; x > 0; x--) {
        decreaseTime(1000);
        tm.displayIntNum(seconds, false);
    }
    explode();
}
void explode() {
    tm.displayText("BOOOOMMMM");
    delay(3000);
    tm.reset();
}