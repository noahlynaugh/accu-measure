#include <Arduino.h>
#include <screen.h>
#include <button.h>
#include <Encoder.h>
#include <battery.h>

//function declarations
void runMeasuringWheel();
void drawMeasurement();
void centerString(String text, uint16_t yCoord, int textSize);
String doubleToString(double num, int decimal);

//--------------------//
//---Important Vars---//
//--------------------//
#define WHEEL_DIAM           8.95 //Measuring wheel diameter in cm
#define DEFAULT_UNITS_INCHES false //sets default units to either cm or inches
#define INCH_FRACT           16 //sets the fraction of an inch that the extra inch display will be rounded to

//-----------//
//---MODES---//
//-----------//
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

//------------------------------//
//---PROGRAM GLOBAL VARIABLES---//
//------------------------------//
boolean zeroButtonToggle = false;
boolean zeroButtonRun = true;
boolean previousZeroButtonState = HIGH;
boolean previousModeButtonState = HIGH;
unsigned long currentTime = micros();
unsigned long lastZeroPress = millis();

const float cmToIn = 0.393701; //conversion from cm to in
const int32_t usInMin = 60 * 1000000; //amount of microseconds in a min

//measurment variables
uint8_t fract;
long newPosition;
double displayPos;

//units for length and flag for changing them
String units;
boolean unitSwitch;

//---------------//
//---PIN SETUP---//
//---------------//
//Arduino Pro-Mini Pins for encoder and pushbuttons
//encoder must use at least one interrupt pin
#define ENCODER_PIN_1     2
#define ENCODER_PIN_2     3
// Create button objects for Mode Button and Zero Button
ButtonHandler modeButton(5);
ButtonHandler zeroButton(4);

//-------------------------//
//--- ENCODER/GEAR SETUP---//
//-------------------------//
//both work as interrupts so we should get good performance
Encoder myEnc(ENCODER_PIN_1, ENCODER_PIN_2);

#define WHEEL_DIAM_IN         WHEEL_DIAM * cmToIn //wheel diameter in inches
#define GEAR_RATIO            48/8 //number of outer gear teeth / number of inner gear teeth
#define ENC_STEPS_PER_REV     48 //number of encoder steps per revolution
//conversion from one revolution to linear distance traveled in cm
#define REV_TO_LIN_DIST_CM    M_PI * WHEEL_DIAM
//distance covered per encoder step in cm
#define ENC_STEP_LENGTH       REV_TO_LIN_DIST_CM / (ENC_STEPS_PER_REV * GEAR_RATIO)

//--------------------//
//---SETUP FUNCTION---//
//--------------------//
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

//---------------//
//---MAIN LOOP---//
//---------------//
void loop() {
  display.clearDisplay();
  //Displays the battery %
  displayBatteryPercent();
  //Display the battery life icon
  drawBatteryBox(113, 0, batteryLvl);
  display.display();
  while (mode == MEASURING_WHEEL) {
    runMeasuringWheel();
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
  

  //Displays the distance covered by the wheel and encoder (ie a ruler)
  //we always show a positive distance (although the reading can increment and decerement)
  void runMeasuringWheel() {
      yield();
      displayMessage("MSRMNT");
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

  //displays the current encoder reading in either cm or in
  void drawMeasurement() {
    display.setTextSize(3);
    if (unitSwitch) {
      displayPos = displayPos * cmToIn;  //convert from cm to in
      //work out the decimal portion in the nearest 1/16's of an inch
      fract = round(( displayPos - floor(displayPos) ) * INCH_FRACT);
      units = "in";
    } else {
      units = "cm";
    }
    //write out current reading and units
    centerString( doubleToString((double)displayPos, 2), 16, 3);
    display.setCursor(48, 35);
    display.print(units);
    if (unitSwitch) {
      //display the closest 1/16th decimal value
      display.setCursor(90, 45);
      display.setTextSize(1);
      display.print(fract);
      display.print("/");
      display.print(INCH_FRACT);
    }
  }

  //centers a string on the display at the specified y coordinate and text size
  void centerString(String text, uint16_t yCoord, int textSize) {
    //the number of pixels needed to the left of the string
    //found by subtracting the screen width from the total length of the string in pixels,
    //then dividing whats left by two (because we want the padding to be equal on left and right)
    int16_t padding = (SCREEN_WIDTH - (text.length() * BASE_FONT_LENGTH * textSize) ) / 2;
    //if the text string is too long, the padding will be negative
    //we set it to zero to avoid this
    if (padding < 0) {
      padding = 0;
    }
    //draw the text at the input y coord
    display.setTextSize(textSize);
    display.setCursor(padding, yCoord);
    display.print(text);
  }

  //takes a double and returns a string of the double to the input decimal places
  //uses the standard dtostrf() function and then trims the result to remove any leading spaces
  String doubleToString(double num, int decimal) {
    const int bufferSize = 10;
    char buffer[bufferSize];
    String tmpStr = dtostrf(num, bufferSize, decimal, buffer);
    tmpStr.trim();
    return tmpStr;
  }


