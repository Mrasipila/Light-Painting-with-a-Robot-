/*--------------------------------------------------------
   CC_BO.ino
    Structure de base
  ------------------------------------------------------*/


#include <motor.h>
// left and right motors with corresponding pins
Motor leftMotor(leftMotorPwmPin, leftMotorDirPin);
Motor rightMotor(rightMotorPwmPin, rightMotorDirPin);
/* Pour les pixels : */
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN       8
#define NUMPIXELS 7
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500
// global variables
int uL = 0;
int uR = 0;
int i = 0;
int R, G, B, numLed = 0;

unsigned long now, startTime;
unsigned long stopTime;


void setup()
{
  Serial.begin(9600);
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(100, 0, 0));
  pixels.show();
}


void loop() {
  delay(10000);
  
  //insert neopixel() and movewhile() commands here


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
