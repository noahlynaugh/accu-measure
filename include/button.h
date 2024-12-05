#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class ButtonHandler {
private:
    uint8_t pin;      // Pin connected to the button
    bool lastState;   // Last recorded state of the button

public:
    // Constructor: Initialize the button on a specific pin
    ButtonHandler(uint8_t buttonPin);

    // Method to initialize the button
    void begin();

    // Method to check if the button is pressed
    bool isPressed();

    // Method to detect if the button state has changed
    bool stateChanged();
};

#endif // BUTTON_HANDLER_H