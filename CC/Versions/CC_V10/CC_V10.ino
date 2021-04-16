/*--------------------------------------------------------
      Correcteur Proportionnel Intégral dérivé v1
    Suite du correcteur proportionnel v3
    Ajout des formules des correcteurs intégral et dérivé
    Source :
    http://www.ferdinandpiette.com/blog/2012/04/asservissement-en-vitesse-dun-moteur-avec-arduino/
  ------------------------------------------------------*/
// On définit les pins des encodeurs
#include <Encoder.h>
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
#include <motor.h>
Motor leftMotor(leftMotorPwmPin, leftMotorDirPin);
Motor rightMotor(rightMotorPwmPin, rightMotorDirPin);
double newTime = 0;
double lastTime = 0;
double now = 0;
double go = 0; 
// Affichage terminal du tour
int i = 0;
// condition d'arrêt pour k = 1
int k = 0;
// CALCUL DU PID
double totalTemps       = 0;
double totalTicksL      = 0;
double totalTicksR      = 0;
// SETPOINT
double leftSetpoint     = 0;
double rightSetpoint    = 0;
// INPUT
double leftInput        = 0;
double rightInput       = 0;
// ERREUR
double leftError        = 0;
double rightError       = 0;
// OUTPUT
double leftOutput       = 0;
double rightOutput      = 0;
// PROPORTIONEL
double leftProportional = 0;
double rightProportional = 0;
// INTEGRAL
double leftIntegral     = 0;
double rightIntegral    = 0;
double leftErrorSum     = 0;
double rightErrorSum    = 0;
// DERIVE
double leftDerivate     = 0;
double rightDerivate    = 0;
double leftPrevError    = 0;
double rightPrevError   = 0;
double leftErrorDeriv   = 0;
double rightErrorDeriv  = 0;
// COEFF
double leftKp           = 0.0999;
double rightKp          = 0.0999;
double leftKi           = 0.002;
double rightKi          = 0.002;
double leftKd           = 0.002;
double rightKd          = 0.002;


void setup()
{
  Serial.begin(115200);
  pixels.begin();
  neopixel(0,100,0,0);
  wait(10000);
}


void loop()
{
  if (k == 0)
  {
    // Mettre ici les appels successifs de move() et neopixel()

    
    // 1 ROUGE
    neopixel(0,100,0,0);
    move(150, 75, 11500);
    stopMotors();
    
    // 2 VERT
    move(-50, 50, 3225);
    stopMotors();
    neopixel(0,0,100,0);
    move(150, 75, 11500);
    stopMotors();
    
    // 3 BLEU
    move(-50, 50, 3225);
    stopMotors();
    neopixel(0,0,0,100);
    move(150, 75, 11500);
    stopMotors();

    // TOUT DROIT
    neopixel(0,0,0,0);
    move(50, 50, 10000);
    stopMotors();
    
    // 4 JAUNE
    move(-50, 50, 3225);
    stopMotors();
    neopixel(0,100,100,0);
    move(150, 75, 11500);
    stopMotors();
    
    // 5 MAGENTA
    move(-50, 50, 3225);
    stopMotors();
    neopixel(0,100,0,100);
    move(150, 75, 11500);
    
    // 6 CYAN
    move(-50, 50, 3225);
    stopMotors();
    neopixel(0,0,100,100);
    move(150, 75, 11500);

    // 7
    move(-50, 50, 3225);
    stopMotors();
    neopixel(0,100,50,50);
    move(150, 75, 11500);
    stopMotors();
    
    // 8
    move(-50, 50, 3225);
    stopMotors();
    neopixel(0,50,100,50);
    move(150, 75, 11500);
    
    // 9
    move(-50, 50, 3225);
    stopMotors();
    neopixel(0,50,50,100);
    move(150, 75, 11500);
    
    // On stop les moteurs  
    stopMotors();
  }
  // k passe à 1 pour ne plus appeller les move()
  k = 1;
  
}

void move(double _leftSetpoint, double _rightSetpoint, double stopTime)
{
  leftSetpoint = _leftSetpoint;
  rightSetpoint = _rightSetpoint;
  totalTemps = millis();
  totalTicksL = leftEnc.read();
  totalTicksR = rightEnc.read();
  newTime = 0;
  lastTime = 0;
    
  while(millis() - totalTemps < stopTime)
  {
    leftInput = leftEnc.read() - totalTicksL;
    rightInput = rightEnc.read() - totalTicksR;
    totalTicksL = leftEnc.read();
    totalTicksR = rightEnc.read();
    
    // Calcul de l'ereur
    leftError = leftSetpoint - leftInput;
    rightError = rightSetpoint - rightInput;
    
    // Correcteur Proportionnel
    leftProportional = leftKp * leftError;
    rightProportional = rightKp * rightError;
    
    // Correcteur Intégral
    leftErrorSum += leftError;
    rightErrorSum += rightError;
    leftIntegral = leftKi * leftErrorSum;
    rightIntegral = rightKi * rightErrorSum;
    
    // Correcteur Dérivé
    leftErrorDeriv = leftError - leftPrevError;
    rightErrorDeriv = rightError - rightPrevError;
    leftPrevError = leftError;
    rightPrevError = rightError;
    leftDerivate = leftKd * leftErrorDeriv;
    rightDerivate = rightKd * rightErrorDeriv;
  
    leftOutput += leftProportional + leftIntegral + leftDerivate;
    rightOutput += rightProportional + rightIntegral + rightDerivate;
    
    if (leftOutput > 250)   leftOutput = 250;
    if (rightOutput > 250)  rightOutput = 250;
    if (leftOutput < -250)  leftOutput = -250;
    if (rightOutput < -250) rightOutput = -250;  

    leftMotor.setU(leftOutput);
    rightMotor.setU(rightOutput);
    
    printInfos();
  
    // laisser le setU faire effet avant de recalculer l'output
    lastTime = millis();
    newTime = millis() - lastTime;
    while (newTime < 150)
    {
     newTime = millis() - lastTime;
    }
  }
}



void wait(double waitTime)
{
    now = millis();
    go = millis() - now;
    while (go < waitTime)
    {
      go = millis() - now;
    }
}

void stopMotors()
{
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

void neopixel(int ledNum, int R, int G, int B)
{
  pixels.clear();
  pixels.show();
  pixels.setPixelColor(ledNum, pixels.Color(R, G, B));
  pixels.show();
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
  i = i + 1;
}
