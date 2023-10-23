#include <LiquidCrystal.h>
class MyLCD {
    public:
        MyLCD();
        virtual ~MyLCD();
        LiquidCrystal* lcdPanel;
};