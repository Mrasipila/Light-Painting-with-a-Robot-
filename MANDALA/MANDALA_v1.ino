/*--------------------------------------------------------
      Mandala v1
     Boucle ouverte
  ------------------------------------------------------*/

#include <motor.h>
// left and right motors with corresponding pins
Motor leftMotor(leftMotorPwmPin, leftMotorDirPin);
Motor rightMotor(rightMotorPwmPin, rightMotorDirPin);
// global variables
int uL = 0;
int uR = 0;
int i = 0;
// Neopixels
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN       8
#define NUMPIXELS 7

// ***** SETUP ********
void setup()
{
  Serial.begin(9600);
  // NEOPIXELS
  Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();
  pixels.clear();
  delay(10000);
  pixels.setPixelColor(0, pixels.Color(10, 10, 10));
  pixels.show(); 
}


// ***** LOOP ********
void loop() 
{
  delay(10000);

  for (i = 0; i < 20; i++)
  {
    uL = 100 - i * 5;
    uR = 100;
    leftMotor.setU(uL);
    rightMotor.setU(uR);
    delay(200);
  }
  
  uL = -50;
  uR = 50;
  leftMotor.setU(uL);
  rightMotor.setU(uR);
  delay(250);
  
  //grand cercle
  uL = 100;
  uR = 150;
  leftMotor.setU(uL);
  rightMotor.setU(uR);
  delay(10000);

  // Demi tour
  uL = -50;
  uR = 50;
  leftMotor.setU(uL);
  rightMotor.setU(uR);
  delay(250);
  
  //spirale cercle
  for (i = 0; i < 20; i++)
  {
    uL = 100 - i * 5;
    uR = 100;
    leftMotor.setU(uL);
    rightMotor.setU(uR);
    delay(200);
  }
  
  leftMotor.stop();
  rightMotor.stop();
  delay(5000);
}
