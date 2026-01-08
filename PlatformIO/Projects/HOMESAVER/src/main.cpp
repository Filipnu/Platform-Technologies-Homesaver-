#include <Arduino.h>


int TRIG1 = 25;
int ECHO1 = 26;
int TRIG2 = 19;
int ECHO2 = 17;
int LED1 = 12;
int LED2 = 5;
int distance = 1190;

#define SOUND_SPEED 0.034

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

 pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
}

void loop() {
  // Sensor 1
  // Clears the trigPin
  digitalWrite(TRIG1, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIG1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG1, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  int duration1 = pulseIn(ECHO1, HIGH);
  
  // Calculate the distance
  int distanceCm1 = duration1 * SOUND_SPEED/2;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance 1 (cm): ");
  Serial.println(distanceCm1);
  
  delay(1000);

  // Sensor 2
  // Clears the trigPin
  digitalWrite(TRIG2, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIG2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG2, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  int duration2 = pulseIn(ECHO2, HIGH);
  
  // Calculate the distance
  int distanceCm2 = duration2 * SOUND_SPEED/2;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance 2 (cm): ");
  Serial.println(distanceCm2);
  
  delay(1000);

  if(duration1 <= distance){
    digitalWrite(LED1, LOW);
  }
  if(duration2 <= distance){
    digitalWrite(LED2, LOW);
  }
  if(duration1 > distance){
    digitalWrite(LED1, HIGH);
  }
  if(duration2 > distance){
    digitalWrite(LED2, HIGH);
  }

  delay(1000);
}