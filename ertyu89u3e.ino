#include <SoftwareSerial.h>
#include <Servo.h>
#include <IRremote.h>  // Include the IRremote library

// Defines the pins
const int trigPin = 8;
const int echoPin = 12;
const int recv_pin = 2;  // IR receiver pin

// Create objects for servo motors and IR receiver
Servo radarServo;
Servo mainServo;  // Servo for controlling the main movement (e.g., picking up trash)
Servo clawServo;  // Servo for controlling the claw
IRrecv irrecv(recv_pin);  // IR receiver object
decode_results results;  // To store the received IR code

// Variables for ultrasonic sensor
long duration;
int distance;

// Define IR remote button codes (these might be different based on your remote)
const int button1 = 0xFFA25D;  // Button 1 (e.g., "MAIN DOWN")
const int button2 = 0xFF629D;  // Button 2 (e.g., "MAIN UP")
const int button3 = 0xFF22DD;  // Button 3 (e.g., "CLAW 90")
const int button4 = 0xFFC23D;  // Button 4 (e.g., "CLAW 0")

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  
  // Initialize servo motors
  radarServo.attach(7);  // Radar servo attached to pin 7
  mainServo.attach(3);   // Main servo attached to pin 6 (for main movement)
  clawServo.attach(4);   // Claw servo attached to pin 5 (for the claw)
  
  // Initialize IR receiver
  irrecv.enableIRIn();  // Start the IR receiver
}

void loop() {
  // Check if an IR signal is received
  if (irrecv.decode(&results)) {
    long int decCode = results.value;
    Serial.println(decCode);  // Print received IR code for debugging
    
    // Check the received code and perform actions
    if (decCode == button1) {
      moveMainDown();  // Move the main servo down (Button 1)
    }
    else if (decCode == button2) {
      moveMainUp();  // Move the main servo up (Button 2)
    }
    else if (decCode == button3) {
      turnClaw90();  // Turn the claw servo to 90 degrees (Button 3)
    }
    else if (decCode == button4) {
      turnClaw0();  // Return the claw servo to 0 degrees (Button 4)
    }

    irrecv.resume();  // Receive the next value
  }
  
  Serial.println("Scanning for objects");  // Updated message for scanning
  
  // Continue sweeping the radar servo and measuring distance
  for (int i = 15; i <= 165; i++) {
    radarServo.write(i);  // Sweep radar servo
    delay(30);
    distance = calculateDistance();  // Measure distance from ultrasonic sensor
    
    // Print the current angle and distance to the serial monitor
    Serial.print(i);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(" cm.");
  }

  // Repeats the servo sweeping from 165 to 15 degrees
  for (int i = 165; i > 15; i--) {
    radarServo.write(i);  // Sweep radar servo back
    delay(30);
    distance = calculateDistance();  // Measure distance from ultrasonic sensor
    Serial.print(i);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(" cm.");
  }
}

// Function to calculate the distance using the ultrasonic sensor
int calculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);  // Read the echo pin and get the time in microseconds
  distance = duration * 0.034 / 2;  // Convert the time into distance (in cm)
  return distance;
}

// Function to move the main servo down (Button 1)
void moveMainDown() {
  mainServo.write(165);  // Move main servo to 165 degrees (down)
  delay(500);  // Wait for servo to reach position
}

// Function to move the main servo up (Button 2)
void moveMainUp() {
  mainServo.write(15);  // Move main servo to 15 degrees (up)
  delay(500);  // Wait for servo to reach position
}

// Function to turn the claw servo to 90 degrees (Button 3)
void turnClaw90() {
  clawServo.write(90);  // Move claw servo to 90 degrees
  delay(500);  // Wait for servo to reach position
}

// Function to return the claw servo to 0 degrees (Button 4)
void turnClaw0() {
  clawServo.write(0);  // Move claw servo to 0 degrees
  delay(500);  // Wait for servo to reach position
}
