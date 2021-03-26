// This Arduino example demonstrates bidirectional operation of a
// 28BYJ-48, using a VMA401 -ULN2003 interface board to drive the stepper.
// The 28BYJ-48 motor is a 4-phase, 8-beat motor, geared down by
// a factor of 68. One bipolar winding is on motor pins 1 & 3 and
// the other on motor pins 2 & 4. The step angle is 5.625/64 and the
// operating Frequency is 100pps. Current draw is 92mA.

#define R 3.5
#define L 15.0

////////////////////////////////////////////////

//declare variables for the FIRST motor pins 
int gmotorPin1 = 11;    // Blue   -28BYJ48 pin 1
int gmotorPin2 = 10;    // Pink   -28BYJ48 pin 2
int gmotorPin3 = 9;    // Yellow -28BYJ48 pin 3
int gmotorPin4 = 8;    // Orange -28BYJ48 pin 4

//declare variables for the SECOND motor pins 
int dmotorPin1 = 2;    // Blue   -28BYJ48 pin 1
int dmotorPin2 = 3;    // Pink   -28BYJ48 pin 2
int dmotorPin3 = 4;    // Yellow -28BYJ48 pin 3
int dmotorPin4 = 5;    // Orange -28BYJ48 pin 4

 

// Red    -28BYJ48 pin 5 (VCC)
int motorSpeed = 1200; 

//variable to set stepper speed
int count = 0;         

// count of steps made
int countsperrev= 512;

// number of steps per full revolution
int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};

//////////////////////////////////////////////////////////////////////////////
// POSITIONS 
 
double DCg = 0;
double DCd = 0;
double DC = 0;
double x = 0;
double y = 0;
double phi = 0;
double desiredA = 0;
double stepsize = ((2*PI*R)/countsperrev)-0.003;

//////////////////////////////////////////////////////////////////////////////
// COORDINATES

// Veuillez insérer vos données
double Cx[] = {};
double Cy[] = {};

// Veuillez insérer la taille de votre ensemble
int dataSize = 8;
int index = 0;

void setup() {
  pinMode(21, OUTPUT);
  //declare the motor pins as outputs
  pinMode(gmotorPin1, OUTPUT);
  pinMode(gmotorPin2, OUTPUT);
  pinMode(gmotorPin3, OUTPUT);
  pinMode(gmotorPin4, OUTPUT);

  pinMode(dmotorPin1, OUTPUT);
  pinMode(dmotorPin2, OUTPUT);
  pinMode(dmotorPin3, OUTPUT);
  pinMode(dmotorPin4, OUTPUT);
  Serial.begin(9600);
  desiredA = atan2(Cy[index],Cx[index]);
  Serial.println(desiredA);
  Serial.println(stepsize);
  delay(5000);
}


//////////////////////////////////////////////////////////////////////////////
// STATES 
int Sclockwiseg = 0;
int Sclockwised = 0;
int Santiclockwiseg = 0;
int Santiclockwised = 0;
int onRun = 1;

int rotation = 1;
int translation = 0;

void loop(){
  // On fait tourner la machine
  onRun = 1;
  Update();

  // On calcul la position de la machine
  DC = (DCg + DCd)/2;
  
  phi = phi + (DCd-DCg)/L;
  x = x + DC * cos(phi);
  y = y + DC * sin(phi);

  // Gestion des états de rotation ou de translation
  if(rotation){
    Serial.print("phi: ");
    Serial.println(phi);
    Serial.print("x: ");
    Serial.println(x);
    Serial.print("y: ");
    Serial.println(y);
    rotate();
  }
  Serial.print("A");
  if(translation){
    Serial.print("phi: ");
    Serial.println(phi);
    Serial.print("x: ");
    Serial.println(x);
    Serial.print("y: ");
    Serial.println(y);
    translate(x,y);
  }
  /*if(count < countsperrev )
    clockwise();
  else if (count == countsperrev * 2)
    count = 0;
  else
    anticlockwise();
  count++;*/

}

/////////////////////////////////////////////////////////////////////////////////
// MOVEMENT CONTROL

