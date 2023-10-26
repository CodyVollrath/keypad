#include <Arduino.h>
#include "controllers/KeypadController.h"

KeypadController controller;

void setup() {
  Serial.begin(9600);
  controller = KeypadController();
}

void loop() {
  controller.performKeyChecks();
}