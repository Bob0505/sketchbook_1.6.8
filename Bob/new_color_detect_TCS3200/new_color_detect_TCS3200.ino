// The three-primary colours of light : RGB

#include <Timer.h>
Timer timer;

const byte S0 = 3;	/* pinB */
const byte S1 = 4;	/* pinA */
const byte S2 = 5;	/* pinE */
const byte S3 = 6;	/* pinF */
const byte TAOS_OUT_PIN = 2;	/* pinC */
const byte LED = 13;	/* pinD */
const byte LED_R = 8;
const byte LED_G = 9;
const byte LED_B = 10;

//Output frequency scaling selection inputs.
//Power off, Two PERCENT, Twenty PERCENT, Hundred PERCENT
enum OFSSI {E_PowerOff, E_TwoPer, E_TwePer, E_HunPer};
enum COLOR { E_white, E_red, E_blue, E_green}; 


void doAfter()
{
  // 500m sec tick started id
  int tickEvent = timer.every(500, detectColor);
}

void setup() {
  TCS3200setup();
  Serial.begin(115200);
  pinMode(LED_R,OUTPUT);
  pinMode(LED_G,OUTPUT);
  pinMode(LED_B,OUTPUT);

  //delay(100);
  int afterEvent = timer.after(100, doAfter);
}

// primary loop takes color readings from all four channels and displays the raw values once per second.  What you might wish to do with those values is up to you...
void loop() {

  //detectColor(TAOS_OUT_PIN);
  //Serial.print("nnn");
  //delay(500);
  timer.update();

}

void detectColor(void) {

  float white = colorRead( E_white, 1);
  float red = colorRead( E_red, 1);
  float blue = colorRead( E_blue, 1);
  float green = colorRead( E_green, 1);

  red = map(red, 0, 180, 0, 100);
  blue = map(blue, 0, 100, 0, 100);
  green = map(green, 0, 170, 0, 100);
  Serial.print("white ");
  Serial.println(white);

  Serial.print("red ");
  Serial.println(red);

  Serial.print("blue ");
  Serial.println(blue);

  Serial.print("green ");
  Serial.println(green);
 
  if (white < 30) {
    if (red < blue && red < green) {
      digitalWrite(LED_R, LOW);
      Serial.println("RED");
    }else{
      digitalWrite(LED_R, HIGH);
    }
    if (blue < red && blue < green) {
      digitalWrite(LED_B, LOW);
      Serial.println("BLUE");
    }else{
      digitalWrite(LED_B, HIGH);
    }
    if (green < red && green < blue) {
      digitalWrite(LED_G, LOW);
      Serial.println("GREEN");
    }else{
      digitalWrite(LED_G, HIGH);
    }
  }
  Serial.println("=====================");
}

/*
This section will return the pulseIn reading of the selected color.
It will turn on the sensor at the start taosMode(E_HunPer), and it will power off the sensor at the end taosMode(E_PowerOff)
color codes: 0=white, 1=red, 2=blue, 3=green
if LEDstate is 0, LED will be off. 1 and the LED will be on.
TAOS_OUT_PIN is the ouput pin of the TCS3200.
*/
float colorRead(enum COLOR color, boolean LEDstate) {

  //turn on sensor and use highest frequency/sensitivity setting
  taosMode(E_HunPer);

  //setting for a delay to let the sensor sit for a moment before taking a reading.
  int sensorDelay = 1;	//100

  //set the S2 and S3 pins to select the color to be sensed
  if (E_white == color) { //white
    digitalWrite(S3, LOW); //S3
    digitalWrite(S2, HIGH); //S2
    // Serial.print(" w");
  }

  else if (E_red == color) { //red
    digitalWrite(S3, LOW); //S3
    digitalWrite(S2, LOW); //S2
    // Serial.print(" r");
  }

  else if (E_blue == color) { //blue
    digitalWrite(S3, HIGH); //S3
    digitalWrite(S2, LOW); //S2
    // Serial.print(" b");
  }

  else if (E_green == color) { //green
    digitalWrite(S3, HIGH); //S3
    digitalWrite(S2, HIGH); //S2
    // Serial.print(" g");
  }  
  else{

  }
  // create a var where the pulse reading from sensor will go
  float readPulse;

  //  turn LEDs on or off, as directed by the LEDstate var
  if (LEDstate == 0) {
    digitalWrite(LED, LOW);
  }
  if (LEDstate == 1) {
    digitalWrite(LED, HIGH);
  }

  // wait a bit for LEDs to actually turn on, as directed by sensorDelay var
  delay(sensorDelay);

  // now take a measurement from the sensor, timing a low pulse on the sensor's "out" pin
  readPulse = pulseIn(TAOS_OUT_PIN, LOW, 80000);

  //if the pulseIn times out, it returns 0 and that throws off numbers. just cap it at 80k if it happens
  if (readPulse < .1) {
    readPulse = 80000;
  }

  //turn off color sensor and LEDs to save power
  taosMode(E_PowerOff);

  // return the pulse value back to whatever called for it...
  return readPulse;

}

// Operation modes area, controlled by hi/lo settings on S0 and S1 pins.
//setting mode to zero will put taos into low power mode. taosMode(E_PowerOff);

void taosMode(enum OFSSI mode) {

  if (E_PowerOff == mode) {
    //power OFF mode-  LED off and both channels "low"
    digitalWrite(LED, LOW);
    digitalWrite(S0, LOW); //S0
    digitalWrite(S1, LOW); //S1
    //  Serial.println("mOFFm");

  } else if (E_HunPer == mode) {
    //this will put in 1:1, highest sensitivity
    digitalWrite(S0, HIGH); //S0
    digitalWrite(S1, HIGH); //S1
    // Serial.println("m1:1m");

  } else if (E_TwePer == mode) {
    //this will put in 1:5
    digitalWrite(S0, HIGH); //S0
    digitalWrite(S1, LOW); //S1
    //Serial.println("m1:5m");

  } else if (E_TwoPer == mode) {
    //this will put in 1:50
    digitalWrite(S0, LOW); //S0
    digitalWrite(S1, HIGH); //S1
    //Serial.println("m1:50m");
  } else {
      
  }

  return;

}

void TCS3200setup() {

  //initialize pins
  pinMode(LED, OUTPUT); //LED pinD

  //color mode selection
  pinMode(S2, OUTPUT); //S2 pinE
  pinMode(S3, OUTPUT); //s3 pinF

  //color response pin (only actual input from taos)
  pinMode(TAOS_OUT_PIN, INPUT); //TAOS_OUT_PIN pinC

  //communication freq (sensitivity) selection
  pinMode(S0, OUTPUT); //S0 pinB
  pinMode(S1, OUTPUT); //S1 pinA

  return;

}
