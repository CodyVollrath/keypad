#include <keypad.h>
#include <LiquidCrystal.h>

/**
 * LCDKeypad is a programmable keypad class that has an lCD and keypad attached
*/
class LcdKeypad : Keypad{
    public:

        /**
         * Creates an instance of the LcdKeypad
         * @param k_rowPins row pins for the key board
         * @param k_colPins column pins for the key board
         * @param rs the register select pin for the lcd
         * @param enable the enable pin for the lcd
         * @param d0 the first pin of the data array
         * @param d1 the second pin of the data array
         * @param d2 the third pin for the data array
         * @param d3 the fourth pin for the data array
         * @param d4 the fifth pin for the data array
        */
        LcdKeypad(byte *k_rowPins, byte *k_colPins, 
        uint8_t rs, uint8_t enable,
		uint8_t d0, uint8_t d1, 
        uint8_t d2, uint8_t d3);

        /**
         * Gets the key of a user entered key press and display it on the lcd
         * @return the character of the key entered
        */
        char getAndDisplayKey();

        /**
         * Sends text to lcd screen
         * @param text the text to send to the screen
         * @param secondsToDisplay the number of seconds the text will show on the screen
         * @note if secondsToDisplay is less than 1, it will show indefinitely. Will not show # or * keys
        */
        void display(const char* text, uint8_t secondsToDisplay);

        /**
         * Clears the lcd screen
         * @note Will set the cursor back to being at the first row of the lcd screen
        */
        void clear();

        /**
         * Changes the row to be first or second
         * @param row1 the top row (>0 is on, ==0 is off)
         * @param row2 the bottom row (>0 is on, ==0 is off)
        */
        void setCursor(uint8_t row1, uint8_t row2);

        virtual ~LcdKeypad();

    private:
        // Statics
        static const byte COLS = 4;
        static const byte ROWS = 4;
        //End Statics

        // Objects
        LiquidCrystal* lcd;
        // End Objects

        // Basic Variables
        char hexaKeys[ROWS][COLS] = {
          {'1', '2', '3', 'A'},
          {'4', '5', '6', 'B'},
          {'7', '8', '9', 'C'},
          {'*', '0', '#', 'D'}
        };
        // End basics
};