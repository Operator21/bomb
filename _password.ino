#include <TM1638plus.h>

class Password {
    int password[8];
    int arrayIndexPosition;

    public:
        Password();
        void setSavePassword();
        void displayPasswordSize(TM1638plus tm);
        void setDefaultValues();
        bool checkPasswordSet();
        void setCurrentArrayIndexValue(int value);
        bool compareWith(Password &second);

};

Password::Password() {
    setDefaultValues();
}

void Password::setSavePassword() {
    for(int x = 0; x < 8; x++) {
        setCurrentArrayIndexValue(16);
    }
}

void Password::displayPasswordSize(TM1638plus tm) {
    for(int x = 0; x < 8; x++) {
        if(password[x] > 0)
            tm.setLED(x, 1);
    }
}

void Password::setDefaultValues() {
    for(int x = 0; x < 8; x++) {
        password[x] = 0;
    }
    arrayIndexPosition = 0;
}

bool Password::checkPasswordSet(){
    if(arrayIndexPosition == 8)
        return true;
    return false;
}

void Password::setCurrentArrayIndexValue(int value) {
    if(arrayIndexPosition < 8) {
        password[arrayIndexPosition] = value;
        arrayIndexPosition++;
        delay(100);
    }
}

bool Password::compareWith(Password &second) {
    for(int x = 0; x < 8; x++){
        if(password[x] != second.password[x])
            return false;
    }
    return true;
}