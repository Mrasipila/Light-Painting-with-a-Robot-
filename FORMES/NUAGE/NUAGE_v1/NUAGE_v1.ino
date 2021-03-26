/*-----------------------------------------
          NUAGE.ino (v1)
        boucle ouverte
  -----------------------------------------*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN       8
#define NUMPIXELS 7
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// left and right motors with corresponding pins
#include <motor.h>
Motor leftMotor(leftMotorPwmPin, leftMotorDirPin);
Motor rightMotor(rightMotorPwmPin, rightMotorDirPin);
// global variables
int uL = 0;
int uR = 0;



void setup() 
{
  Serial.begin(9600);
  // NEOPIXELS
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();
  pixels.clear();

  pixels.setPixelColor(0, pixels.Color(100, 0, 0));
  pixels.show();
}


void loop() 
{
  delay(10000);

  for(int i = 0; i < 7; i++)
  {
    //demi-tour en avant (cyan) (delay1000)
    pixels.clear();
    pixels.show();
    pixels.setPixelColor(0, pixels.Color(10, 0, 5+i*5));
    pixels.show();
    uL = 200;
    uR = 50;
    leftMotor.setU(uL);
    rightMotor.setU(uR);
    delay(1250);
    
    uL = 0;
    uR = 0;
    leftMotor.setU(uL);
    rightMotor.setU(uR);
    delay(100);
    
    //demi-tour en arriere (violet)
    pixels.clear();
    pixels.show();
    pixels.setPixelColor(0, pixels.Color(0, 10,5+i*5 ));
    pixels.show();
    uL = -50;
    uR = -200;
    leftMotor.setU(uL);
    rightMotor.setU(uR);
    delay(1000);
    
    uL = 0;
    uR = 0;
    leftMotor.setU(uL);
    rightMotor.setU(uR);
    delay(100);
  }
  
  leftMotor.stop();
  rightMotor.stop();
}
