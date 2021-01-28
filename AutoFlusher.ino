// AutoFlusher Project by Matthew Rada
// Last updated on 01/26/21

#include "Stepper.h"

// set parameters and pins
#define STEPS  32   // Number of steps per revolution of Internal shaft
int  Steps2Take;  // 2048 = 1 Revolution
const int TRIG_PIN = 7; // Arduino pin connected to Ultrasonic Sensor's TRIG pin
const int ECHO_PIN = 6; // Arduino pin connected to Ultrasonic Sensor's ECHO pin
Stepper small_stepper(STEPS, 8, 10, 9, 11);

// variables that may need to be changed based on how you/your toilet works
const int maximumDistance = 65; // 65cm; maximum distance needed to initiate flushing
const int waitTime = 5; // 5 seconds; minimum time needed to initiate flushing
const int flushDelay = 6000; // 2000ms = 2s; set how long toilet will be left open to flush

// variables will change; used as counters
long duration_us;
int distance_cm;
long flushTime = 0;
long countFlush = 0;

// set up ultrasound sensor 
void setup() 
  {
  pinMode(TRIG_PIN, OUTPUT); // set arduino pin to output mode
  pinMode(ECHO_PIN, INPUT);  // set arduino pin to input mode
  Serial.begin(9600);//Initialization of Serial Port
  }

// begin observation and process conditions 
void loop() {
    {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(5);

    // generate 10-microsecond pulse to TRIG pin
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
  
    // measure duration of pulse from ECHO pin
    duration_us = pulseIn(ECHO_PIN, HIGH);
    // calculate the distance
    distance_cm = 0.017 * duration_us;
  
    // print the value to Serial Monitor
    Serial.print("distance: ");
    Serial.print(distance_cm);
    Serial.println(" cm");
    delay(1000);
    }

    // when you are within range, you are timed for how long you are there
  if (distance_cm <= maximumDistance)
    {
    flushTime++;
    Serial.print(flushTime);
    Serial.println("s");
    } else if (flushTime >= waitTime) {
      flushTime >= waitTime;
    } else {flushTime = 0;}
   
    // initiate flushing; power on motor, delay, then turn off motor to avoid overheating
  if (distance_cm >= maximumDistance && distance_cm <= 200 && flushTime >= waitTime)  
    {
    digitalWrite (8,HIGH);
    digitalWrite (9,HIGH);
    digitalWrite (10,HIGH);
    digitalWrite (11,HIGH);
    small_stepper.setSpeed(15); //Speed in rpm
    Steps2Take  =  5120;  // 2048 = 1 rotation; feel free to modify rotation based on the lenght of your string
    small_stepper.step(Steps2Take);
    delay(flushDelay); 
                      
    small_stepper.setSpeed(15);
    Steps2Take  = -5120;  // 2048 = 1 rotation; feel free to modify rotation based on the length of your string
    small_stepper.step(Steps2Take);
    delay(500); 
    digitalWrite (8,LOW);
    digitalWrite (9,LOW);
    digitalWrite (10,LOW);
    digitalWrite (11,LOW);            

    countFlush++;
    Serial.print("flushes: ");
    Serial.println(countFlush);
    
    flushTime = 0;
    } 

  //if (distance_cm >= maximumDistance)
    //{flushTime = 0;}
    //return;
}
    
