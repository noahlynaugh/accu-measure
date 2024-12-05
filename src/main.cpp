#include <Arduino.h>
#include <screen.h>
#include <button.h>
#include <Encoder.h>
#include <battery.h>

#define DEFAULT_UNITS_INCHES false //sets default units to either cm or inches

#define INCH_FRACT           16 //sets the fraction of an inch that the extra inch display will be rounded to

// ================================================================
// ===                         MODES                            ===
// ================================================================
//Defines the order of modes, you can re-order modes as you wish
//you can remove modes by setting the mode number to greater than the NUM_MODES ex: 999
//if you do this don't forget to decrease NUM_MODES to match the number of modes remaining
//and also change the order of the remaining modes (going from 0 to however remain)
#define MEASURING_WHEEL  0
#define DIAM_MEASURE     1
#define MEASURING_INSIDE 2
#define TACHOMETER       3

uint8_t NUM_MODES =      4; //total number of active modes

volatile uint8_t mode =  0; //inital mode

// ================================================================
// ===                         PIN SETUP                        ===
// ================================================================
//Arduino Pro-Mini Pins for encoder and pushbuttons
//encoder must use at least one interrupt pin
#define ENCODER_PIN_1     2
#define ENCODER_PIN_2     3
//both work as interrupts so we should get good performance
Encoder myEnc(ENCODER_PIN_1, ENCODER_PIN_2);
// Create button objects for Mode Button and Zero Button
ButtonHandler modeButton(5);
ButtonHandler zeroButton(4);


//--------------------///
//---SETUP FUNCTION---///
//--------------------///
void setup() { 
// Initialize buttons
modeButton.begin();
zeroButton.begin();
//start reading the internal voltage reference, and take an inital reading
initializeBattery();
batteryLvl = getBatteryLevel(vRef.readVcc(), MIN_BAT_VOLTAGE, MAX_BAT_VOLTAGE);

//Write the Builtin LED to verify running code
pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN, HIGH);
//Initialize the screen
initializeScreen();
}

//---------------///
//---MAIN LOOP---///
//---------------///
void loop() {
  display.clearDisplay();
  //Displays the battery %
  displayBatteryPercent();
  //Display the battery life icon
  drawBatteryBox(113, 0, batteryLvl);
  display.display();

  //Displays the distance covered by the wheel and encoder (ie a ruler)
  //we always show a positive distance (although the reading can increment and decerement)
  void runMeasuringWheel() {
      yield();
      display.clearDisplay();
      drawHeader("Measuring Wheel");

      newPosition = myEnc.read();
      //we always show a positive distance
      //the user can zero it if needed
      //the position calculated by multiplying the steps by the linear distance covered by one encoder step
      displayPos = abs(newPosition) * ENC_STEP_LENGTH;
      //zeros the encoder count
      if (!zeroButtonRun) {
        myEnc.write(0);
        zeroButtonRun = true;
      }
      drawMeasurement();
      display.setCursor(0, 57);
      display.setTextSize(1);
      display.print("Wheel Radius: ");
      if (unitSwitch) {
        display.print(WHEEL_DIAM_IN / 2);
      } else {
        display.print(WHEEL_DIAM / 2);
      }
      display.print(units);
      display.display();
  }

  // Detect state changes for Button 1
  // if (button1.stateChanged()) {
  //     Serial.println("Button 1 state changed!");
  //     displayMessage("Right");
  //     delay(2000);
  //     return;
  // }

  // Optional: Detect state changes for Button 2
  // if (button2.stateChanged()) {
  //     Serial.println("Button 2 state changed!");
  //     displayMessage("Left");
  //     delay(2000);
  //     return;
  }

