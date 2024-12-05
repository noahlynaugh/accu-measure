#include "Button.h"

// Constructor: Store the pin number and initialize the lastState
ButtonHandler::ButtonHandler(uint8_t buttonPin) : pin(buttonPin), lastState(false) {}

// Initialize the button pin as input
void ButtonHandler::begin() {
    pinMode(pin, INPUT_PULLUP); // Use internal pull-up resistor
}

// Check if the button is currently pressed
bool ButtonHandler::isPressed() {
    return digitalRead(pin) == LOW; // Active LOW button
}

// Detect if the button state has changed since the last check
bool ButtonHandler::stateChanged() {
    bool currentState = digitalRead(pin) == LOW;
    bool changed = (currentState != lastState);
    lastState = currentState;
    return changed;
}