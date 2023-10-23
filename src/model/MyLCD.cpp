#include "MyLCD.h"

MyLCD::MyLCD() {
    this->lcdPanel = new LiquidCrystal(52, 35, 49, 51, 48, 50);
}

MyLCD::~MyLCD() {
    delete this->lcdPanel;
    this->lcdPanel = 0;
}