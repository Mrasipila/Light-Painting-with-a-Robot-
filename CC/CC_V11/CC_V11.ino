/*--------------------------------------------------------
      Correcteur Proportionnel Intégral dérivé v2
      
    Implémentation de la fonction initialize qui tune le coefficient en fonction de la consigne 
    La consigne est comprise entre 0 et 120 en valeur absolue
    
    Sources :
    http://www.ferdinandpiette.com/blog/2011/08/implementer-un-pid-sans-faire-de-calculs/
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
#define DEB 0
#define TIME_IN_LOOP 28
// Gains du PID
double leftKp           = 0;
double leftKi           = 0;
double leftKd           = 0;
double rightKp          = 0;
double rightKi          = 0;
double rightKd          = 0;


void loop()
{
  if (k == 0)
  {
    neopixel(0, 100, 100, 0);
    move(30, 30,  2000);
    stopMotors();
    
    neopixel(0, 0, 0, 0);
    move(30, -30, 620);
    stopMotors();
    neopixel(0, 100, 100, 0);
    move(30, 30,  1000);
    stopMotors();
    neopixel(0, 0, 0, 0);
    move(30, -30, 620);
    stopMotors();
    neopixel(0, 0, 0, 0);
    move(30, 30,  1000);
    stopMotors();
    neopixel(0, 0, 0, 0);
    move(30, -30, 620);
    stopMotors();
    neopixel(0, 100, 100, 0);
    move(30, 30,  1000);
    stopMotors();
    neopixel(0, 0, 0, 0);
    move(-30, 30, 620);
    stopMotors();
    neopixel(0, 0, 0, 0);
    move(30, 30,  1000);
    stopMotors();
    neopixel(0, 0, 0, 0);
    move(-30, 30, 620);
    stopMotors();
    neopixel(0, 100, 100, 0);
    move(30, 30,  1000);
    stopMotors();
    neopixel(0, 0, 0, 0);
    
    k = 1;
  }
}

void move(double _leftSetpoint, double _rightSetpoint, double stopTime)
{
  leftSetpoint = _leftSetpoint;
  rightSetpoint = _rightSetpoint;

  // initialise les valeurs et ajuste les gains en fonction de la consigne
  initialize (_leftSetpoint, _rightSetpoint);
  
  totalTemps = millis();
  
  totalTicksL = leftEnc.read();
  totalTicksR = rightEnc.read();
  
  while (millis() - totalTemps < stopTime)
  {
    leftInput = leftEnc.read() - totalTicksL;
    rightInput = rightEnc.read() - totalTicksR;
    totalTicksL = leftEnc.read();
    totalTicksR = rightEnc.read();

    // Calcul de l'erreur
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

    debug();

    // laisser le setU faire effet avant de recalculer l'output
    lastTime = millis();
    newTime = millis() - lastTime;
    
    while (newTime < TIME_IN_LOOP)
    {
      newTime = millis() - lastTime;
    }
  }
}


void initialize (double _leftSetpoint, double _rightSetpoint)
{
  // MET A ZERO LES VARIABLES DE LA CONSIGNE PRECENDENTE
  leftErrorSum = 0;
  rightErrorSum = 0;
  leftPrevError = 0;
  rightPrevError = 0;
  // inutile de remettre à zéro puisque calculé fonction d'autres variables...
  leftErrorDeriv = 0;
  rightErrorDeriv = 0;
  // à vérifier puis supprimer

  // AJUSTE LES COEFF FONCTION DE LA CONSIGNE
  
  // Sp = 30 Kp = 0.4 Ki = 0 Kd = 1.5 : intervalle entre 0 et 45
  if (abs(_leftSetpoint) <= 45)
  {
    leftKp  = 0.4;
    leftKi  = 0.0;
    leftKd  = 1.5;
  }
  if (abs(_rightSetpoint) <= 45)
  {
    rightKp = 0.4;
    rightKi = 0.0;
    rightKd = 1.5;
  }
  // Sp = 60  Kp = 0.06 Ki = 0 Kd = 0.3 : intervalle entre 45 et 75
  if (abs(_leftSetpoint) > 45 || abs(_leftSetpoint) <= 75 )
  {
    leftKp  = 0.06;
    leftKi  = 0.0;
    leftKd  = 0.3;
  }
  if (abs(_rightSetpoint) > 45 || abs(_rightSetpoint) <= 75)
  {
    rightKp = 0.06;
    rightKi = 0.0;
    rightKd = 0.3;
  }
  // Sp = 90  Kp = 0.8  Ki = 0 Kd = 2.5 : intervalle entre 75 et 105
  if (abs(_leftSetpoint) > 75 || abs(_leftSetpoint) <= 105 )
  {
    leftKp  = 0.8;
    leftKi  = 0.0;
    leftKd  = 2.5;    
  }
  if (abs(_rightSetpoint) > 75 || abs(_rightSetpoint) <= 75)
  {
    rightKp = 0.8;
    rightKi = 0.0;
    rightKd = 2.5;
  }
  // Sp = 120 Kp = 0.6  Ki = 0 Kd = 0 : intervalle entre 105 et l'infini
  // l'output est bornée plus haut dans la fonction move()
  if (abs(_leftSetpoint) > 75)
  {
    leftKp  = 0.6;
    leftKi  = 0.0;
    leftKd  = 0.0;
  }
  if (abs(_rightSetpoint) > 75)
  {
    rightKp = 0.6;
    rightKi = 0.0;
    rightKd = 0.0;
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
  move(0,0,1000);
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


void debug()
{
  if (DEB == 1)
  Serial.println(leftInput);
  if (DEB == 2)
  Serial.println(rightInput);
  if (DEB == 3)
  Serial.println(leftError);
  if (DEB == 4)
  Serial.println(rightError);
  if (DEB == 5)
  Serial.println(leftOutput);
  if (DEB == 6)
  Serial.println(rightOutput);
}



void setup()
{
  Serial.begin(115200);
  pixels.begin();
  neopixel(0, 0, 0, 0);
  wait(2000);
}
