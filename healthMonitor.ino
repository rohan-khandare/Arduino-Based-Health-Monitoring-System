#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 10
#define REPORTING_PERIOD_MS     1000
#define rs 12
#define en 11 
#define d4 5 
#define d5 4  
#define d6 3 
#define d7 2

int Contrast=99;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
 
PulseOximeter pox;
uint32_t tsLastReport = 0;
 
void onBeatDetected()
{
    Serial.println("Beat!");
}
 
void setup()
{   
    
    Serial.begin(115200);
    analogWrite(6, Contrast);
    Serial.print("Initializing pulse oximeter..");
   // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Oximeter");
    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
}
 
void loop()
{
    // Make sure to call update as fast as possible
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
 
        Serial.print("BPM: ");
        Serial.println(pox.getHeartRate());
        Serial.print("SpO2: ");
        Serial.print(pox.getSpO2());
        Serial.println("%");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("BPM: ");
        lcd.print(pox.getHeartRate());
        
        lcd.setCursor(0,1);
        lcd.print("SpO2: ");
        lcd.print(pox.getSpO2());
        lcd.print("%");
        sensors.setWaitForConversion(false);
        sensors.requestTemperatures();
        sensors.setWaitForConversion(free);
        int tempCelsius = sensors.getTempCByIndex(0);
        Serial.print("temperature:");
        Serial.println(tempCelsius);
        lcd.setCursor(9, 0);
        lcd.print("T: ");
        lcd.print(tempCelsius);
        lcd.print((char)223);  // Display degree symbol
        lcd.print("C");
        //delay(1000);
        
        tsLastReport = millis();
    }
}