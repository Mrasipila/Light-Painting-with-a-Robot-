#include <PID_v2.h>
#include <motor.h>
#include <Encoder.h>
#define R 3.5
#define L 15.0
#define N 1440.0
#define DEB0 1
#define DEB1 0

int deltatickG = 0;
int deltaprimeG = 0;
int deltatickD = 0;
int deltaprimeD = 0;
double DCg = 0;
double DCd = 0;
double DC = 0;
double phi = 0;
double x = 0;
double y = 0;
double desiredA;
int mark = 0;

// PID

//Define Variables we'll be connecting to
// slw : setpoint left wheel
// ilw : input left wheel
// olw : output left wheel
double slw, ilw, olw;
double srw, irw, orw;

//Specify the links and initial tuning parameters
PID rwPID(&irw, &orw, &srw, 40, 0, 0, DIRECT);
PID lwPID(&ilw, &olw, &slw, 40, 0, 0, DIRECT);


// MOTOR : left and right motors with corresponding pins
Motor leftMotor(leftMotorPwmPin, leftMotorDirPin);
Motor rightMotor(rightMotorPwmPin, rightMotorDirPin);


// ENCODERS
Encoder leftEnc(19, 18);
Encoder rightEnc(20, 21);

// TIMER
double temprime;
double t;
double temps;
double setpoint;
double delta;


void setup() 
{
  //   put your setup code here, to run once:
  Serial.begin(9600);
  double xd = 10;
  double yd = 10;
  setpoint = atan2(yd - y, xd - x);
  
  rwPID.SetMode(AUTOMATIC);
  lwPID.SetMode(AUTOMATIC);
  temprime = millis();
  t = 0;

  // 
}

void loop() 
{
  // Nombre de ticks entre deux itérations
  deltatickG = leftEnc.read() - deltaprimeG;
  deltaprimeG = leftEnc.read();
  deltatickD = rightEnc.read() - deltaprimeD;
  deltaprimeD = rightEnc.read(); 

  Serial.print("deltatickG: ");
  Serial.println(deltatickG);
  Serial.print("deltatickD: ");
  Serial.println(deltatickD);

  // Temps écoulé entre deux itérations
  // temprime : temps du setup
  temps = millis() - temprime;
  delta = temps - t;
  t = temps;

  DCg = 2 * PI * R * (deltatickG / N);
  DCd = 2 * PI * R * (deltatickD / N);

  DC = (DCg + DCd) / 2;
  x = x + DC * cos(phi);
  y = y + DC * sin(phi);
  phi = phi + (DCd - DCg) / L;

  rightWspeed(DCd / delta, 30);
  leftWspeed(DCd / delta, 30);

  if (DEB0) 
  {
    Serial.print("x : ");
    Serial.println(x);
    Serial.print("y : ");
    Serial.println(y);
    Serial.print("phi : ");
    Serial.println(phi);
  }
}

void rightWspeed(double actualSpeed, double desiredSpeed)
{
  irw = actualSpeed;
  srw = desiredSpeed;
  rwPID.Compute();
  Serial.println(orw);
  rightMotor.setU(orw);
}

void leftWspeed(double actualSpeed, double desiredSpeed) {
  ilw = actualSpeed;
  slw = desiredSpeed;
  lwPID.Compute();
  Serial.println(olw);
  leftMotor.setU(olw);
}
