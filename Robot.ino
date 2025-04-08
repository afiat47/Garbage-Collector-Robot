#include <Servo.h>

#define IR_SENSOR_RIGHT 51
#define IR_SENSOR_LEFT 53
#define TRIGGER_PIN 12
#define MOTOR_SPEED 180
int count = 0;
//Right motor
int enableRightMotor = 7;
int rightMotorPin1 = 52;
int rightMotorPin2 = 50;

//Left motor
int enableLeftMotor = 8;
int leftMotorPin1 = 48;
int leftMotorPin2 = 46;
int terminate = 0;
int isDone = 0;

int leftBoxPin = 2, rightBoxPin = 3;

Servo myservo1;
Servo myservo2;
Servo myservo3;// create servo object to control a servo
Servo boxLeftServo;
Servo boxRightServo;
// twelve servo objects can be created on most boards

float pos = 167;
float pos2 = 0; // variable to store the servo position


void setup()
{
  Serial.begin(9600);
  //The problem with TT gear motors is that, at very low pwm value it does not even rotate.
  //If we increase the PWM value then it rotates faster and our robot is not controlled in that speed and goes out of line.
  //For that we need to increase the frequency of analogWrite.
  //Below line is important to change the frequency of PWM signal on pin D5 and D6
  //Because of this, motor runs in controlled manner (lower speed) at high PWM value.
  //This sets frequecny as 7812.5 hz.
  //TCCR0B = TCCR0B & B11111000 | B00000010 ;

  // put your setup code here, to run once:
  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  pinMode(IR_SENSOR_RIGHT, INPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);
  initServo();
  Serial.println("setup");
}


void initServo() {
  Serial.println("initServo");
  myservo1.attach(12); // left 9
  myservo2.attach(11); // right 8
  myservo3.attach(10); // middle 7

  //boxLeftServo.attach(2);
  boxRightServo.attach(4);

  myservo3.write(0);
  myservo1.write(67);
  myservo2.write(100);

  //boxLeftServo.write(0);
  boxRightServo.write(150);
  delay(5000);
}


void initAll()
{
  
  isDone = 1;
//  throwTrash();
//  Serial.println("Calling");
//  return;

  runCar();
  rotateMotor(0, 0);


  delay(1000);
  pickTrash();
  rotateMotor(0,0);

  rotateMotor(-MOTOR_SPEED, MOTOR_SPEED);
  revertCar();

  runCar();
  rotateMotor(0,0);

  rotateMotor(-MOTOR_SPEED, MOTOR_SPEED);
  revertCar();
  rotateMotor(0, 0);
  throwTrash();
  
  //throwTrash();
  
}


void loop() {

  
  Serial.println("loop");
  if (isDone) {
    delay(5000);
    return;
  }
  Serial.println("initAll-loop");
  initAll();
}

void runCar() {
  Serial.println("runCar");
  while (true) {
    int rightIRSensorValue = digitalRead(IR_SENSOR_RIGHT);
    int leftIRSensorValue = digitalRead(IR_SENSOR_LEFT);

    //If none of the sensors detects black line, then go straight
    if (rightIRSensorValue == LOW && leftIRSensorValue == LOW)
    {
      rotateMotor(MOTOR_SPEED, MOTOR_SPEED);
    }
    //If right sensor detects black line, then turn right
    else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW )
    {
      rotateMotor(-MOTOR_SPEED, MOTOR_SPEED);
    }
    //If left sensor detects black line, then turn left
    else if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH )
    {
      rotateMotor(MOTOR_SPEED, -MOTOR_SPEED);
    }
    //If both the sensors detect black line, then stop
    else if (rightIRSensorValue == HIGH && leftIRSensorValue == HIGH) { //first stop
      //stopp(0,0);
      break;
    }
  }
}


void pickTrash() {
  //down

  Serial.println("down");
  float inc = 2;
  for (pos = 100, pos2 = 67; pos >= 0 && pos2 <= 167; pos2 = pos2 + inc, pos -= inc) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo1.write(pos2);
    myservo2.write(pos);   // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 15ms for the servo to reach the position
  }

 delay(1000);
  Serial.println("shrink"); 
  for (int i = 0; i < 90; i += 5) {
   myservo3.write(i);
    delay(10);
  }

 delay(1000);

  Serial.println("top");
  //top
  for (pos = 167, pos2 = 0; pos >= 0 && pos2 <= 167; pos2 = pos2 + inc, pos -= inc) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo1.write(pos);
    myservo2.write(pos2);   // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 15ms for the servo to reach the position
  }

//    myservo2.write(167);
//    myservo1.write(0);
  

  delay(1000);

  Serial.println("release");
  //release
  for (int i = 0; i <= 90; i += 5) {
    myservo3.write(110 - i);
    delay(10);
  }

  delay(1000);

  //normal
  Serial.println("Normal");
  for (int i = 0; i <= 67; i++) {
    myservo1.write(i);
    myservo2.write(167 - i);
  }
}

void throwTrash() {
  Serial.println("throwTrash");
  for (int i = 150; i >= 50; i=i-5) {
    //boxLeftServo.write(i);
    boxRightServo.write(i);
    delay(10);
  }

  delay(1000);

  for (int i = 50; i <= 150; i++) {
    boxRightServo.write(i);
    delay(10);
  }
  delay(1000);
}


void revertCar() {
  Serial.println("RevertCar");
  int ok = 0;

  while (1) {

    int rightIRSensorValue = digitalRead(IR_SENSOR_RIGHT);
    int leftIRSensorValue = digitalRead(IR_SENSOR_LEFT);
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);

    if (rightIRSensorValue == LOW) ok = 1;

    if (ok && rightIRSensorValue == HIGH) break;
    delay(10);
  }

}


void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{

  if (rightMotorSpeed < 0)
  {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  }
  else
  {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
  }

  if (leftMotorSpeed < 0)
  {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
  }
  else
  {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
  }
  analogWrite(enableRightMotor, abs(rightMotorSpeed));
  analogWrite(enableLeftMotor, abs(leftMotorSpeed));
}
