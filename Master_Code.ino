// Master R4 - Final Muscle Control Bridge
const int exgPin = A0;
const int sampleWindow = 100; // 100ms smoothing window
unsigned int sample;

void setup() {
  Serial.begin(115200);  // For your PC Monitor
  Serial1.begin(9600);   // To HC-05 Master
  pinMode(exgPin, INPUT);
  
  Serial.println("--- MASTER SYSTEM ARMED ---");
}

void loop() {
  unsigned long startMillis = millis();
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // 1. SIGNAL PROCESSING
  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(exgPin);
    if (sample < 1024) {
      if (sample > signalMax) signalMax = sample;
      if (sample < signalMin) signalMin = sample;
    }
  }

  unsigned int musclePower = signalMax - signalMin;

  // 2. THREE-ZONE LOGIC
  Serial.print("POWER: ");
  Serial.print(musclePower);

  if (musclePower <= 60) {
    // ZONE 1: STOP
    Serial1.print('s'); 
    Serial.println(" -> [ SEND: STOP ]");
  } 
  else if (musclePower > 61 && musclePower <= 200) {
    // ZONE 2: FORWARD
    Serial1.print('f'); 
    Serial.println(" -> [ SEND: FORWARD ]");
  } 
  else {
    // ZONE 3: REVERSE
    Serial1.print('b'); 
    Serial.println(" -> [ SEND: REVERSE ]");
  }

  delay(10); 
}