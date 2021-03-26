/*--------------------------------------------------------
      esiea.ino (v1)

 ------------------------------------------------------*/


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

void loop() {
  // put your main code here, to run repeatedly:
    delay(10000);
  pixels.clear();
  pixels.show();
  pixels.setPixelColor(0, pixels.Color(100, 00,00));//(blanc)
  pixels.show();

  //e1
  uR=70;
  uL=70;
  totalTemps2 = millis();
  delayCommande=1500;
  while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(100);
    {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }
   
    //décalage
      uL = 0;
      uR = 50;
      totalTemps2 = millis();
      delayCommande=900;
      while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(100);
      {
        totalTemps = millis();
        leftMotor.setU(uL);
        rightMotor.setU(uR);
      }

    // cercle e
    uL=65; 
    uR=200;
    totalTemps2 = millis();
    delayCommande=4000;  
    while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(1250);   
      {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }
     leftMotor.stop();
    rightMotor.stop();
    pixels.clear();
    pixels.show();
    pixels.setPixelColor(0, pixels.Color(0, 00, 00));//(blanc)
    pixels.show();
    //delay(10000);
    pixels.setPixelColor(0, pixels.Color(100,00, 00));//(blanc)
    pixels.show();
    
    //et c'est parti pour le s
    uL=50; 
    uR=200;
    totalTemps2 = millis();
    delayCommande=2000;  
    while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(1250);   
      {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }

       // cercle s
    uL=200; 
    uR=50;
    totalTemps2 = millis();
    delayCommande=1500;  
    while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(1250);   
      {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }
    
    leftMotor.stop();
    rightMotor.stop();
    pixels.clear();
    pixels.show();
    pixels.setPixelColor(0, pixels.Color(0, 00, 00));//(blanc)
    pixels.show();
    //  delay(10000);
      pixels.setPixelColor(0, pixels.Color(100, 00, 00));//(blanc)
   
   pixels.show();
   
    // i comme "ingénieurs"
     uR=76;
     uL=70;
  totalTemps2 = millis();
  delayCommande=1500;
  while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(100);
    {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }
    pixels.clear();
    pixels.show();
    
     uR=76;
     uL=70;
  totalTemps2 = millis();
  delayCommande=600;
  while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(100);
    {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }
    leftMotor.stop();
    rightMotor.stop();
    
    pixels.setPixelColor(0, pixels.Color(100, 00, 00));//(blanc) 
    pixels.show();
     delay(1000);
    pixels.clear();
    pixels.show();
    pixels.setPixelColor(0, pixels.Color(00, 0, 00));//(blanc)
   pixels.show();
      //delay(10000);
       pixels.setPixelColor(0, pixels.Color(100, 00, 00));
        pixels.show();
        
    
  //e2
  uR=70;
  uL=70;
  totalTemps2 = millis();
  delayCommande=1500;
  while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(100);
    {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }
   
    //décalage
      uL = 0;
      uR = 50;
      totalTemps2 = millis();
      delayCommande=900;
      while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(100);
      {
        totalTemps = millis();
        leftMotor.setU(uL);
        rightMotor.setU(uR);
      }

    // cercle e
    uL=65; 
    uR=200;
    totalTemps2 = millis();
    delayCommande=4000;  
    while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(1250);   
      {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }
     leftMotor.stop();
    rightMotor.stop();
    pixels.clear();
    pixels.show();
    pixels.setPixelColor(0, pixels.Color(0, 00, 00));//(blanc)
    pixels.show();
   // delay(10000);
    pixels.setPixelColor(0, pixels.Color(100, 0, 00));//(blanc)
    pixels.show();
    
      

  //a
  uR=70;
  uL=70;
  totalTemps2 = millis();
  delayCommande=1500;
  while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(100);
    {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }
   
    //décalage
      uL = 0;
      uR = 50;
      totalTemps2 = millis();
      delayCommande=900;
      while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(100);
      {
        totalTemps = millis();
        leftMotor.setU(uL);
        rightMotor.setU(uR);
      }

    // cercle a
    uL=65; 
    uR=200;
    totalTemps2 = millis();
    delayCommande=4000;  
    while ((delayCommande+totalTemps2)>(totalTemps))//remplace delay(1250);   
      {
       totalTemps = millis();
       leftMotor.setU(uL);
       rightMotor.setU(uR);
    }

     leftMotor.stop();
    rightMotor.stop();
    
}
