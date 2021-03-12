/*--------------------------------------------------------
      Correcteur Proportionnel v2
    Ajout de la variable newTime
    Laisse le temps au correcteur de faire effet 
      avant de recalculer l'output
  ------------------------------------------------------*/


#include <motor.h>
#include <Encoder.h>
//TODO : nb ticks par secondes et non par itération 
//TODO faire un délai entre chaque calcul de PID
Encoder leftEnc(19, 18);
Encoder rightEnc(21, 20);
Motor leftMotor(leftMotorPwmPin, leftMotorDirPin);
Motor rightMotor(rightMotorPwmPin, rightMotorDirPin);
double totalTicksL = 0;
double totalTicksR = 0;
double totalTemps = 0;
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
double leftKp = 0.07;
double rightKp = 0.07;
int i;
int tour;


void setup()
{
  Serial.begin(9600);
  i = 0;
  delay(300);
}


void loop()
{
  if (i == 0)
  {
  //tout droit pendant 3 secondes
  move(200, 200, 3000); 
  stopMotors();
  }
  i = 1;
  
}

void move(double leftSetpoint, double rightSetpoint, double stopTime)
{
  totalTemps = millis();
  totalTicksL = leftEnc.read();
  totalTicksR = rightEnc.read();
  tour = 0;
  double newTime = 0;
  double lastTime = 0;
    
  while(millis() - totalTemps < stopTime)
  {
    leftInput = leftEnc.read() - totalTicksL;
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

    printInfos();
    
    // laisser le setU faire effet avant de recalculer l'output
    lastTime = millis();
    newTime = millis() - lastTime;
    while (newTime < 150)
    {
      newTime = millis() - lastTime;
    }
    i = i + 1;
  }
  stopMotors();
  delay(1000);
}


// empêcher l'élan
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
  i = 1;
}


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
