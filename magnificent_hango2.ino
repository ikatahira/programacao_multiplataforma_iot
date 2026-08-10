#define IR_SENSOR 2
#define LED       3

void setup() {
  pinMode(IR_SENSOR, INPUT);
  pinMode(LED, OUTPUT);

  Serial.begin(115200);
}

void print_stateled(const bool beam, const bool inverted) {
  Serial.print("State[");
  Serial.print(beam ? "High" : "Low");
  Serial.print("]");

  if (inverted) {
    Serial.println(beam ? "Feixe detectado" : "Feixe interrompido");
  } else {
    Serial.println(beam ? "Feixe interrompido" : "Feixe detectado");
  }
}

void loop() {
  const bool beamDetected = digitalRead(IR_SENSOR);

  digitalWrite(LED, beamDetected);

  print_stateled(beamDetected, 0);

  delay(50);
}