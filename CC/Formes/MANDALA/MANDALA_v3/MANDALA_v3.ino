/*--------------------------------------------------------
     MANDALA.ino (v3)
     
     Sur base de CC.ino (v8)
  ------------------------------------------------------*/

#include <motor.h>
#include <Encoder.h>
// On définit les pins des encodeurs
Encoder leftEnc(19, 18);
Encoder rightEnc(21, 20);
// On met en place les neopixels
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN       8
#define NUMPIXELS 7
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
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
// Affichage terminal du tour
int i = 0;
// condition d'arrêt pour k = 1
int k = 0;

void setup()
{
  Serial.begin(9600);
  pixels.begin();
  neopixel(0,100,0,0);
  delay(10000);
}


void loop()
{
  if (k == 0)
  {
    //spirale
    neopixel(0,100,0,0);
    
    for (int j = 0; j < 20; j++)
    {
      move(100-j*5, 100, 1000);
    }
    
    // grand cercle
    neopixel(0,0,100,0);
    move(150, 175, 18000);
    move(150,175,18000);
    
    //decalage
    move(-50,50,500);
    
    //spirale fin
    neopixel(0,0,0,100);

    for (int j = 0; j < 20; j++)
    {
      move(100-j*5,100,1000);
    }
    
    // On stop les moteurs  
    stopMotors();
  }
  // k passe à 1 pour ne plus appeller les move()
  k = 1;
}

void move(double leftSetpoint, double rightSetpoint, double stopTime)
{
  totalTemps = millis();
  totalTicksL = leftEnc.read();
  totalTicksR = rightEnc.read();
  double newTime = 0;
  double lastTime = 0;
  
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

    // printInfos();
    // i = i + 1;
   
    // laisser le setU faire effet avant de recalculer l'output
    lastTime = millis();
    newTime = millis() - lastTime;
    while (newTime < 150)
    {
      newTime = millis() - lastTime;
    } 
  }
}

void stopMotors()
{
  Serial.println();
  Serial.println();
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


void printInfos()
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
}





void neopixel(int ledNum, int R, int G, int B)
{
  pixels.clear();
  pixels.show();
  pixels.setPixelColor(ledNum, pixels.Color(R, G, B));
  pixels.show();
}
