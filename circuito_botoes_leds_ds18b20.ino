/*
  ============================================================
  Projeto: 4 Botões + 2 LEDs + Sensor de Temperatura DS18B20
  Placa: Arduino UNO
  ============================================================

  LIGAÇÕES (conforme identificado no diagrama):
    Botão 1  -> pino digital 9   (resistor pull-down para GND)
    Botão 2  -> pino digital 10  (resistor pull-down para GND)
    Botão 3  -> pino digital 6   (resistor pull-down para GND)
    Botão 4  -> pino digital 7   (resistor pull-down para GND)
    LED 1    -> pino digital 12  (com resistor em série)
    LED 2    -> pino digital 13  (com resistor em série)
    DS18B20  -> pino digital 11  (dados, com resistor pull-up
                                   de ~4,7kΩ entre o fio de dados e o 5V)

  ATENÇÃO: se algum botão/LED responder "trocado" quando você
  testar, é só ajustar o número do pino correspondente aqui embaixo.

  Biblioteca necessária (instalar pelo Gerenciador de Bibliotecas
  da IDE do Arduino):
    - OneWire (por Paul Stoffregen)
    - DallasTemperature (por Miles Burton)
  ============================================================
*/

#include <OneWire.h>
#include <DallasTemperature.h>

// ---------------- Pinos ----------------
const int pinBotao1  = 9;
const int pinBotao2  = 10;
const int pinBotao3  = 6;
const int pinBotao4  = 7;

const int pinLED1    = 12;
const int pinLED2    = 13;

const int pinDS18B20 = 11;

// ---------------- Sensor DS18B20 ----------------
OneWire oneWire(pinDS18B20);
DallasTemperature sensores(&oneWire);

// Intervalo entre leituras de temperatura (ms)
const unsigned long INTERVALO_LEITURA = 1000;
unsigned long ultimaLeitura = 0;

void setup() {
  Serial.begin(9600);

  pinMode(pinBotao1, INPUT);
  pinMode(pinBotao2, INPUT);
  pinMode(pinBotao3, INPUT);
  pinMode(pinBotao4, INPUT);

  pinMode(pinLED1, OUTPUT);
  pinMode(pinLED2, OUTPUT);

  sensores.begin();

  Serial.println(F("Sistema iniciado."));
}

void loop() {
  // ---- Leitura dos botões ----
  // Como os botões usam resistor pull-down: parado = LOW, pressionado = HIGH
  bool botao1Pressionado = digitalRead(pinBotao1) == HIGH;
  bool botao2Pressionado = digitalRead(pinBotao2) == HIGH;
  bool botao3Pressionado = digitalRead(pinBotao3) == HIGH;
  bool botao4Pressionado = digitalRead(pinBotao4) == HIGH;

  // ---- Lógica simples de exemplo ----
  // LED 1 acende se o Botão 1 OU o Botão 2 forem pressionados
  digitalWrite(pinLED1, (botao1Pressionado || botao2Pressionado) ? HIGH : LOW);

  // LED 2 acende se o Botão 3 OU o Botão 4 forem pressionados
  digitalWrite(pinLED2, (botao3Pressionado || botao4Pressionado) ? HIGH : LOW);

  // ---- Leitura periódica da temperatura ----
  unsigned long agora = millis();
  if (agora - ultimaLeitura >= INTERVALO_LEITURA) {
    ultimaLeitura = agora;

    sensores.requestTemperatures();
    float temperaturaC = sensores.getTempCByIndex(0);

    if (temperaturaC == DEVICE_DISCONNECTED_C) {
      Serial.println(F("{\"erro\":\"DS18B20 nao encontrado\"}"));
    } else {
      // Uma linha em JSON por leitura -> facil de ler no navegador
      // com a Web Serial API (JSON.parse de cada linha).
      Serial.print(F("{\"temperatura\":"));
      Serial.print(temperaturaC);
      Serial.print(F(",\"botao1\":")); Serial.print(botao1Pressionado);
      Serial.print(F(",\"botao2\":")); Serial.print(botao2Pressionado);
      Serial.print(F(",\"botao3\":")); Serial.print(botao3Pressionado);
      Serial.print(F(",\"botao4\":")); Serial.print(botao4Pressionado);
      Serial.println(F("}"));
    }
  }

  delay(100);
}
