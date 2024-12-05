#include "screen.h"
#include "battery.h"

// Create the display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const float cmToIn = 0.393701; //conversion from cm to in
const int32_t usInMin = 60 * 1000000; //amount of microseconds in a min

// Function to initialize the screen
void initializeScreen() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Halt execution
    }
    display.clearDisplay();
    display.display();
}

// Function to display a message
void displayMessage(const char* message) {
    // Clear only the text area
    display.fillRect(0, 20, SCREEN_WIDTH, 32, SSD1306_BLACK); // Clear message area
    display.setTextSize(4);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 20);
    display.println(message);
    display.display(); // Update the display
}

//displays the current encoder reading in either cm or in
void drawMeasurment() {
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

void displayBatteryPercent(){
    unsigned long currentTime = micros(); //current time to check time passed
    if (currentTime - prevBatReadTime > batteryUpdateTime) {
        batteryLvl = getBatteryLevel(vRef.readVcc(), MIN_BAT_VOLTAGE, MAX_BAT_VOLTAGE);
        prevBatReadTime = millis();
    }
    display.setCursor(64, 0);
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.print(batteryLvl);
    display.print(char(37)); //prints the % symbol
}

//Function to display battery life
void drawBatteryBox(int x, int y, uint8_t batteryLvl) {
    // Draw the Battery Rectangle Box
    display.fillRect(x, y, 18, 16, SSD1306_WHITE);
    display.fillRect(x+1,y+4,13,8, SSD1306_BLACK);
    display.fillRect(x+12,y,2,18, SSD1306_BLACK);
    display.fillRect(x,y,16,2, SSD1306_BLACK);
    display.fillRect(x,y+14,16,2, SSD1306_BLACK);
    display.fillRect(x+14,y+2,4,2, SSD1306_BLACK);
    display.fillRect(x+14,y+12,4,2, SSD1306_BLACK);
    if (batteryLvl>0 && batteryLvl<=25){
    //Display 25%
    display.fillRect(x+4, y+6, 1, 4, SSD1306_WHITE);
    }
    if (batteryLvl>25 && batteryLvl<=50){
    //Display 25%
    display.fillRect(x+4, y+6, 1, 4, SSD1306_WHITE);
    //Display 50%
    display.fillRect(x+6, y+6, 1, 4, SSD1306_WHITE);
    }
    if (batteryLvl>50 && batteryLvl<=75){
    //Display 25%
    display.fillRect(x+4, y+6, 1, 4, SSD1306_WHITE);
    //Display 50%
    display.fillRect(x+6, y+6, 1, 4, SSD1306_WHITE);
    //Display 75%
    display.fillRect(x+8, y+6, 1, 4, SSD1306_WHITE);
    }
    if (batteryLvl>75 && batteryLvl<=100){
    //Display 25%
    display.fillRect(x+4, y+6, 1, 4, SSD1306_WHITE);
    //Display 50%
    display.fillRect(x+6, y+6, 1, 4, SSD1306_WHITE);
    //Display 75%
    display.fillRect(x+8, y+6, 1, 4, SSD1306_WHITE);
    //Display 100%
    display.fillRect(x+10, y+6, 1, 4, SSD1306_WHITE);
    }
    // Update the display with the new frame
    display.display();
}

