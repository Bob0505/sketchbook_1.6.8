/*

 */

#if defined(ESP8266)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

#include <RtcDS1307.h>
#include <Wire.h>
#include <Adafruit_HTU21DF.h>
#include <SPI.h>
#include <SD.h>

// Connect Vin to 3-5VDC
// Connect GND to ground
// Connect SCL to I2C clock pin (A5 on UNO)
// Connect SDA to I2C data pin (A4 on UNO)
const int LED_PIN = 13;

// change this to match your SD shield or module;
//     Arduino Ethernet shield: pin 4
//     Adafruit SD shields and modules: pin 10
//     Sparkfun SD shield: pin 8
const int chipSelect = 10;

RtcDS1307 Rtc;
Adafruit_HTU21DF htu = Adafruit_HTU21DF();
File myFile;

//=======================================	Tiny_RTC	Start

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.println(datestring);
}
                //"DATE","TIME","TEMP","HUM"
                //08/17/2016 15:47:43
void printCsv(const RtcDateTime& dt, float Temp, float Hum)
{
    char datestring[50];
    //Integer
    int TempI = (int)Temp, HumI = (int)Hum;
    //Decimal
    int TempD = (int)(100*Temp-100*TempI), HumD = (int)(100*Hum-100*HumI);

    snprintf_P(datestring, 
            countof(datestring),
                 //"     DATE      ", "      TIME     ", "TEMP",  " HUM "
            PSTR("\"%02u/%02u/%04u\",\"%02u:%02u:%02u\",\"%d.%02d\",\"%d.%02d\""),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second(),
            TempI, TempD,
            HumI, HumD);

    //"DATE","TIME","TEMP","HUM"
    myFile.println(datestring);
}
//=======================================	Tiny_RTC	End

void setup()
{
//  string fileName = "Temp_log.csv";
 // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
//=======================================	Tiny_RTC	Start
    Serial.print("compiled: ");    
	RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);

    //--------RTC SETUP ------------
    Rtc.Begin();

    // if you are using ESP-01 then uncomment the line below to reset the pins to
    // the available pins for SDA, SCL
    // Wire.begin(0, 2); // due to limited pins, use pin 0 and 2 for SDA, SCL
    if (!Rtc.IsDateTimeValid()) 
    {
        // Common Cuases:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");

        // following line sets the RTC to the date & time this sketch was compiled
        // it will also reset the valid flag internally unless the Rtc device is
        // having an issue

        Rtc.SetDateTime(compiled);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low); 
//=======================================	Tiny_RTC	End

//=======================================	HTU21DF	Start
  Serial.println("HTU21D-F test");
  pinMode(LED_PIN, OUTPUT);

  if (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }

//=======================================	HTU21DF	End

  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(SS, OUTPUT); //Pin10
//=======================================	SD	Start
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  if (SD.exists("Temp_log.csv")) {
    Serial.println("Temp_log.csv exists.");
  } else {
    Serial.println("Temp_log.csv doesn't exist.");
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    myFile = SD.open("Temp_log.csv", FILE_WRITE);

    // if the file opened okay, write to it:
    if (myFile) {
      Serial.print("Writing to Temp_log.csv...");
      myFile.println("\"DATE\",\"TIME\",\"TEMP\",\"HUM\"");
	  // close the file:
      myFile.close();
      Serial.println("done.");
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening Temp_log.csv");
    }
  }
  //=======================================	SD	End
}

void loop()
{
  digitalWrite(LED_PIN, HIGH);
//=======================================	Tiny_RTC	Start
    if (!Rtc.IsDateTimeValid()) 
    {
        // Common Cuases:
        //    1) the battery on the device is low or even missing and the power line was disconnected
        Serial.println("RTC lost confidence in the DateTime!");
    }

    RtcDateTime now = Rtc.GetDateTime();

    printDateTime(now);
//=======================================	Tiny_RTC	End

//=======================================	HTU21DF	Start
  float Temperature = htu.readTemperature(), Humidity = htu.readHumidity();
  Serial.ardprt("Tmp: %f, Hum:%f", Temperature, Humidity);    //TEST
//=======================================	HTU21DF	End

//=======================================	SD	Start
  myFile = SD.open("Temp_log.csv", FILE_WRITE);
  if (myFile) {
    printCsv(now, Temperature, Humidity);
	// close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening Temp_log.csv");
    if (!SD.begin(chipSelect)) {
      Serial.println("initialization failed!");
    } else {
      Serial.println("initialization done.");
    }
  }
//=======================================	SD	End
  digitalWrite(LED_PIN, LOW);
//  Serial.println(" === === === === ");
  delay(5*1000); //sec
}
