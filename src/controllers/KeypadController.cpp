#include "KeypadController.h"

KeypadController::KeypadController() {
    byte colPins[ROWS] = {5,4,3,2};
    byte rowPins[COLS] = {9,8,7,6};
    this->keypad = new LcdKeypad(rowPins, colPins, 52, 53, 51, 49, 47, 45);
    this->keypad->display("PM Mode On", 0);
    this->keypad->setCursor(0, 1);
}

KeypadController::~KeypadController() {
    //DTOR
}

// Public Methods
void KeypadController::performKeyChecks() {
    if (this->isProgramMode) {
      this->takeKeyActions(button, setPasswordToKeypadSeq);
    } 
    else if (button == '*') {
      this->enterProgramMode(button);
    }
    else {
      this->takeKeyActions(button, checkPassword);
    }
}

char KeypadController::getAndDisplayButton() {
    return this->keypad->getAndDisplay(key);
}


// Private Methods
void KeypadController::checkPassword() {
    int isPassword = this->comparePassword();
    if (isPassword) {
      this->keypad->display("Password Match", 0);
      if (this->isPromptingProgramMode) {
        this->isPromptingProgramMode = 0;
        this->isProgramMode = 1;
        this->displayProgramMode();
        return;
      }
      delay(1000);
      this->keypad->clear();
    } else {
      this->keypad->display("NO MATCH", 1);
    }
}
int KeypadController::getSizeOfNonZeroChars() {
    int count = 0;
    for (int i = 0; i < 4; i++) if (this->keypadSeq[i]) ++count;
    return count;
}

void KeypadController::addToKeySeq(char button) {
    for (int i = 0; i < 4; i++) {
      if (!this->keypadSeq[i]) {
        this->keypadSeq[i] = button;
        return;
      }
    }
}

void KeypadController::setKeysTo0() {
    for (int i = 0; i < 4; i++) this->keypadSeq[i] = 0;
}

int KeypadController::comparePassword() {
  for (int i = 0; i < 4; i++) {
    if (this->password[i] != this->keypadSeq[i]) {
      return 0;
    }
  }
  return 1;
}

void KeypadController::setPasswordToKeypadSeq() {
    for (int i = 0; i < 4; i++) {
      this->password[i] = this->keypadSeq[i];
    }
    
    this->isProgramMode = 0;
    this->keypad->display("Password Set", 1);
}

void KeypadController::displayProgramMode() {
    delay(1000);
    this->keypad->clear();
    this->keypad->display("PM Mode On", 0);
    this->keypad->setCursor(0,1);
}

void KeypadController::enterProgramMode(char button) {
    this->keypad->clear();
    if (this->isProgramMode) {
        return;
    }

    this->isPromptingProgramMode = 1;
    this->keypad->display("Entering PM", 1);
    this->keypad->display("Enter Password", 0);
    this->keypad->setCursor(0, 1);
    this->takeKeyActions(button, checkPassword);
}

void KeypadController::takeKeyActions(char button, void (KeypadController::*terminationAction)()) {
    int size = this->getSizeOfNonZeroChars();
    if (button != '#' && button != '*' && size < 4) {
        this->addToKeySeq(button);
    } else if (button == '#') {
        this->keypad->clear();
        (this->*terminationAction)();
    } else if (size == 4) {
        this->keypad->clear();
        this->keypad->display("Ressetting...", 1);
        this->setKeysTo0();
    }
}

