#include <Arduino.h>
#include "model/LcdKeypad.h"

const byte COLS = 4;
const byte ROWS = 4;

bool isProgramMode = 1;
bool isPromptingProgramMode = 0;

byte colPins[ROWS] = {5, 4, 3, 2};
byte rowPins[COLS] = {9, 8, 7, 6};
LcdKeypad keypad = LcdKeypad(rowPins, colPins, 52, 53, 51, 49, 47, 45);

// entered keys
char keypadSeq[4];
char password[4];

// Private Helpers Keypad
int getSizeOfNonZeroChars() {
  int count = 0;
  for (int i = 0; i < 4; i++) if (keypadSeq[i]) ++count;
  return count;
}

void addToKeySeq(char button) {
    for (int i = 0; i < 4; i++) {
      if (!keypadSeq[i]) {
        keypadSeq[i] = button;
        return;
      }
    }
}

int comparePassword() {
  for (int i = 0; i < 4; i++) {
    if (password[i] != keypadSeq[i]) {
      return 0;
    }
  }
  return 1;
}

void setKeysTo0() {
  for (int i = 0; i < 4; i++) keypadSeq[i] = 0;
}

void setPasswordToKeypadSeq() {
  for (int i = 0; i < 4; i++) {
    password[i] = keypadSeq[i];
  }
  
  isProgramMode = 0;
  keypad.display("Password Set", 1);
}

void displayProgramMode() {
    delay(1000);
    keypad.clear();
    keypad.display("PM Mode On", 0);
    keypad.setCursor(0, 1);
}

void checkPassword() {
  int isPassword = comparePassword();
  if (isPassword) {
    keypad.display("Password Match", 0);
    if (isPromptingProgramMode) {
      isPromptingProgramMode = 0;
      isProgramMode = 1;
      displayProgramMode();
      return;
    }
    delay(1000);
    keypad.clear();
  } else {
    keypad.display("NO MATCH", 1);
  }
}

void takeKeyActions(char button, void (*terminationAction)()) {
  int size = getSizeOfNonZeroChars();
  if (button != '#' && button != '*' && size < 4) {
    addToKeySeq(button);
  } else if (button == '#') {
    keypad.clear();
    terminationAction();
    setKeysTo0();
  } else if (size == 4) {
    keypad.clear();
    keypad.display("Resetting...", 1);
    setKeysTo0();
  }
}

void enterProgramMode(char button) {
  keypad.clear();
  if (isProgramMode) {
    return;
  }
  isPromptingProgramMode = 1;
  keypad.display("Entering PM", 1);
  keypad.display("Enter Password", 0);
  keypad.setCursor(0, 1);
  
  takeKeyActions(button, checkPassword);
}


void performKeyChecks(char button) {
  if (isProgramMode) {
    takeKeyActions(button, setPasswordToKeypadSeq);
  } 
  else if (button == '*') {
    enterProgramMode(button);
  }
  else {
    takeKeyActions(button, checkPassword);
  }
}
// End Private helpers


void setup() {
  Serial.begin(9600);
  keypad.display("Pm Mode On", 0);
  keypad.setCursor(0, 1);
}

void loop() {
  char button = keypad.getAndDisplayKey();
  if (button) {
    performKeyChecks(button);
  }
}