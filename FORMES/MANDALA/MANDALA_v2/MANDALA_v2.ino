/*--------------------------------------------------------
     MANDALA.ino (v2)
     Boucle ouverte avec while et millis
  ------------------------------------------------------*/

#include <motor.h>

/* Pour les pixels : */
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN       8
#define NUMPIXELS 7
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500


// left and right motors with corresponding pins
Motor leftMotor(leftMotorPwmPin, leftMotorDirPin);
Motor rightMotor(rightMotorPwmPin, rightMotorDirPin);

// global variables
int uL = 0;
int uR = 0;
int i=0;
int R,G,B,numLed=0;
unsigned long now, startTime;
unsigned long stopTime;
// ************** SETUP *********************
void setup() {
  Serial.begin(9600);
  // NEOPIXELS
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.clear();
  // for(int i=0; i<NUMPIXELS; i++)
  // {
  //    pixels.setPixelColor(i, pixels.Color(10, 10, 10));
  pixels.setPixelColor(0, pixels.Color(100, 0, 0));
  pixels.show();
}
// *************** LOOP **********************
void loop(){
  delay(10000);
 //spirale
  for (i=0;i<20;i++)
    {
      neopixel(0,0,100,0);
      movewhile(100-i*5,100,200);
    }
   movewhile(-50,50,600);
   movewhile(100,150,10750);   
   //decalage
   movewhile(-50,50,200);
      
//spirale cercle
  for (i=0;i<20;i++)
    {
      neopixel(0,0,100,0);
      movewhile(100-i*5,100,150);
    }
  leftMotor.stop();
  rightMotor.stop();

  delay(5000);
}

void neopixel(int ledNum, int R, int G, int B)
{
  pixels.clear();
  pixels.show();
  pixels.setPixelColor(ledNum, pixels.Color(R, G, B));
  pixels.show();
}

void movewhile (int uL, int uR, int stopTime)
{
  startTime = millis();
  while ((stopTime + startTime) > (now)) 
  {
    now = millis();
    leftMotor.setU(uL);
    rightMotor.setU(uR);
  }
}
