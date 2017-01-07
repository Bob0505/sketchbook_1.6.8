#include <LedControl.h>

#define NANO_SS		10
#define NANO_MOSI	11
#define NANO_MISO	12

#define ANNINY_MOSI	0
#define ANNINY_MISO	1
#define ANNINY_SCK	2

#define DIN_PIN	ANNINY_MISO
#define CS_PIN	ANNINY_MOSI
#define CLK_PIN	ANNINY_SCK

#define FRAME_NUM   3

// Pins: DIN,CLK,CS, # of Display connected
LedControl lc=LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);
unsigned long delayTime=500;  // Delay between Frames

// Put values in arrays
byte frame_array[FRAME_NUM][8] =
{
  {
   B11100111,
   B11000011,
   B10000001,
   B11000011,
   B11000011,
   B11000011,
   B10100101,
   B01011010
  },
  {
   B00011000,
   B00111100,
   B01111110,
   B00111100,
   B00111100,
   B00111100,
   B01011010,
   B10100101
  },
  {
   B01000000,
   B11000000,
   B01000000,
   B01000010,
   B01111100,
   B00111100,
   B00100100,
   B01101100
  }
};

void setup()
{
  lc.shutdown(0,false);  // Wake up displays
  lc.setIntensity(0, 5);  // Set intensity levels(0~15)	brightness
  lc.clearDisplay(0);  // Clear Displays
}

void show_frame(int frame_num)
{
  for(int j=0; j<frame_num ; j++)
  {
    for (int i=0; i<8; i++)  
      lc.setRow(0, i, frame_array[j][i]);

    delay(delayTime);
  }
}

void loop()
{
	show_frame(FRAME_NUM);

}