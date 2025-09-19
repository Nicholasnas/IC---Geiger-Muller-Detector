#include <Arduino.h>



int pino_geiger = 2;
volatile unsigned long contador_pulsos = 0;
unsigned long tempo_inicio = 0;
const float fator_calibracao = 153.8; // Fator de calibração

void contar_pulsos() {
  // Toda vez que um evento(pulso) é detectado chama essa funcao
  contador_pulsos++;
}

void setup() {
  Serial.begin(9600);

  pinMode(pino_geiger, INPUT);
  attachInterrupt(digitalPinToInterrupt(pino_geiger), contar_pulsos, FALLING);

  tempo_inicio = millis(); // Iniciar a contagem de tempo
}

void loop() {
  unsigned long tempo_atual = millis();

  // Verifica se um minuto (60000ms) se passou 
   if (tempo_atual - tempo_inicio >= 60000) {
    noInterrupts();
    unsigned long pulsos = contador_pulsos;
    contador_pulsos = 0;
    interrupts();

    // Resetar o tempo para o proximo ciclo de 1 min
    tempo_inicio = tempo_atual;

    float cpm = pulsos; // CPM
    float cps = float(pulsos)/ 60; // CPS
    float uSv_h = cpm / fator_calibracao; // Conversao para µSv/h
    float mrem_h = uSv_h / 10.0; // Conversao para mREM/h

   
    Serial.print(cps, 3 );
    Serial.print(",");
    Serial.print(cpm);
    Serial.print(",");
    Serial.print(uSv_h, 3); // numero de casas decimais
    Serial.print(",");
    Serial.println(mrem_h, 3);
  }
}