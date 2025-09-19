int pino_geiger = 2; 
volatile unsigned long contador_pulsos = 0;

void contar_pulsos() {
  contador_pulsos++;
}

void setup() {
  Serial.begin(9600);
  pinMode(pino_geiger, INPUT); 
  attachInterrupt(digitalPinToInterrupt(pino_geiger), contar_pulsos, FALLING);
}

void loop() {
  unsigned long pulsos_anteriores = contador_pulsos;
  delay(1000);

  unsigned long pulsos_novos = contador_pulsos - pulsos_anteriores;

  Serial.print("Pulsos nos últimos 1 segundos: ");
  Serial.println(pulsos_novos);
}