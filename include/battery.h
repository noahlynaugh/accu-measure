#ifndef BATTERY_H
#define BATTERY_H
#include <VoltageReference.h>

extern const unsigned long batteryUpdateTime; //how often we update the battery level in ms
extern unsigned long prevBatReadTime; //the last time we read the battery in ms
//our range for lipo voltage is 4-3.2V,
extern uint8_t batteryLvl; //the battery percentage
extern VoltageReference vRef;
#define MAX_BAT_VOLTAGE 4000 //max battery voltage
#define MIN_BAT_VOLTAGE 3200 //min battery voltage 

// Function declarations
uint8_t getBatteryLevel(uint16_t voltage, uint16_t minVoltage, uint16_t maxVoltage);
void displayBatteryPercent();
void initializeBattery();

#endif // battery_h