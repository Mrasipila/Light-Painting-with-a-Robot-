/*-----------------------------------------
          POKEBALL.ino (v1)
         Sans correcteur
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
int R,G,B,numLed=0;
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
  //grand demi cercle
  neopixel(0,100,0,0);
  movewhile(100,150,5500);
  
  //grand demi cercle inv
  neopixel(0,100,100,100);
  movewhile(100,150,5000);

  //decalage
  movewhile(-50,50,600);
  
  //tout droit
  neopixel(0,100,100,0);
  movewhile(50,50,2500);

  //decalage
  movewhile(-50,50,200);
  
  //petit cercle
  movewhile(200,50,2800);
  
  //decalage
  movewhile(-50,50,250);
  
  //tout droit
  movewhile(50,50,2500);
        
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