void rotate(){
  if(phi < desiredA){
    ChangeState(1,1,0,0);
   /* Sclockwiseg = 1;
    Santiclockwised = 0;
    Sclockwised = 1;
    Santiclockwiseg = 0;*/
    clockwised();
    clockwiseg();
    Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    /*Serial.println("Rotate");
    Serial.println(Sclockwiseg);
    Serial.println(Santiclockwised);
    Serial.println(Sclockwised);
    Serial.println(Santiclockwiseg);*/
  }
  
  if(phi > desiredA){
    ChangeState(0,0,1,1);
    Serial.println("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
    /*Sclockwiseg = 0;
    Santiclockwised = 1;
    Sclockwised = 0;
    Santiclockwiseg = 1;*/
    anticlockwised();
    anticlockwiseg();
    /*Serial.println("Rotate");
    Serial.println(Sclockwiseg);
    Serial.println(Santiclockwised);
    Serial.println(Sclockwised);
    Serial.println(Santiclockwiseg);*/
  }
  
  if(phi >= desiredA-0.01 && phi <= desiredA + 0.01){
    onRun = 0;
    rotation = 0;
    translation = 1;
    Update();
    Serial.print("A");
    delay(1000);
  }
}

// Gere la translation et l'arrêt du robot à la position
void translate(double x, double y){
  if(x > Cx[index] - 0.9 && x < Cx[index] + 0.9 && y > Cy[index] - 0.9 && y < Cy[index] + 0.9){
    // Correction de l'erreur de déplacement 68 équivaut à environ ce qu'on perd, cad 0.9 cm
    for(int i = 0 ; i < 68 ; i++){
      ChangeState(0,1,1,0);
      clockwised();
      anticlockwiseg();
    }
    // On allume la LED
    digitalWrite(21,HIGH);
    // On change l'état du système (Passage à l'état translation)
    onRun = 0;
    rotation = 1;
    translation = 0;
    Update();
    // On redefinie les valeurs de la position pour éviter le cumule d'erreurs
    if(index < dataSize){
      //x = Cx[index];
      //y = Cy[index];
      //phi = desiredA;
      index ++;
      // Etat arrêt du système
      if(index == dataSize){
        // Program ENDS
        exit(0);
        delay(10000);
      }
      desiredA = atan2(Cy[index]-y,Cx[index]-x);
      Serial.println("DesiredA");
      Serial.print(desiredA);
      Serial.println("phi!");
      Serial.print(phi);
    } else {
      // Program ENDS
      exit(0);
      delay(10000);
    }
    delay(200);
  } else {
    ChangeState(0,1,1,0);
    clockwised();
    anticlockwiseg();
  }
}

/////////////////////////////////////////////////////////////////////////////////
// STATE HANDLER

// Met à jour les états, pour définir le sens de rotation des roues pour le calcul
// de la position et gère aussi la reinitialisation de DCg et DCd pour ne pas incrémenter 
// les valeurs de x et y, lorsque le robot ne tourne pas. 
void Update(){
  if(onRun){
    if(Sclockwiseg){
      //Serial.println("AAA");
      DCg = (-1.0)*stepsize;
    }
    if(Sclockwised){
      //Serial.println("BBB");
      DCd = stepsize;
    }
    if(Santiclockwiseg){
      //Serial.println("CCC");
      DCg = stepsize;
    }
    if(Santiclockwised){
      //Serial.println("DDD");
      DCd = (-1.0)*stepsize;
    }
  }else{
    DCg = 0;
    DCd = 0;
    Santiclockwiseg = 0;
    Santiclockwised = 0;
    Sclockwiseg = 0;
    Sclockwised = 0;
   }
}

// Change l'état du système
void ChangeState(int SCG, int SCD, int SACG, int SACD){
  Sclockwiseg = SCG;
  Santiclockwiseg = SACG;
  Sclockwised = SCD;
  Santiclockwised = SACD;
  //ApplyState(SCG,SCD,SACG,SACD);
}
/* 
 * On n'utilisera pas cette fonction car pour le moment
 * On veut que l'execution des commande roue gauche et 
 * droite ai un minimum de delai entre les deux. 
void ApplyState(int SCG, int SCD, int SACG, int SACD){
  if(SCG){
     clockwiseg();
  }
  if(SCD){
     clockwised();
  }
  if(SACG){
     anticlockwiseg();
  }
  if(SACD){
     anticlockwised();
  }
}*/
////////////////////////////////////////////////////////////////////////////////set pins to ULN2003 high in sequence from 1 to 4
//delay "motorSpeed" between each pin setting (to determine speed)
// Code "bas niveau" pour la gestion du sens de rotation des deux moteurs

void anticlockwiseg(){
for(int i = 0; i < 8; i++){
  setOutputg(i);
  delayMicroseconds(motorSpeed);
  }
}

void clockwiseg(){
  for(int i = 7; i >= 0; i--)
  {
    setOutputg(i);
    delayMicroseconds(motorSpeed);
  }
}

void setOutputg(int out){
  digitalWrite(gmotorPin1, bitRead(lookup[out], 0));
  digitalWrite(gmotorPin2, bitRead(lookup[out], 1));
  digitalWrite(gmotorPin3, bitRead(lookup[out], 2));
  digitalWrite(gmotorPin4, bitRead(lookup[out], 3));
} 

////////////////////////////////////////////////////////////////////////////////set pins to ULN2003 high in sequence from 1 to 4
//delay "motorSpeed" between each pin setting (to determine speed)

void anticlockwised(){
for(int i = 0; i < 8; i++){
  setOutputd(i);
  delayMicroseconds(motorSpeed);
  }
}

void clockwised(){
  for(int i = 7; i >= 0; i--)
  {
    setOutputd(i);
    delayMicroseconds(motorSpeed);
  }
}

void setOutputd(int out){
  digitalWrite(dmotorPin1, bitRead(lookup[out], 0));
  digitalWrite(dmotorPin2, bitRead(lookup[out], 1));
  digitalWrite(dmotorPin3, bitRead(lookup[out], 2));
  digitalWrite(dmotorPin4, bitRead(lookup[out], 3));
} 