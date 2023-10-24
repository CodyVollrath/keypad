#include "LcdKeypad.h"


LcdKeypad::LcdKeypad(
        byte *rowPins, byte *colPins, 
        uint8_t rs, uint8_t enable,
		uint8_t d0, uint8_t d1, 
        uint8_t d2, uint8_t d3
    ) : Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS) {
        this->lcd = new LiquidCrystal(rs, enable, d0, d1, d2, d3);
        this->lcd->begin(16, 2);
}

LcdKeypad::~LcdKeypad() {

}

char LcdKeypad::getAndDisplayKey() {
    char key = Keypad::getKey();
    if (key && (key != '#' || key != '*')) {
        this->lcd->print(key);
    }
    return key;
}

void LcdKeypad::display(const char* text, uint8_t secondsToDisplay) {
    this->lcd->print(text);
    if (secondsToDisplay) {
        delay(secondsToDisplay * 1000);
        this->clear();
    }
}

void LcdKeypad::clear() {
    this->lcd->clear();
    this->setCursor(1, 0);
}

void LcdKeypad::setCursor(uint8_t row1, uint8_t row2) {
    this->lcd->setCursor(row1, row2);
}

