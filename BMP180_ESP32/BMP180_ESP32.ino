/*
 * Demo program to add your Nuttyfi, NodeMCU ESP8266 or ESP32 wifi board to 
 * Blynk2.0 IoT mobile app & web dashboard
 * This program is property of SME Dehradun. for any query related to this program, 
 * contact us at www.smedehradn.com
 * if your want any solution related for any IoT Customized Boards and Sensor, 
 * then contact to www.nuttyengineer.com 
 */
// Replace/ Fill-in information from your Blynk Template here

#define BLYNK_TEMPLATE_ID "Change it"
#define BLYNK_DEVICE_NAME "Change it"

#define BLYNK_FIRMWARE_VERSION "0.1.0"
#define BLYNK_PRINT Serial
#include "BlynkEdgent.h"

#include <SFE_BMP180.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 

SFE_BMP180 pressure;
#define ALTITUDE 430.0 // Sea Level Altitude in SME, Dehradun Head Quarter  (in meter)
char status;
double T,P,p0,a; 
/***********Begining of bmpData() function to measure the data********/
void bmpData()
{
  display.clearDisplay();
  display.setTextSize(2);      
  display.setTextColor(WHITE); // 
  display.setCursor(1, 1);     // Start at top-left corner
  display.print("    SME    ");

  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print(" deg C, ");
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.println(" deg F");
      
      display.setTextSize(1.8);      
      display.setTextColor(WHITE); // 
      display.setCursor(1, 25);     // Start at top-left corner
      display.print("TEMP:");
      display.print(T,2);
      display.println(" deg C");
     Blynk.virtualWrite(V0, T);

      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");

          display.setTextSize(1.7);      
          display.setTextColor(WHITE); // 
          display.setCursor(1, 35);     // Start at top-left corner
          display.print("Pressure: ");
          display.print(P,2);
          display.println(" mb "); // millibar
          Blynk.virtualWrite(V1, P);

          p0 = pressure.sealevel(P,ALTITUDE); // we're at 430 meters (Dehradun, Uttarakhand)
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, "); // millibar
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");

          display.setTextSize(1.5);      
          display.setTextColor(WHITE); // 
          display.setCursor(1, 45);     // Start at top-left corner
          display.print("SL pressure:");
          display.print(p0,2);
          display.println(" mb"); // millibar
          Blynk.virtualWrite(V2, p0);

          a = pressure.altitude(P,p0);
          Serial.print("computed altitude: ");
          Serial.print(a,0);
          Serial.print(" meters, ");
          Serial.print(a*3.28084,0);
          Serial.println(" feet");

          display.setTextSize(1.5);      
          display.setTextColor(WHITE); // 
          display.setCursor(1, 55);     // Start at top-left corner
          display.print("Altitude:");
          display.print(a,0);
          display.println(" meter");
          Blynk.virtualWrite(V3, a);
          display.display();
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
  delay(2000);
}
/***********End of the bmpData() function********/

void init_sensor()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  
  Serial.println("Initializing OLED Display");
    display.clearDisplay();
  display.setTextSize(1.9);      
  display.setTextColor(WHITE); // Draw Yellow text
  display.setCursor(1, 35);     // Start at top-left corner
  display.print("BMP180 Sensor interfacing with Blynk2.0");
  display.display();
  delay(2000);
  
  if (pressure.begin()) 
  {
    Serial.println("BMP180 init success");
    display.clearDisplay();
    display.setTextSize(1.8);      
    display.setTextColor(WHITE); // Draw Yellow text
    display.setCursor(1, 40);     // Start at top-left corner
    display.print("BMP180 init success");
    display.display();
    delay(2000);
  }
    else
  {
    Serial.println("BMP180 init fail\n\n");
    display.clearDisplay();
    display.setTextSize(1.8);      
    display.setTextColor(WHITE); // Draw Yellow text
    display.setCursor(1, 40);     // Start at top-left corner
    display.print("BMP180 init fail");
    display.display();
    while(1); // Pause forever.
  } 

}
void setup()
{
  Serial.begin(9600);
  BlynkEdgent.begin();
  delay(2000);
  init_sensor();
}

void loop() 
{
    BlynkEdgent.run();
    bmpData();
}
