#include "model/LcdKeypad.h"

class KeypadController {
    public:
        KeypadController();
        virtual ~KeypadController();
        void performKeyChecks();

    private:
        static const byte COLS = 4;
        static const byte ROWS = 4;
        bool isProgramMode = 1;
        bool isPromptingProgramMode;
        char keypadSeq[COLS];
        char password[COLS];
        LcdKeypad* keypad;

        // Helpers
        void enterProgramMode(char button);

        void takeKeyActions(char button, void (KeypadController::*terminationAction)());
        void checkPassword();
        void displayProgramMode();
        void setPasswordToKeypadSeq();
        void setKeysTo0();
        int comparePassword();
        void checkPassword();
        void addToKeySeq(char button);
        int getSizeOfNonZeroChars();

};