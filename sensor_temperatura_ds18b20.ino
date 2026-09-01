/*
  ============================================================
  Bônus Semana 20 — Sensor de Temperatura Real via Web Serial API
  Placa: Arduino UNO

  Ligações:
    DS18B20 VCC    -> 5V
    DS18B20 GND    -> GND
    DS18B20 DADOS  -> pino digital 11
                      (com resistor de pull-up de ~4,7kΩ
                       entre o fio de dados e o 5V)

  Este sketch imprime UMA linha em JSON por segundo na porta
  Serial, ex: {"temperatura":24.31}
  É esse formato que o hook useSerialSensor.js (React) espera
  para dar JSON.parse em cada linha recebida.

  Bibliotecas necessárias (Gerenciador de Bibliotecas da IDE):
    - OneWire (Paul Stoffregen)
    - DallasTemperature (Miles Burton)
  ============================================================
*/

#include <OneWire.h>
#include <DallasTemperature.h>

#define PINO_DS18B20 11

OneWire oneWire(PINO_DS18B20);
DallasTemperature sensores(&oneWire);

void setup() {
  Serial.begin(9600);
  sensores.begin();
}

void loop() {
  sensores.requestTemperatures();
  float temperaturaC = sensores.getTempCByIndex(0);

  if (temperaturaC == DEVICE_DISCONNECTED_C) {
    Serial.println(F("{\"erro\":\"DS18B20 nao encontrado\"}"));
  } else {
    Serial.print(F("{\"temperatura\":"));
    Serial.print(temperaturaC);
    Serial.println(F("}"));
  }

  delay(1000);
}
