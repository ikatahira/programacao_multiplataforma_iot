
#define LED_BUILTIN 2  // GPIO do LED interno (normalmente 2)

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH); // Liga
  delay(1000);                    // 1 segundo
  digitalWrite(LED_BUILTIN, LOW);  // Desliga
  delay(1000);                    // 1 segundo
}