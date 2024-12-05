#include "screen.h"
#include "battery.h"

// Create the display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(message);
    display.display(); // Update the display
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

