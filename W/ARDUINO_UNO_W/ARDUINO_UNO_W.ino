#include <Servo.h>

//Servo myservo;
Servo my2servo;
Servo my3servo;

int pos = 0;

#define trigPin  A5
#define echoPin  A4
// defines variables
long duration;
int distance;

#define Dout A1
float metalDetected;
int monitoring;
int metalDetection = 1;

#include <Stepper.h>

// defines pins numbers
const int dirPin  = 3;
const int stepPin = 4;
const int enPin   = 5;

//DC motor
byte ena = 6; //oramgedriveแดง
byte in1 = 7; //greendriveแดง
byte in2 = 8; //ขี้ม้าdriveแดง

#include <HX711.h>
#include "HX711.h"

float calibration_factor = 427569.00;
#define zero_factor 8518738

#define DOUT  A3
#define CLK   A2
#define DEC_POINT  4

//#define STEPS 100
//Stepper motorStep(STEPS, 3, 4, 5);

float offset = 0;
float get_units_kg();

//void firstmotor();
void motorStep( int x);

HX711 scale(DOUT, CLK);

#include <SoftwareSerial.h>
// UNO Pin 12 -> Pin D5 NodeNCU, UNO Pin 13 -> Pin D6 NodeMCU
SoftwareSerial NodeSerial(12, 13);
int positionData = 0;

void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  Serial.begin(9600);
  NodeSerial.begin(9600);
  scale.set_scale(calibration_factor);
  scale.set_offset(zero_factor);

  // Sets the two pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);

  //DC motor
  pinMode( ena, OUTPUT );
  pinMode( in1, OUTPUT );
  pinMode( in2, OUTPUT );

  my2servo.attach(11); //ที่เปิด
  my3servo.attach(10); //ที่ปัด
}

