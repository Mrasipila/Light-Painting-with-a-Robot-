/*-----------------------------------------
          Spirale v1
        Boucle ouverte
-----------------------------------------*/
#include <motor.h>
// left and right motors with corresponding pins
Motor leftMotor(leftMotorPwmPin, leftMotorDirPin);
Motor rightMotor(rightMotorPwmPin, rightMotorDirPin);
// global variables
int uL = 0;
int uR = 0;
int i=0;


void setup() 
{
  Serial.begin(9600); 
}



void loop() 
{
  delay(2000);
     
  for (i=0;i<20;i++)
  {
     //tout droit long
     uL = 50;
     uR = i*2;
     leftMotor.setU(uL);
     rightMotor.setU(uR);
     delay(200);
  }
  
  for (i=20;i>0;i--)
  {
     //tout droit long
     uL = i*2;
     uR = 50;
     leftMotor.setU(uL);
     rightMotor.setU(uR);
     delay(200);
  }
   
  leftMotor.stop();
  rightMotor.stop();
}
