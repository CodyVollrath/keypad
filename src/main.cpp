#include <Arduino.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

const byte COLS = 4;
const byte ROWS = 4;

bool isProgramMode = 1;
bool isPromptingProgramMode = 0;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'}, 
  {'*', '0', '#', 'D'}
};

byte colPins[ROWS] = {5, 4, 3, 2};
byte rowPins[COLS] = {9, 8, 7, 6};


// Interface with keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Interface with LCD
LiquidCrystal lcd = LiquidCrystal(52, 53, 51, 49, 47, 45);                                                                                                                                                                                  

// entered keys
char keypadSeq[4];
char password[4];

//Private Helper for LCD

void clear() {
  lcd.clear();
  lcd.setCursor(1,0);
}

// End Private Helper

// Private Helpers Keypad
int getSizeOfNonZeroChars() {
  int count = 0;
  for (int i = 0; i < 4; i++) if (keypadSeq[i]) ++count;
  return count;
}

void addToKeySeq(char button) {
    lcd.print(button);
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
  clear();
  for (int i = 0; i < 4; i++) {
    password[i] = keypadSeq[i];
  }
  lcd.print("Password Set");
  isProgramMode = 0;
  delay(1000);
  clear();
  
}

void checkPassword() {
  clear();
  int isPassword = comparePassword();
  if (isPassword) {
    lcd.print("Password Matches");
    if (isPromptingProgramMode) {
      isPromptingProgramMode = 0;
      isProgramMode = 1;
      delay(1000);
      clear();
      lcd.print("PM Mode On");
      lcd.setCursor(0, 1);
      
      return;
    }
  } else {
    lcd.print("NO MATCH!");
  }
  
  delay(1000);
  clear();
  
}

void takeKeyActions(char button, void (*terminationAction)()) {
  int size = getSizeOfNonZeroChars();
  if (button != '#' && button != '*' && size < 4) {
    addToKeySeq(button);
  } else if (button == '#') {
    clear();
    terminationAction();
    setKeysTo0();
  } else if (size == 4) {
    clear();
    lcd.print("resetting...");
    setKeysTo0();
    delay(1000);
    clear();
  }
}

void enterProgramMode(char button) {
  clear();
  if (isProgramMode) {
    return;
  }
  lcd.print("Entering PM");
  delay(1000);
  clear();
  lcd.print("Enter Password");
  lcd.setCursor(0, 1);
  isPromptingProgramMode = 1;
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
  lcd.begin(16, 2);
  lcd.print("PM Mode On");
  lcd.setCursor(0, 1);
}

void loop() {
  char button = customKeypad.getKey();
  if (button) {
    performKeyChecks(button);
  }
}