/*--------------------------------------------------------
   BORDURE.ino
   Boucle ouverte
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
int j = 0;
unsigned long totalTemps, totalTemps2;
unsigned long delayCommande;


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

  for(i=0;i<7;i++)
  {
    //demitour en avant (cyan)(delay1000)
    pixels.clear();
    pixels.show();
    pixels.setPixelColor(0, pixels.Color(10, 0, 5+i*5));
    pixels.show();
    uL = 200;
    uR = 50;
    totalTemps2 = millis();
    delayCommande=1100;
    
    while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(1250); 
    {
      totalTemps = millis();
      leftMotor.setU(uL);
      rightMotor.setU(uR);
    }
    
    uL = 0;
    uR = 0;
    
    totalTemps2 = millis();
    delayCommande=100;
    while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(100);
    {
      totalTemps = millis();
      leftMotor.setU(uL);
      rightMotor.setU(uR);
    }

    //demitour en arriere (violet)
    pixels.clear();
    pixels.show();
    pixels.setPixelColor(0, pixels.Color(0, 10,5+i*5 ));
    pixels.show();
    uL = 50;
    uR = 200;
    
    totalTemps2 = millis();
    delayCommande=950;
    while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(1000);
    {
      totalTemps = millis();
      leftMotor.setU(uL);
      rightMotor.setU(uR);
    }
    
    uL = 0;
    uR = 0;
    totalTemps2 = millis();
    delayCommande=100;
    while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(100);
    {
      totalTemps = millis();
      leftMotor.setU(uL);
      rightMotor.setU(uR);
    }

  }
  leftMotor.stop();
  rightMotor.stop();
}
