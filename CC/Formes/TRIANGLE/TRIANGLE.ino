/*
    TRIANGLE.ino
    Sur base de CC.ino (v5)

*/
 
#include <PID_v2.h>
#include <FastPID.h>
#include <motor.h>
#include <Encoder.h>
#define R 3.5
#define L 15.0
#define N 1440.0
#define DEB0 1
#define DEB1 0
#define SIZEMAX 3

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
double xd,yd;
int mark = 0;
int tag = 1;

// PID 

//Define Variables we'll be connecting to
double slw, ilw, olw;
double srw, irw, orw;
//double setpoint, input, output;

//Specify the links and initial tuning parameters
PID rwPID(&irw, &orw, &srw,5,0,0,DIRECT);
PID lwPID(&ilw, &olw, &slw,5,0,0,DIRECT); 
//PID rotPID(&input, &output, &setpoint,40,0,0,DIRECT);
float Kp=0.65, Ki=0, Kd=0, Hz=10;
int output_bits = 16;
bool output_signed = true;


FastPID myPID(Kp, Ki, Kd, Hz, output_bits, output_signed);


// MOTOR : left and right motors with corresponding pins

Motor leftMotor(leftMotorPwmPin, leftMotorDirPin);
Motor rightMotor(rightMotorPwmPin, rightMotorDirPin);
void leftWspeed(double actualSpeed, double desiredSpeed);
void rightWspeed(double actualSpeed, double desiredSpeed);


// ENCODERS 

Encoder leftEnc(19,18);
Encoder rightEnc(20,21);
void rotate(double phi,double desiredA);
void forward(double x, double y, double desiredx, double desiredy);

// TIMER 

double temprime;
double t;
double temps;
double delta;
double aS;

// ARRAY OF POINTS

int arrayX[] = {10,-10,0};
int arrayY[] = {10,10,0};
int index = 1;
int cpt = 0;
int i = 0;

void setup() {
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
  // put your setup code here, to run once:
  Serial.begin(9600);
  xd = 10;
  yd = 10;
  desiredA = atan2(yd-y,xd-x);
  Serial.print("desiredA :");
  Serial.println(desiredA);
  Serial.println(x);
  Serial.println(y);
  delay(5000);
  rwPID.SetMode(AUTOMATIC);
  lwPID.SetMode(AUTOMATIC);
  //rotPID.SetMode(AUTOMATIC);
 /* if (myPID.err1()) {
  Serial.println("There is a configuration error!");
  for (;;) {}
  }*/
  temprime = millis();
  Serial.println(temprime);
  t = 0;
}

void loop() {
  deltatickG = leftEnc.read() - deltaprimeG;
  deltaprimeG = leftEnc.read();

  deltatickD = rightEnc.read() - deltaprimeD;
  deltaprimeD = rightEnc.read();
  /*Serial.print("deltatickG: ");
  Serial.println(deltatickG);
  Serial.print("deltatickD: ");
  Serial.println(deltatickD);*/

  temps = millis()-temprime;
  delta=temps-t;
  t=temps;

  DCg = 2*PI*R*(deltatickG/N);
  DCd = 2*PI*R*(deltatickD/N);

  DC = (DCg + DCd)/2;
  x = x + DC * cos(phi);
  y = y + DC * sin(phi);
  phi = phi + (DCd - DCg)/L;

  if(!mark)
    rotate(phi,desiredA);

  if(mark)
    forward(DCd/delta,DCg/delta);

  Serial.print("t : ");
  Serial.println(temps);
  
  Serial.print("atan : ");
  Serial.println(desiredA);

  if(DEB0){
    Serial.print("x : ");
    Serial.println(x);
    Serial.print("y : ");
    Serial.println(y);
    Serial.print("xd : ");
    Serial.println(xd);
    Serial.print("yd : ");
    Serial.println(yd);
    Serial.print("phi : ");
    Serial.println(phi);
  }
  i++;
}

void rightWspeed(double actualSpeed, double desiredSpeed){
  irw = actualSpeed;
  srw = desiredSpeed;
  rwPID.Compute();
  //Serial.print("oR : ");
  //Serial.println(orw);
  rightMotor.setU(orw);
}

void leftWspeed(double actualSpeed, double desiredSpeed){
  ilw = actualSpeed;
  slw = desiredSpeed;
  lwPID.Compute();
  //Serial.print("oL : ");
  //Serial.println(olw);
  leftMotor.setU(olw);
}

void rotate(double phi, double desiredA){
  if (phi < desiredA){
    leftMotor.setU(-35);
    rightMotor.setU(35);
  }

  if (phi > desiredA){
    leftMotor.setU(35);
    rightMotor.setU(-35);
  }

  if (phi <= desiredA + 0.05 && phi >= desiredA -0.05){
    leftMotor.setU(0);
    rightMotor.setU(0);
    delay(1000);
    mark = 1;
    DCd = 0;
    DCg = 0;   
  }
}

void forward(double actualSpeedR, double actualSpeedL){
  rightWspeed(actualSpeedR*1000,15.0);
  leftWspeed(actualSpeedL*1000,15.0);
  //if(abs(x)>abs(xd) || abs(y)>abs(yd){
  // Baisser la valeur ci-dessous ??? 
  delay(100);
  if(x > xd - 3 && x < xd + 3 && y > yd - 3 && y < yd + 3 && tag){
   /* if(cpt == 1){
      x=arrayX[index];
      y=arrayY[index];
      index++;
      xd = arrayX[index];
      yd = arrayY[index]; 
      desiredA = atan2(yd-y,xd-x);
      cpt = 0;
    }
    cpt++;*/
    mark = 0;
    leftMotor.setU(0);
    rightMotor.setU(0);
    delay(1000);
    nextPoint();
  }
}

void nextPoint(){
    xd = arrayX[index];
    yd = arrayY[index];
    desiredA = atan2(yd-y,xd-x);
    delay(1000);
    if(index == SIZEMAX){
      leftMotor.setU(0);
      rightMotor.setU(0);
      delay(10000);
    }
    index++;
}
