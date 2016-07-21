/*------------------------------------------------------------------------
  WS2811 demo
  ------------------------------------------------------------------------*/
#include "Ai_WS2811.h"

#define DATA_PIN 8
#define NUM_LEDS 8

Ai_WS2811 ws2811;


void setup()
{
  ws2811.init(DATA_PIN);
  Serial.begin(9600);
}

byte a = 0x00;
byte b = 0xff;
int curr = 0;

void loop()
{
  cli();
  for (byte i = 0; i < NUM_LEDS; ++i) {
    if (i == curr)
      ws2811.setColor(a, b, a);
    else if (i == curr - 1)
      ws2811.setColor(b, a, a);
    else if (i == curr - 2)
      ws2811.setColor(a, a, b);
    else
      ws2811.setColor(a, a, a);
    ws2811.send();
  }
  curr++;
  if (curr == NUM_LEDS)
    curr = 0;
  sei();
  _delay_ms(100);
}

