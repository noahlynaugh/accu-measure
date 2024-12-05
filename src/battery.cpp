#include "battery.h"


//initialize the voltage reference library to read the Arduino's internal reference voltage
VoltageReference vRef;
const unsigned long batteryUpdateTime = 5000; //Interval the battery life will be updated in ms
unsigned long prevBatReadTime = 0; //the last time we read the battery in ms
uint8_t batteryLvl = 0; //the battery percentage

void initializeBattery(){
    vRef.begin();
}

//copied from Roberto Lo Giacco Battery Sense library
//the LiPo's remaining power is not linearly related to its voltage
//so we use a best fit line to approximate the remaining power percentage
//Function to get the battery level (returns 0-100)
uint8_t getBatteryLevel(uint16_t voltage, uint16_t minVoltage, uint16_t maxVoltage) {

    if (voltage <= minVoltage) {
    return 0; // Battery is fully discharged
    }

    // slow
    // uint8_t result = 110 - (110 / (1 + pow(1.468 * (voltage - minVoltage)/(maxVoltage - minVoltage), 6)));

    // steep
    // uint8_t result = 102 - (102 / (1 + pow(1.621 * (voltage - minVoltage)/(maxVoltage - minVoltage), 8.1)));

    // normal
    uint8_t result = 105 - (105 / (1 + pow(1.724 * (voltage - minVoltage) / (maxVoltage - minVoltage), 5.5)));
    return result >= 100 ? 100 : result;
}