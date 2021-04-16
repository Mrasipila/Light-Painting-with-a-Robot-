
#include <PID_v1.h>
#include <motor.h>
#include <math.h>
#define R 3.5
#define L 15.0
#define N 721.0
#define DEF 0

// left and right motors with corresponding pins
Motor leftMotor(leftMotorPwmPin, leftMotorDirPin);
Motor rightMotor(rightMotorPwmPin, rightMotorDirPin);

// global functions
double Setpointg, Inputg, Outputg;
PID PIDg(&Inputg, &Outputg, &Setpointg, 2, 5, 1, P_ON_M, DIRECT);

double Setpointd, Inputd, Outputd;
PID PIDd(&Inputd, &Outputd, &Setpointd, 2, 5, 1, P_ON_M, DIRECT);

// global variables
int uL = 0;
int uR = 0;

int leftInc = 10;
int rightInc = 10;

int deltatickG = 0;
int deltaprimeG = 0;
int deltatickD = 0;
int deltaprimeD = 0;

double DCg = 0;
double DCd = 0;

int sum = 0;
int resol = 200;

double quantum = 15.32 / resol;
double quantum1 = 15.32 / 400;

long temprime = 0;
double vitesseActuel = 0;

// Position
double phi = 0;
double x = 0;
double y = 0;
double DC = 0;
double desiredA = 0;
double xd = 0;
double yd = 0;
double gapPh = 0.1;
double gapPo = 2;

// Scaling
double facteur = 0;
double Out1 = 0;
double Out2 = 0;
int sign1 = 0;
int sign2 = 0;

volatile long int leftCount = 0;
volatile long int rightCount = 0;

void encoderD() {
  if (digitalRead(RH_ENCODER_A) == HIGH) {
    if (digitalRead(RH_ENCODER_B) == LOW) {
      rightCount--;
    } else {
      rightCount++;
    }
  } else {
    if (digitalRead(RH_ENCODER_B) == LOW) {
      rightCount++;
    } else {
      rightCount--;
    }
  }
  Serial.print("lc :");
  Serial.println(rightCount);
}

void encoderG() {
  if (digitalRead(LH_ENCODER_A) == HIGH) {
    if (digitalRead(LH_ENCODER_B) == LOW) {
      leftCount--;
    } else {
      leftCount++;
    }
  } else {
    if (digitalRead(LH_ENCODER_B) == LOW) {
      leftCount++;
    } else {
      leftCount--;
    }
  }
  Serial.print("rc :");
  Serial.println(leftCount);
}

// ************** SETUP *********************
void setup() {

  pinMode(LH_ENCODER_A, INPUT_PULLUP);
  pinMode(LH_ENCODER_B, INPUT_PULLUP);
  pinMode(RH_ENCODER_A, INPUT_PULLUP);
  pinMode(RH_ENCODER_B, INPUT_PULLUP);
  deltatickG = leftCount;
  deltatickD = rightCount;
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(RH_ENCODER_A), encoderD, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LH_ENCODER_A), encoderG, CHANGE);

  //initialize the variables we're linked to
  Inputg = 0;
  Setpointg = 0;
  Inputd = 0;
  Setpointd = 0;

  //turn the PID on
  PIDg.SetMode(AUTOMATIC);
  PIDd.SetMode(AUTOMATIC);

  //Point désiré
  xd = 10;
  yd = 10;
}
// *******************************************


// *************** LOOP **********************
void loop() {

  // Encodeurs
  deltatickG = leftCount - deltaprimeG;
  deltaprimeG = leftCount;
  deltatickD = rightCount - deltaprimeD;
  deltaprimeD = rightCount;
  DCg = 2 * PI * R * (deltatickG / N);
  DCd = 2 * PI * R * (deltatickD / N);
  if (DEF) {
    Serial.print("deltaL :");
//    Serial.println(deltatickL);
    Serial.print("deltaD :");
    Serial.println(deltatickD);
    Serial.print("lc :");
    Serial.println(leftCount);
    Serial.print("rc :");
    Serial.println(rightCount);
  }
  DC = (DCg + DCd) / 2;
  x = x + DC * cos(phi);
  y = y + DC * sin(phi);
  phi = phi + (DCd - DCg) / L;
  Serial.print("x : ");
  Serial.println(x);
  Serial.print("y : ");
  Serial.println(y);

  desiredA = atan(yd - y / xd - x);
  Serial.print("DePhi :");
  Serial.println(desiredA);
  Serial.print("PHI :");
  Serial.println(phi);
  if (phi > desiredA - gapPh && phi < desiredA + gapPh) {
    Setpointd = 4;
    Setpointg = 4;
    //delay(9000);
    if (x > xd - gapPo && x < xd + gapPo && y > yd - gapPo && y < yd + gapPo) {
      Setpointd = 0;
      Setpointg = 0;
      Serial.print("AAAAAAAAAAAAAAAAAAA");

      leftMotor.setU(0);
      rightMotor.setU(0);
      delay(10000);
      leftMotor.setU(0);
      rightMotor.setU(0);
      delay(10000);
    }
  } else {
    Setpointd = 4;
    Setpointg = -4;
  }

  // Moteur Gauche & Droit
  Inputg = DCg * (millis() - temprime); // en cm/millisecondes
  Inputd = DCd * (millis() - temprime); // en cm/millisecondes
  PIDd.Compute();
  PIDg.Compute();

  Serial.print("Od :");
  Serial.println(Outputd);

  Serial.print("Og :");
  Serial.println(Outputg);

  // Rescaling des valeurs passées au moteur
  if (Outputg >= 0) {
    sign1 = 0;
  } else {
    sign1 = 1;
  }

  if (Outputd >= 0) {
    sign2 = 0;
  } else {
    sign2 = 1;
  }

  if (Outputg > Outputd) {
    if (Outputd == 0) {
      Outputd = 1;
    }
    facteur = abs(Outputg) / abs(Outputd);
    if ( sign1 != sign2 ) {
      Out2 = ((100 / facteur) * 4);
      Out1 = Out2 * facteur;
      Out1 -= 200; // Outputg
      Out2 -= 200; // Outputd
    } else if (sign1 == 0 && sign2 == 0) {
      Out2 = ((100 / facteur) * 2);
      Out1 = Out2 * facteur;
    } else {
      Out2 = ((100 / facteur) * 2) * (-1);
      Out1 = Out2 * facteur * (-1);
    }
  } else {
    if (Outputg == 0) {
      Outputg = 1;
    }
    facteur = abs(Outputd) / abs(Outputg);
    if ( sign1 != sign2 ) {
      Out1 = ((100 / facteur) * 4);
      Out2 = Out2 * facteur;
      Out1 -= 200; // Outputg
      Out2 -= 200; // Outputd
    } else if (sign1 == 0 && sign2 == 0) {
      Out1 = ((100 / facteur) * 2);
      Out2 = Out2 * facteur;
    } else {
      Out1 = ((100 / facteur) * 2) * (-1);
      Out2 = Out2 * facteur * (-1);
    }
  }

  leftMotor.setU(Out1);
  Serial.print("uL :");
  Serial.println(Out1);
  rightMotor.setU(Out2);
  Serial.print("uD :");
  Serial.println(Out2);


  // Timer
  temprime = millis();
  Serial.print("t : ");
  Serial.println(millis() - temprime);

}
