#include <RGBTools.h>

const byte LED_R = 11;
const byte LED_G = 9;
const byte LED_B = 10;

// set pins of red, green and blue
RGBTools RGB( LED_R, LED_G, LED_B, COMMON_ANODE);

void setup() {
  Serial.begin(115200);

  Serial.println(" === Set RED ===");
  RGB.setColor(Color::RED);
  delay(1000);

  Serial.println(" === Set GREEN ===");
  RGB.setColor(Color::GREEN);
  delay(1000);
  
  Serial.println(" === Set BLUE ===");
  RGB.setColor(Color::BLUE);
  delay(1000);
}

void loop() {

  int duration = 500, MaxLux = 100;

  Serial.println(" === Step1 === ");
  RGB.fadeTo( MaxLux, 0, 0, 50, duration); // to Red   in 50 steps and 3 seconds

  Serial.println(" === Step2 === ");
  RGB.fadeTo( 0, MaxLux, 0, 50, duration); // to Green in 50 steps and 3 seconds

  Serial.println(" === Step3 === ");
  RGB.fadeTo( 0, 0, MaxLux, 50, duration); // to Blue  in 50 steps and 3 seconds

}
