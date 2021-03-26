#include <FastPID.h>
#include <motor.h>
#include <Encoder.h>

/* Pour les neopixels */
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN       8
#define NUMPIXELS 7
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


// left and right motors with corresponding pins
Motor leftMotor(leftMotorPwmPin, leftMotorDirPin);
Motor rightMotor(rightMotorPwmPin, rightMotorDirPin);

// left and right encoders with corresponding pins
Encoder leftEnc(19, 18);
Encoder rightEnc(20, 21);

// Configuration PID
float Kp = 0.9, Ki = 0, Kd = 0, Hz = 1;
int output_bits = 16;
bool output_signed = false;
FastPID myPID(Kp, Ki, Kd, Hz, output_bits, output_signed);

// Rayon de la roue
#define R 3.5
// Distance entre les deux roues
#define L 15.0
// Nombre de ticks pour un tour
#define N 1440.0

/* Variables nécessaires pour calcul du déplacement */
// Diff entre nombre de ticks attendus et réels
int deltatickG = 0;
int deltaprimeG = 0;
int deltatickD = 0;
int deltaprimeD = 0;

// Distance parcourue par roue
double DCg = 0;
double DCd = 0;

// Distance parcourue
double DC = 0;

// Angle entre position actuelle
// et position souhaité
double phi = 0;

// Coordonnées initiales
double x = 0;
double y = 0;

// Coordonnées désirées
double xd = 10;
double yd = 10;

// Angle désiré
double desiredA, desA;
//
int input = 0;
// int mark = 0;

// Fonction de contrôle du mouvement du robot
void rotate(double phi, double desiredA);
void forward(double x, double y, double desiredx, double desiredy);
// void curve(...)
// left_motor -> rwspeed


void setup()
{
  Serial.begin(9600);

  // Calcul l'angle entre xd et x :
  desiredA = atan2(yd - y, xd - x);
  Serial.println("setup desiredA :");
  Serial.println(desiredA);
  Serial.println(desiredA);
  Serial.println(desiredA);
  
  

  // affInfos();
  // delay(1000);

  if (myPID.err())
  {
    Serial.println("There is a configuration error!");
    for (;;) {}
  }
  myPID.setOutputRange(-255, 255);
}


void loop()
{
  // Nbre de ticks fait entre deux itérations
  deltatickG = leftEnc.read() - deltaprimeG;
  deltaprimeG = leftEnc.read();
  deltatickD = rightEnc.read() - deltaprimeD;
  deltaprimeD = rightEnc.read();

  // print infos

  //Distance parcourue des roues :
  DCg = 2 * PI * R * (deltatickG / N);
  DCd = 2 * PI * R * (deltatickD / N);

  // Distance parcourue du robot
  DC = (DCg + DCd) / 2;

  // Mise à jour des coordonnées
  x = x + DC * cos(phi);
  y = y + DC * sin(phi);
  // Calcul de l'angle
  phi = phi + (DCd - DCg) / L;
  desA = desiredA * 100;
  input = phi * 100;

  rotate(input, desA);

  // forward(x, y, 10, 10);
}

void rotate(int input, double desA)
{
  uint8_t output = myPID.step(desA, input);
  rightMotor.setU(-output);
  leftMotor.setU(output);
}

void forward(double x, double y, double desiredx, double desiredy)
{
  leftMotor.setU(40);
  rightMotor.setU(40);
  arret(x, y, desiredx, desiredy);
}


void arret(double x, double y, double desiredx, double desiredy)
{
  if (x > desiredx - 2 && x < desiredx + 2 && y > desiredy - 2 && y < desiredy + 2)
  {
    leftMotor.setU(0);
    rightMotor.setU(0);
    delay(1000);
  }
}

void affInfos()
{
  Serial.println(input);
  Serial.println(desA);

  Serial.print("input :");
  Serial.println(input);

  Serial.print("x : ");
  Serial.println(x);
  Serial.print("y : ");
  Serial.println(y);
  Serial.print("phi : ");
  Serial.println(phi);

  Serial.print("desiredA :");
  Serial.println(desiredA);
  Serial.println(x);
  Serial.println(y);
}
