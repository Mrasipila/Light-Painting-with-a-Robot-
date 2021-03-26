/*-----------------------------------------
          NUAGE.ino (v2)
        Boucle ouverte forcée avec while millis
  -----------------------------------------*/

/* Pour les pixels : */
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN       8
#define NUMPIXELS 7
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#include <motor.h>

// left and right motors with corresponding pins
Motor leftMotor(leftMotorPwmPin, leftMotorDirPin);
Motor rightMotor(rightMotorPwmPin, rightMotorDirPin);

// global variables
int uL = 0;
int uR = 0;
int i = 0;
int numLed=0;
int R,G,B =0;
unsigned long now, startTime, stopTime;


// ************** SETUP *********************
void setup() {
  // put your setup code here, to run once:
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
void loop() {
  delay(10000);

  for(i=0;i<7;i++)
  {
 //demitour en avant (cyan)
    neopixel(numLed,R,G,B);
    movewhile(200,50,1250);
    
    movewhile(0,0,100);
    
    neopixel(numLed,R,G,B);
    movewhile(-50,-200,1000);

    movewhile(0,0,100);
  }    
  leftMotor.stop();
  rightMotor.stop();
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
