/*
  Triagem hospitalar (ENADE) - simulação com LEDs no Arduino Uno
  ----------------------------------------------------------------
  Cada posição da fila = 1 LED verde + 1 LED amarelo (só um dos dois
  acende, indicando a cor do cartão daquele paciente na posição).

  Ligações (LED -> resistor 220R -> GND):
    Posição 1: verde D2  | amarelo D3
    Posição 2: verde D4  | amarelo D5
    Posição 3: verde D6  | amarelo D7
    Posição 4: verde D8  | amarelo D9
    Posição 5: verde D10 | amarelo D11
    Botão:     D12 -> GND (usa INPUT_PULLUP)

  A cada clique no botão, insere o próximo paciente da sequência de
  teste (a mesma do enunciado: 10-V, 11-V, 5-A, 12-V, 6-A) e
  reorganiza os LEDs conforme a lista fica.
*/

#include <Arduino.h>

// ---------- Lista encadeada (igual ao código do enunciado) ----------
struct lista {
  int numero;
  char cor;
  struct lista* prox;
};
typedef struct lista Lista;

Lista* inserir_fim(Lista* l, Lista* no) {
  Lista* aux;
  no->prox = NULL;
  if (l == NULL) return no;
  aux = l;
  while (aux->prox != NULL)
    aux = aux->prox;
  aux->prox = no;
  return l;
}

Lista* inserir_prioridade(Lista* l, Lista* no) {
  Lista* aux;
  if (l == NULL || l->cor == 'V') {
    no->prox = l;
    l = no;
  } else {
    aux = l;
    while (aux->prox != NULL && aux->prox->cor == 'A')
      aux = aux->prox;
    no->prox = aux->prox;
    aux->prox = no;
  }
  return l;
}

Lista* inserir(Lista* l, int numero, char cor) {
  Lista* no = (Lista*) malloc(sizeof(Lista));
  no->numero = numero;
  no->cor = cor;

  if (l == NULL) {
    no->prox = l;
    l = no;
  } else {
    if (cor == 'V')
      l = inserir_fim(l, no);
    else
      l = inserir_prioridade(l, no);
  }
  return l;
}

// ---------- Configuração dos LEDs ----------
const int NUM_SLOTS = 5;
const int pinVerde[NUM_SLOTS]   = {2, 4, 6, 8, 10};
const int pinAmarelo[NUM_SLOTS] = {3, 5, 7, 9, 11};
const int pinBotao = 12;

// ---------- Sequência de teste (a do enunciado) ----------
const int  seqNum[]  = {10, 11, 5, 12, 6};
const char seqCor[]  = {'V', 'V', 'A', 'V', 'A'};
const int  totalPacientes = 5;
int indiceAtual = 0;

Lista* fila = NULL;

void atualizarLEDs() {
  // apaga tudo primeiro
  for (int i = 0; i < NUM_SLOTS; i++) {
    digitalWrite(pinVerde[i], LOW);
    digitalWrite(pinAmarelo[i], LOW);
  }

  Lista* aux = fila;
  int pos = 0;
  Serial.print("Fila atual: ");
  while (aux != NULL && pos < NUM_SLOTS) {
    if (aux->cor == 'V')
      digitalWrite(pinVerde[pos], HIGH);
    else
      digitalWrite(pinAmarelo[pos], HIGH);

    Serial.print(aux->numero);
    Serial.print(aux->cor);
    Serial.print("  ");

    aux = aux->prox;
    pos++;
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_SLOTS; i++) {
    pinMode(pinVerde[i], OUTPUT);
    pinMode(pinAmarelo[i], OUTPUT);
  }
  pinMode(pinBotao, INPUT_PULLUP);

  Serial.println("Pressione o botao para inserir o proximo paciente.");
}

void loop() {
  static bool ultimoEstado = HIGH;
  bool estadoAtual = digitalRead(pinBotao);

  // detecta borda de descida (botao pressionado) com debounce simples
  if (ultimoEstado == HIGH && estadoAtual == LOW) {
    delay(30); // debounce
    if (digitalRead(pinBotao) == LOW) {
      if (indiceAtual < totalPacientes) {
        int numero = seqNum[indiceAtual];
        char cor   = seqCor[indiceAtual];

        Serial.print("Inserindo paciente ");
        Serial.print(numero);
        Serial.print("-");
        Serial.println(cor);

        fila = inserir(fila, numero, cor);
        atualizarLEDs();

        indiceAtual++;
      } else {
        Serial.println("Todos os pacientes da sequencia ja foram inseridos.");
      }
    }
  }
  ultimoEstado = estadoAtual;
}
