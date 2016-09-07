/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <idDHT11.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET 4


int idDHT11pin = 2; //Digital pin for comunications
int idDHT11intNumber = 0; //interrupt number (must be the one that use the previus defined pin (see table above)
// This wrapper is in charge of calling 
// mus be defined like this for the lib work

//declaration
void dht11_wrapper(); // must be declared before the lib initialization

// Lib instantiate
idDHT11 DHT11(idDHT11pin, idDHT11intNumber, dht11_wrapper);

void dht11_wrapper() {
  DHT11.isrCallback();
}

Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void teststext(int Row, char* string, float DigData ) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, Row);
//  display.clearDisplay();
  display.println(string);
  display.display();
  delay(1);

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(60, Row);
  display.println(DigData);
  display.display();
  delay(1);

//  display.clearDisplay();
//  delay(2000);

}

void setup()   {                
  Serial.begin(115200);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  // init done

  // draw scrolling text
//  teststext(0, "Temp: ", 30.5);
//  delay(2000);
  display.clearDisplay();
}

void loop() {

  int result = DHT11.acquireAndWait();
  switch (result)
  {
  case IDDHTLIB_OK: 
    Serial.println("OK"); 
    break;
  case IDDHTLIB_ERROR_CHECKSUM: 
    Serial.println("Error\n\r\tChecksum error"); 
    break;
  case IDDHTLIB_ERROR_ISR_TIMEOUT: 
    Serial.println("Error\n\r\tISR time out error"); 
    break;
  case IDDHTLIB_ERROR_RESPONSE_TIMEOUT: 
    Serial.println("Error\n\r\tResponse time out error"); 
    break;
  case IDDHTLIB_ERROR_DATA_TIMEOUT: 
    Serial.println("Error\n\r\tData time out error"); 
    break;
  case IDDHTLIB_ERROR_ACQUIRING: 
    Serial.println("Error\n\r\tAcquiring"); 
    break;
  case IDDHTLIB_ERROR_DELTA: 
    Serial.println("Error\n\r\tDelta time to small"); 
    break;
  case IDDHTLIB_ERROR_NOTSTARTED: 
    Serial.println("Error\n\r\tNot started"); 
    break;
  default: 
    Serial.println("Unknown error"); 
    break;
  }
display.clearDisplay();
  Serial.print("Humidity (%): ");
  Serial.println(DHT11.getHumidity(), 2);
//  teststext( 0, "Humi: ", DHT11.getHumidity());
  teststext( 0, "Humi: ", 12);

  Serial.print("Temperature (Celsius): ");
  Serial.println(DHT11.getCelsius(), 2);
//  teststext(18, "Temp: ", DHT11.getCelsius());
  teststext(18, "Temp: ", 28);
  delay(1000);
}


