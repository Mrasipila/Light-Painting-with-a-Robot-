/*--------------------------------------------------------
      Mandala v2
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
int leftInc = 10;
int rightInc = 10;
int i=0;
int j=0;
unsigned long totalTemps, totalTemps2;
unsigned long delayCommande;
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
      pixels.clear();
      pixels.show();
      pixels.setPixelColor(0, pixels.Color(0, 100, 0));
      pixels.show();
      uL = 100-i*5;
      uR = 100;
      totalTemps2 = millis();
      delayCommande=200;
    
      while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(1250); 
      {
        totalTemps = millis();
        leftMotor.setU(uL);
        rightMotor.setU(uR);
      }
    }
   
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

    
//grand cercle
  uL=100; 
  uR=150;
  totalTemps2 = millis();
  delayCommande=10750;  
  while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(1250); 
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
      
//spirale cercle
  for (i=0;i<20;i++)
    {
      pixels.clear();
      pixels.show();
      pixels.setPixelColor(0, pixels.Color(0, 100, 0));
      pixels.show();
      uL = 100-i*5;
      uR = 100;
      totalTemps2 = millis();
      delayCommande=150;
    
      while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(1250); 
      {
        totalTemps = millis();
        leftMotor.setU(uL);
        rightMotor.setU(uR);
      }
    }


  leftMotor.stop();
  rightMotor.stop();

  delay(5000);
}
  
