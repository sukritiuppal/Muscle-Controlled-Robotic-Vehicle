#include <LiquidCrystal.h>

// LCD Mapping
LiquidCrystal lcd(A4, A3, A2, A1, A0, 13);

// Pin Definitions
const int trigPin = 12; 
const int echoPin = 2;

// Motor Pins
const int motorL1 = 6; 
const int motorL2 = 7;
const int motorR1 = 8;
const int motorR2 = 9;

char command = 's'; 
int distance = 0;

void setup() {
  Serial.begin(9600); 
  delay(500);
  lcd.begin(16, 2);

  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  pinMode(motorL1, OUTPUT); pinMode(motorL2, OUTPUT);
  pinMode(motorR1, OUTPUT); pinMode(motorR2, OUTPUT);
  lcd.setCursor(0, 0);
  lcd.print("DESHBOT CAR");
  delay(1500);
  lcd.clear();
}

void loop() {
  // --- 1. SENSOR READ ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000); 
  distance = duration * 0.034 / 2;

  // --- 2. BLUETOOTH INPUT ---
  if (Serial.available() > 0) {
    command = Serial.read();
  }

  // --- 3. LOGIC (AUTO-AVOIDANCE) ---
// --- 3. LOGIC (AUTO-AVOIDANCE) ---
  if (distance > 0 && distance < 20) {
    // Obstacle detected!
    stopMotors();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("OBSTACLE! "); lcd.print(distance);
    lcd.setCursor(0, 1);
    lcd.print("TURNING AROUND");
    
    delay(500); 
    
    // Execute turn
    right();
    delay(1000); // Time to turn 180 degrees
    
    // CHANGE IS HERE: 
    // Instead of stopping, we set the command to forward
    command = 'f'; 
    lcd.clear();
    // No stopMotors() here, so it transitions smoothly into executeCommand('f')
  } 
  else {
    // No obstacle, follow current command (which is now 'f' after a turn)
    executeCommand(command);
    
    lcd.setCursor(0, 0);
    lcd.print("DIST: "); lcd.print(distance); lcd.print("cm   ");
  }
  
  delay(50); 
}

void executeCommand(char c) {
  lcd.setCursor(0, 1);
  switch (c) {
    case 'f': lcd.print("MOVING: FORWARD "); forward(); break;
    case 'b': lcd.print("MOVING: BACK    "); backward(); break;
    case 'l': lcd.print("MOVING: LEFT    "); left(); break;
    case 'r': lcd.print("MOVING: RIGHT   "); right(); break;
    case 's': lcd.print("STATUS: STOPPED "); stopMotors(); break;
  }
}

// Movement Logic
void forward() {
  digitalWrite(motorL1, HIGH); digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, HIGH); digitalWrite(motorR2, LOW);
}
void backward() {
  digitalWrite(motorL1, LOW); digitalWrite(motorL2, HIGH);
  digitalWrite(motorR1, LOW); digitalWrite(motorR2, HIGH);
}
void left() {
  digitalWrite(motorL1, LOW); digitalWrite(motorL2, HIGH); 
  digitalWrite(motorR1, HIGH); digitalWrite(motorR2, LOW); 
}
void right() {
  digitalWrite(motorL1, HIGH); digitalWrite(motorL2, LOW); 
  digitalWrite(motorR1, LOW); digitalWrite(motorR2, HIGH); 
}
void stopMotors() {
  digitalWrite(motorL1, LOW); digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW); digitalWrite(motorR2, LOW);
}