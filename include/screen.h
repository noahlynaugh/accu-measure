#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Screen dimensions and address
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

//Units
const float cmToIn = 0.393701; //conversion from cm to in
const int32_t usInMin = 60 * 1000000; //amount of microseconds in a min
//measurment variables
uint8_t fract;
long newPosition;
double displayPos;
//units for length and flag for changing them
String units;
boolean unitSwitch;

// Declare the display object
extern Adafruit_SSD1306 display;

// Function declarations
void initializeScreen();
void displayMessage(const char* message);
void drawBatteryBox(int x, int y, uint8_t batteryLvl);
void displayBatteryPercent();
void drawMeasurment();

#endif // SCREEN_H