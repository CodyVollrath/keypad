#include <Arduino.h>
#include <Keypad.h>

const byte COLS = 4;
const byte ROWS = 4;
const int RED_LED = 13;
const int BLUE_LED = 11;
const int GREEN_LED = 12;

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

// entered keys
char keypadSeq[4];
char password[4];

//Private Helper for Sound

void activateLight(int lightPin) {
  digitalWrite(lightPin, HIGH);
  digitalWrite(lightPin, LOW);
}
// End Private Helper

// Private Helpers Keypad
int getSizeOfNonZeroChars() {
  int count = 0;
  for (int i = 0; i < 4; i++) if (keypadSeq[i]) ++count;
  return count;
}

void addToKeySeq(char button) {
    Serial.println(button);
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
  Serial.println("Password is set... Program mode turning off!");
  isProgramMode = 0;
}

void checkPassword() {
  int isPassword = comparePassword();
  if (isPassword) {
    Serial.println("Password Matches with Keypad Sequence");
    if (isPromptingProgramMode) isProgramMode = 1;
  } else {
    Serial.println("NO MATCH!");
  }
  isPromptingProgramMode = 0;
}

void takeKeyActions(char button, void (*terminationAction)()) {
  int size = getSizeOfNonZeroChars();
  if (button != '#' && button != '*' && size < 4) {
    addToKeySeq(button);
  } else if (button == '#') {
    terminationAction();
    setKeysTo0();
  } else if (size == 4) {
    Serial.println("Too many key sequences, resetting...");
    setKeysTo0();
  }
}

void enterProgramMode(char button) {
  if (isProgramMode) {
    Serial.println("Programming Mode is already enabled");
    return;
  } else if (password) {
    Serial.println("Entering Programing Mode");
    Serial.println("Enter Password");
  } else {
    Serial.println("Password is not set and you are not in programming mode... Contact Administrator!");
  }

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
  pinMode(11, OUTPUT);
}

void loop() {
  char button = customKeypad.getKey();
  if (button) {
    performKeyChecks(button);
    digitalWrite(11, HIGH);
  }
  
}