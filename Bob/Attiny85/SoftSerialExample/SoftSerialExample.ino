/*
  Software serial multiple serial test
 
 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.
 
 The circuit: 
 * RX is digital pin 2 (connect to TX of other device)
 * TX is digital pin 3 (connect to RX of other device)
 
 created back in the mists of time
 modified 9 Apr 2012
 by Tom Igoe
 based on Mikal Hart's example
 
 This example code is in the public domain.
 
 <SoftSerial> adapted from <SoftwareSerial> for <TinyPinChange> library which allows sharing the Pin Change Interrupt Vector.
 Single difference with <SoftwareSerial>: add #include <TinyPinChange.h>  at the top of your sketch.
 RC Navy (2012): http://p.loussouarn.free.fr
 
 */
#include <TinyPinChange.h>  /* Ne pas oublier d'inclure la librairie <TinyPinChange> qui est utilisee par la librairie <RcSeq> */
#include <SoftSerial.h>     /* Allows Pin Change Interrupt Vector Sharing */

#define ledPin 1
#define RX1_pin 2
#define TX1_pin 3
#define RX2_pin 4
#define TX2_pin 5


SoftSerial portOne(RX1_pin, TX1_pin);
//portTwo need to check
//SoftSerial portTwo(RX2_pin, TX2_pin);
void setup()  
{
 // Open serial communications and wait for port to open:

  // set the data rate for the SoftwareSerial port
  portOne.begin(9600);	//19200 4800
  portOne.println("Hello, world!!!");
/*
  portTwo.begin(9600);	//19200 4800
  portTwo.println("World, hello!!!");
*/
}

void loop() // run over and over
{
  portOne.print("Bob Love Kara~!!!");
//  portTwo.print("Kara Love Bob~!!!");

  // fade in from min to max in increments of 5 points:
  for(int fadeValue = 0 ;  255>= fadeValue; fadeValue +=51) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);

    // wait for 30 milliseconds to see the dimming effect    
    for(int index=0;index<10;index++)
    {
      if (portOne.available())
         portOne.write(portOne.read());
	 
	 portOne.print(index);
/*
	 if (portTwo.available())
         portTwo.write(portTwo.read()+1);
	 
      portTwo.println(' '); portTwo.print(index); portTwo.println(' ');
*/
	  delay(50);
    }
  } 
}
