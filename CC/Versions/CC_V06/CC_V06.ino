#include <motor.h>
#include <Encoder.h>
#include <PID_v2.h>

// On construit nos deux PID
double Kp = 0.9, Ki = 0, Kd = 0;
PID_v2 leftPID(Kp, Ki, Kd, PID::Direct);
PID_v2 rightPID(Kp, Ki, Kd, PID::Direct);
Encoder leftEnc(19, 18);
Encoder rightEnc(21, 20);
Motor leftMotor(leftMotorPwmPin, leftMotorDirPin);
Motor rightMotor(rightMotorPwmPin, rightMotorDirPin);
double deltaTickL = 0;
double totalTicksL = 0;
double deltaTickR = 0;
double totalTicksR = 0;
double inputLeft = 0;
double inputRight = 0;
double outputLeft = 0;
double outputRight = 0;
double nb_ticks_stopL = 0;
double nb_ticks_stopR = 0;

int i = 1;
int tour = 0;

void setup()
{
  Serial.begin(9600);
  leftPID.SetOutputLimits(-200, 200);
  rightPID.SetOutputLimits(-200, 200);
}

void loop()
{
  leftPID.Start(0, 0, 95);
  rightPID.Start(0, 0, 95);
  moveRobot(100, 100);
}


void moveRobot(double distance_cm_L, double distance_cm_R)
{
  nb_ticks_stopL = distance_cm_L * 65.45;
  nb_ticks_stopR = distance_cm_R * 65.45;

  while (i == 0)
  {
    deltaTickL = leftEnc.read() - totalTicksL;
    deltaTickR = rightEnc.read() - totalTicksR;
    totalTicksL = leftEnc.read();
    totalTicksR = rightEnc.read();
    leftMotor.setU(leftPID.Run(deltaTickL));
    rightMotor.setU(rightPID.Run(deltaTickR));
   
    if (totalTicksL > nb_ticks_stopL || totalTicksR > nb_ticks_stopR)
    { stopMotors(); }
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
  // Pour (if == 1) comme condition d'arrêt
  i = 1;
}
