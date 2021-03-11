/*-----------------------------------------
          Pokeball, v1
        Richard et Falitiana, 2021
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
int j = 0;
unsigned long totalTemps, totalTemps2, delayCommande;

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
  pixels.clear();
  pixels.show();
  pixels.setPixelColor(0, pixels.Color(100, 0,0));//(rouge)
  pixels.show();
  //grand demi cercle
  uL=100; 
  uR=150;
  totalTemps2 = millis();
  delayCommande=5500;  
  while ((delayCommande+totalTemps2)>(totalTemps))//remplace 
    {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }

  pixels.clear();
  pixels.show();
  pixels.setPixelColor(0, pixels.Color(100, 100, 100));//(blanc)
  pixels.show();
    //grand demi cercle inv
  uL=100; 
  uR=150;
  totalTemps2 = millis();
  delayCommande=5000;  
  while ((delayCommande+totalTemps2)>(totalTemps))//remplace 
    {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }
  //decalage
  uL = -50;
  uR = 50;
  totalTemps2 = millis();
  delayCommande=600;
  while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(100);
    {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }
  pixels.clear();
  pixels.show();
  pixels.setPixelColor(0, pixels.Color(100, 100, 0));//(jaune)
  pixels.show();
  
//tout droit
  uL = 50;
  uR = 50;
  totalTemps2 = millis();
  delayCommande=2500;
  while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(100);
    {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }

  //decalage
  uL = -50;
  uR = 50;
  totalTemps2 = millis();
  delayCommande=200;
  while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(100);
    {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }
    
  //petit cercle
  uL = 200;
  uR = 50;
  totalTemps2 = millis();
  delayCommande=2800;
  while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(100);
    {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }
    //decalage
  uL = -50;
  uR = 50;
  totalTemps2 = millis();
  delayCommande=250;
  while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(100);
    {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }
//tout droit
  uL = 50;
  uR = 50;
  totalTemps2 = millis();
  delayCommande=2500;
  while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(100);
    {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }
      
  leftMotor.stop();
  rightMotor.stop();
}
