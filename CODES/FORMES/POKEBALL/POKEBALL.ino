/*-----------------------------------------
        Pokeball, v2
        Richard, Falitiana et Raphaël, 2021
  -----------------------------------------*/

#include <motor.h>
#include <Encoder.h>
// On définit les pins des encodeurs
Encoder leftEnc(19, 18);
Encoder rightEnc(21, 20);
/* Pour les pixels : */
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN       8
#define NUMPIXELS 7
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// Left and right motors with corresponding pins
Motor leftMotor(leftMotorPwmPin, leftMotorDirPin);
Motor rightMotor(rightMotorPwmPin, rightMotorDirPin);
// PID variables
double leftError = 0;
double leftSetpoint = 0;
double leftInput = 0;
double leftOutput = 0;
double rightError = 0;
double rightSetpoint = 0;
double rightInput = 0;
double rightOutput = 0;
double leftPrevOutput = 0;
double rightPrevOutput = 0;
double totalTicksL = 0;
double totalTicksR = 0;
double leftKp = 0.07;
double rightKp = 0.07;
double totalTemps = 0;
// Affichage terminal
int i = 0;
int k = 0;
int tour = 0;



void setup()
{
  Serial.begin(9600);

  // NEOPIXELS
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();
  neopixel(0, 100, 0, 0);
  
  delay(10000);
}


void loop()
{
  if (k == 0)
  {
    // grand cercle
    neopixel(0, 100, 0, 0);
    move(150, 175, 17000);
    neopixel(0, 100, 100, 100);
    move(150,175,17000);
    
    // decalage
    move(-50, 50, 2000);
    neopixel(0, 100, 100, 0);
    
    // premier trait droit
    move(200,200,3350);
    
    // partie invisble
    neopixel(0,0,0,0);
    move(200, 200, 1280);

    // décalage pour petit cercle
    move(50, -50, 2000);
    
    // petit cercle
    neopixel(0, 100, 100, 0);
    move(200,100,8200);
    
    // remettre dans l'axe
    move(-50, 50, 2000);
    
    // deuxième trait droit
    move(200, 200, 3350);
    
    stopMotors();
  }
  k = 1;
}

void neopixel(int ledNum, int R, int G, int B)
{
  pixels.clear();
  pixels.show();
  pixels.setPixelColor(ledNum, pixels.Color(R, G, B));
  pixels.show();
}

void move(double leftSetpoint, double rightSetpoint, double stopTime)
{
  totalTemps = millis();
  totalTicksL = leftEnc.read();
  totalTicksR = rightEnc.read();
  while(millis() - totalTemps < stopTime)
  {
    leftInput = leftEnc.read() - totalTicksL ;
    rightInput = rightEnc.read() - totalTicksR;
    totalTicksL = leftEnc.read();
    totalTicksR = rightEnc.read();
    
    leftError = leftSetpoint - leftInput;
    rightError = rightSetpoint - rightInput;

    leftOutput = leftPrevOutput + leftKp * leftError;
    rightOutput = rightPrevOutput + rightKp * rightError;
    
    if (leftOutput > 250)   leftOutput = 250;
    if (rightOutput > 250)  rightOutput = 250;
    if (leftOutput < -250)     leftOutput = -250;
    if (rightOutput < -250)    rightOutput = -250;
    
    leftPrevOutput = leftOutput;
    rightPrevOutput = rightOutput;
    
    leftMotor.setU(leftOutput);
    rightMotor.setU(rightOutput);

    printInfos2();
  }

 // stopMotors();
 // delay(1000);
}



void printInfos2()
{
  Serial.println();
  Serial.println();
  Serial.print("Tour ");
  Serial.println(i);
  Serial.print("leftInput = ");
  Serial.println(leftInput);
  Serial.print("rightInput = ");
  Serial.println(rightInput);
  Serial.println();
  Serial.print("leftError = ");
  Serial.println(leftError);
  Serial.print("rightError = ");
  Serial.println(rightError);
  Serial.println();
  Serial.print("leftOutput = ");
  Serial.println(leftOutput);
  Serial.print("rightOutput = ");
  Serial.println(rightOutput);
  i = i + 1;
}

void stopMotors()
{
  Serial.println();
  Serial.println();
  //Serial.println("Stop motors");
  leftMotor.setU(-30);
  rightMotor.setU(-30);
  delay(100);
  leftMotor.setU(-15);
  rightMotor.setU(-15);
  delay(100);
  leftMotor.setU(-10);
  rightMotor.setU(-10);
  delay(100);
  leftMotor.setU(-5);
  rightMotor.setU(-5);
  delay(100);
  rightMotor.stop();
  leftMotor.stop();
}



/*
void set_U(double leftSetpoint, double rightSetpoint, double stopTime)
{
  totalTemps = millis();
  while(millis() - totalTemps < stopTime)
  {
    leftMotor.setU(leftSetpoint);
    rightMotor.setU(rightSetpoint);
  }
  stopMotors();
  delay(1000);
}*/