void loop()
{
  {
    //motorStep(1);
    digitalRead(enPin) == LOW;
    digitalRead(dirPin) == LOW;
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
    digitalWrite(trigPin, LOW); // Clears the trigPin
    digitalWrite(trigPin, HIGH); // Sets the trigPin on HIGH state for 10 micro seconds
    delay(100);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH); // Reads the echoPin
    
    // Calculate the distance (Garbage in the bin)
    // lid of the bin open
    distance = duration * 0.034 / 2;
    Serial.print("Distance: ");
    Serial.println(distance);
    {
      if (distance < 45 && distance >= 0) {
        {
          analogWrite( ena, 300 );
          digitalWrite( in1, HIGH );
          digitalWrite( in2, LOW );
          Serial.print("Uppppppp");
          delay(10000);
          
          analogWrite( ena, 0 );
          digitalWrite( in1, LOW );
          digitalWrite( in2, LOW );
          Serial.print("hold on");
          delay(4000);
          
          analogWrite( ena, -300 );
          digitalWrite( in1, LOW );
          digitalWrite( in2, HIGH );
          Serial.print("downnnnnn");
          delay(9000);

          analogWrite( ena, 0 );
          digitalWrite( in1, LOW );
          digitalWrite( in2, LOW );
        }

        //waste separation process****************************************************
        monitoring = analogRead(Dout);
        metalDetected = (float) monitoring * 100 / 1024.0;
        Serial.print("Metal is Proximited = ");
        Serial.print(metalDetected);
        Serial.println("%");
        //Metal trash detection
        if (monitoring > 250)
        {
          Serial.println("Metal is Detected");
          delay(500);
          float weight = get_units_kg() + 0.0125;
          Serial.print("Reading: ");
          Serial.print(weight + offset, DEC_POINT);
          Serial.println(" kg");
          Serial.println(weight + offset, 4);
          if (weight + offset <= 0.000) {
            Serial.println("Put your trash");
            Serial.println("----------------------------------------------------------");
            delay(5000);
          }

          else if (weight + offset <= 0.02)

          {
            Serial.println("CARTON");
            Serial.println("----------------------------------------------------------");

            my3servo.write(90);
            delay(1000);
            my3servo.write(45);
            delay(1000);
            my3servo.write(80);
            delay(1000);

            //180 degree
            digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
            for (int x = 0; x < 1500; x++)
            {
              digitalWrite(stepPin, HIGH);
              delayMicroseconds(2000);
              digitalWrite(stepPin, LOW);
              delayMicroseconds(2000);
            }
            delay(1000); // One second delay
            my2servo.write(-90);
            delay(2000);
            my2servo.write(90);
            delay(2000);

            positionData = 3;                      
            Serial.println(positionData);
            //        Serial.println(analogData);
            if (NodeSerial.available() > 0)
            {
              NodeSerial.write(positionData);
            }
            
            digitalWrite(dirPin, LOW); // Enables the motor to move in a particular direction
            for (int x = 0; x < 1500; x++)
            {
              digitalWrite(stepPin, HIGH);
              delayMicroseconds(2000);
              digitalWrite(stepPin, LOW);
              delayMicroseconds(2000);
            }
          }

          else
          {

            Serial.println("CAN");
            Serial.println("----------------------------------------------------------");
            
            my3servo.write(90);
            delay(1000);
            my3servo.write(45);
            delay(1000);
            my3servo.write(80);
            delay(1000);

            //270 degree
            digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
            for (int x = 0; x < 2300; x++)
            {
              digitalWrite(stepPin, HIGH);
              delayMicroseconds(2000);
              digitalWrite(stepPin, LOW);
              delayMicroseconds(2000);
            }
            delay(1000); // One second delay
            my2servo.write(-90);
            delay(2000);
            my2servo.write(90);
            delay(2000);

            positionData = 4;                       
            Serial.println(positionData);
            if (NodeSerial.available() > 0)
            {
              NodeSerial.write(positionData);
            }
            
            digitalWrite(dirPin, LOW); // Enables the motor to move in a particular direction
            for (int x = 0; x < 2300; x++)
            {
              digitalWrite(stepPin, HIGH);
              delayMicroseconds(2000);
              digitalWrite(stepPin, LOW);
              delayMicroseconds(2000);
            }
            delay(10000); // One second delay
          }
        }


        if (monitoring < 250)
        {
          Serial.println("NON-Metal is Detected");
          Serial.print("Reading: ");
          float weight = get_units_kg() + 0.0125;
          Serial.print("Reading: ");
          Serial.print(weight + offset, DEC_POINT);
          Serial.println(" kg");
          if (weight + offset <= 0.005) {
            Serial.println("Put your trash");
            Serial.println("----------------------------------------------------------");
            delay(5000);
          }
          else if (weight + offset > 0.03)
          {

            Serial.println("GLASS");
            Serial.println("----------------------------------------------------------");

            my3servo.write(90);
            delay(1000);
            my3servo.write(45);
            delay(1000);
            my3servo.write(80);
            delay(1000);

            //0 degree
            digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction

            for (int x = 0; x++;)
            {
              digitalWrite(stepPin, HIGH);
              delayMicroseconds(2000);
              digitalWrite(stepPin, LOW);
              delayMicroseconds(2000);
            }
            delay(1000); // One second delay

            my2servo.write(-90);
            delay(2000);
            my2servo.write(90);
            delay(2000);

            positionData = 2;                       
            Serial.println(positionData);
            if (NodeSerial.available() > 0)
            {
              NodeSerial.write(positionData);
            }

          }
          else
          {

            Serial.println("PLASTIC BOTTLE");
            Serial.println("----------------------------------------------------------");
            delay(500);

            my3servo.write(90);
            delay(1000);
            my3servo.write(45);
            delay(1000);
            my3servo.write(80);
            delay(1000);

            //90 degree
            digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
            for (int x = 0; x < 700; x++)
            {
              digitalWrite(stepPin, HIGH);
              delayMicroseconds(2000);
              digitalWrite(stepPin, LOW);
              delayMicroseconds(2000);
            }
            delay(1000); // One second delay

            my2servo.write(-90);
            delay(2000);
            my2servo.write(90);
            delay(2000);

            positionData = 1;                     
            Serial.println(positionData);

            if (NodeSerial.available() > 0)
            {
              NodeSerial.write(positionData);
            }
            
            digitalWrite(dirPin, LOW); // Enables the motor to move in a particular direction
            for (int x = 0; x < 700; x++)
            {
              digitalWrite(stepPin, HIGH);
              delayMicroseconds(2000);
              digitalWrite(stepPin, LOW);
              delayMicroseconds(2000);
            }
            delay(10000); // One second delay
          }
        }
      }
    }
  }
}


float get_units_kg()
{
  return (scale.get_units() * 0.453592);
}

void motorStep( int x)
{
  for (int x = 0; x <= 1600; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
}
