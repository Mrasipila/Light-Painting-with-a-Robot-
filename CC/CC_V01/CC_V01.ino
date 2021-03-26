  /*-----------------------------------------
Encoder with Interruption test
Falitiana ANDRIANANTOANDRO, 2021
-----------------------------------------*/

#include <motor.h>

// left and right motors with corresponding pins
Motor leftMotor(leftMotorPwmPin, leftMotorDirPin);
Motor rightMotor(rightMotorPwmPin, rightMotorDirPin);

// global variables
int uL = 0;
int uR = 0;
int leftInc = 10;
int rightInc = 10;
int currentStateLH_A;
int currentStateRH_A;
int previousStateLH_A;
int previousStateRH_A;
String encdirL =" ";
String encdirR =" ";

// variables to store the number of encoder pulses
// for each motor
volatile long int leftCount = 0;
volatile long int rightCount = 0;


// ***** SETUP ********
void setup() {
Serial.begin(9600);

 // put your setup code here, to run once:
pinMode(LH_ENCODER_A, INPUT_PULLUP);
pinMode(LH_ENCODER_B, INPUT_PULLUP);
pinMode(RH_ENCODER_A, INPUT_PULLUP);
pinMode(RH_ENCODER_B, INPUT_PULLUP);

//Read the initial state of encoders
//Assign to previousStateEnc_x
previousStateLH_A= digitalRead(LH_ENCODER_A);
previousStateRH_A= digitalRead(RH_ENCODER_A);


// initialize hardware interrupts
attachInterrupt(digitalPinToInterrupt(LH_ENCODER_A), leftEncoderEvent, CHANGE);
attachInterrupt(digitalPinToInterrupt(RH_ENCODER_A), rightEncoderEvent, RISING);

}
// ***************

// ***** LOOP ********
void loop() {

//Read the current state of encoders
//currentStateLH_A= digitalRead(LH_ENCODER_A);
//currentStateRH_A= digitalRead(RH_ENCODER_A);

 leftMotor.setU(uL);
rightMotor.setU(uR);

 /* uL=uL+leftInc;
if (uL>=200){
uL=200;
leftInc=0;
}
*/

/* Update previousStateLH_A with current state */
// previousStateLH_A= currentStateLH_A;
// previousStateRH_A= currentStateRH_A;

// leftMotor.stop();
// rightMotor.stop();
// delay(100);
}
// ***************

// encoder event for the interrupt call
void leftEncoderEvent() {

if (digitalRead(LH_ENCODER_A)==LOW)
{
  if (digitalRead(LH_ENCODER_B) == HIGH) 
  {
    leftCount++;
    encdirL="Forward"; 
  } 
  else 
  {
    leftCount--;
  encdirL="Backward";
  }
  if(leftCount>=100)
  { 
    uL = uL + leftInc;
    leftCount=0;
    leftMotor.setU(uL);
    if (uL>=200) { uL=200; leftInc=0; }
  }   
  
  Serial.print("Left Count: ");
  Serial.println(leftCount);
  Serial.println();
  Serial.print("Left Motor direction:");
  Serial.println(encdirL);
  Serial.println();
  Serial.print("Left Motor control values: ");
  Serial.print(leftMotor.getU());
} 
else
{
  if (digitalRead(LH_ENCODER_B) == HIGH) 
  {
    leftCount--;
    encdirL="Backward";
  } 
  else 
  {
    leftCount++;
    encdirL="Forward";
  }
  if(leftCount<=-100)
  {
    uL = uL - leftInc;
    leftCount=0;
    leftMotor.setU(uL);
    if (uL<=-200)
    {
      uL=-200;
      leftInc=0;
    }
  }
  Serial.print("Left Count: ");
  Serial.println(leftCount);
  Serial.println();
  Serial.print("Left Motor direction:");
  Serial.println(encdirL);
  Serial.println();
  Serial.print("Left Motor control values: ");
  Serial.print(leftMotor.getU());
 }
}


// encoder event for the interrupt call
void rightEncoderEvent() 
{
  if (digitalRead(LH_ENCODER_A)==LOW && digitalRead(RH_ENCODER_B) == HIGH)
  {
    rightCount++;
    encdirR="Forward";
  }
  
  if (digitalRead(LH_ENCODER_A)==LOW && digitalRead(RH_ENCODER_B) == LOW)
  {
    rightCount--;
    encdirR="Backward";
  }
  
  if(rightCount>=100)
  {
    uR = uR + rightInc;
    rightCount=0;
    rightMotor.setU(uR);
    if (uR>=200) { uR=200; rightInc=0; }
  }
  /*
  Serial.print("Right Count: ");
  Serial.println(rightCount);
  Serial.println();
  Serial.print("Motor direction: Right:");
  Serial.println(encdirR);
  Serial.println();
  Serial.print("Motor control values: Right: ");
  Serial.print(rightMotor.getU());
  */
 
 else
 {
    if (digitalRead(RH_ENCODER_B) == HIGH) 
    {
      rightCount--;
      encdirR="Backward";
    } 
    else 
    {
      rightCount++;
      encdirR="Forward";
    }
    if(rightCount<=-100)
    {
      uR = uR - rightInc;
      rightCount=0;
      rightMotor.setU(uR);

      if (uR<=-200)
      {
        uR=-200;
        rightInc=0;
      }
    }
    Serial.print("Right Count: ");
    Serial.println(rightCount);
    Serial.println();
    Serial.print("Motor direction: Right:");
    Serial.println(encdirR);
    Serial.println();
    Serial.print("Motor control values: Right: ");
    Serial.print(rightMotor.getU());
    }
}
