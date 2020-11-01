// Pins
const int PWMB = 8; 
const int BIN2 = 9;
const int BIN1 = 10;
const int AIN1 = 11;
const int AIN2 = 12;
const int PWMA = 13;
const int SW_PIN = 7;     // Switch to turn the motors on and off
const int LIGHT_PIN = A0; // Photocell
const int LED = 2;        //LED      
const int echoPin = 5;
const int trigPin = 6; 

// Constants
const int SEARCH_DRIVE_TIME = 200;  // Time to run one motor while searching
const int TURN_DRIVE_TIME = 200;    // Time to turn in a direction
const int MOVE_DRIVE_TIME = 300;   // Time to drive in a direction
const int STOP_DRIVE_TIME = 200;    // Time to stop after moving
const int NUM_LIGHT_LEVELS = 3;
long duration, distance;
void setup() {
  // Set switch pin 
  pinMode(7, INPUT_PULLUP);
  pinMode(5, INPUT);
  

  // Set the motor control pins as outputs
  pinMode(6, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(2, OUTPUT);

  // Initialize Serial comms
  Serial.begin(9600);
  Serial.println("Let's get this Light "); 
 
}
void loop() {
  digitalWrite(6, LOW);
  delayMicroseconds(5);
  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(6, HIGH);
  delayMicroseconds(10);
  digitalWrite(6, LOW);
  // Read the echoPin, pulseIn() returns the duration (length of the pulse) in microseconds:
  duration = pulseIn(5, HIGH);
  // Calculate the distance:
  distance= duration*0.034/2;
  if (distance > 20){
    digitalWrite(2,HIGH);
     }
   else {
    (digitalWrite(2,LOW));
    drive(-255, 255);
    delay (800);
     }
  // Print the distance on the Serial Monitor:
  Serial.print("Distance = ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(100);

  // Store light levels as array [left, center, right]
  int light_levels[3];

  // If switch is flipped, search for light
  if (digitalRead(7) == LOW)  {
  // Record light value to the left
    drive(0, -255);
    delay(400);
    light_levels[0] = analogRead(A0);
    drive (0, 255);
    delay (400);
    drive(0, 0);
    delay(200);
    
 // Record light value to the right
    drive(255, 0);
    delay(400);
    light_levels[2] = analogRead(A0);
    drive (-255, 0);
    delay(400);
    drive(0, 0);
    delay(200);
      
    // Record light value in the center
    light_levels[1] = analogRead(A0);
    
    // Find direction of max light
    int max_light = 0;
    int max_light_index = 0;
    for ( int i = 0; i < 3; i++ ) {
      if ( light_levels[i] > max_light ) {
        max_light = light_levels[i];
        max_light_index = i;
      }
      Serial.print(light_levels[i]);
      Serial.print(" ");
    }
    Serial.println();
    Serial.print("Max light: ");
    Serial.println(max_light_index);

    // Move in the direction of max light
      if ( max_light_index == 0 ){
      Serial.println("Chasing brightest light on the left");
      
      drive(0, -255);
      delay(600);
      drive(0, 0);
      delay(200);
                   
      } 
      else if ( max_light_index == 1 ) {
      Serial.println("Chasing brightest light straight ahead");
      
      drive(255, -255);
      delay(210);
      drive(0, 0);
      delay(200); 
      
      }
      else {
      Serial.println("Chasing brightest light to the right");
      
      drive(255, 0);
      delay(600);
      drive(0, 0);
      delay(200);
      } 
   
  // If switch is not flipped, do nothing
  } else {
    digitalWrite(5,LOW);
    digitalWrite(6,LOW);
    drive(0, 0);
    }
    
}

void rightMotor(int motorSpeed)                       
{

  // If speed is positive, run the motor forward
  if (motorSpeed > 0) {
    digitalWrite(11, HIGH); 
    digitalWrite(12, LOW);

  // If it's negative, run the motor backward
  } else if (motorSpeed < 0) {
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);

  // If it's 0, brake the motor
  } else {
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
  }
  analogWrite(13, abs(motorSpeed));
}

void leftMotor(int motorSpeed)
{
  // If speed is positive, run the motor forward
  if (motorSpeed > 0) {
    digitalWrite(10, HIGH); 
    digitalWrite(9, LOW);

  // If it's negative, run the motor backward
  } else if (motorSpeed < 0) {
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);

  // If it's 0, brake the motor
  } else {
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
  }
  analogWrite(8, abs(motorSpeed));
}

void drive(int leftSpeed, int rightSpeed) {
  leftMotor(leftSpeed);
  rightMotor(rightSpeed);
  
  }