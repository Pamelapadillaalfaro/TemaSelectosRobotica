// -------------------------
// Control de motores con L293D + Encoder N20
// -------------------------

// Pines de control L293D
const int IN1 = 13; // Motor 1 PWM
const int IN3 = 12; // Motor 2
const int EN1 = 11; // Enable Motor 1
const int EN2 = 10; // Enable Motor 2

// Pines del encoder
const int C1M1 = 3;  // Canal A motor 1 (usa interrupción)
const int C1M2 = 2;  // Canal A motor 2 (usa interrupción)

// Contadores de pulsos
volatile long pulsesM1 = 0;
volatile long pulsesM2 = 0;

// Variables para RPM y posición
unsigned long lastTime = 0;
float rpmM1 = 0, rpmM2 = 0;
float vueltasM1 = 0, vueltasM2 = 0;
float gradosM1 = 0, gradosM2 = 0;

// CONFIGURA según tu motor:
const int pulsesPerRevolution = 330; // 11 pulsos * 30 relación = 330

String option;

void setup() {
  // Pines de motor
  pinMode(IN1, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);

  // Pines del encoder
  pinMode(C1M1, INPUT_PULLUP);
  pinMode(C1M2, INPUT_PULLUP);

  // Interrupciones para leer los encoders
  attachInterrupt(digitalPinToInterrupt(C1M1), encoderM1ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(C1M2), encoderM2ISR, RISING);

  // Comunicación serial
  Serial.begin(9600);
  Serial.println("Control de Motores + Lectura de Encoders");
  Serial.println("Comandos: avanza | retrocede | izquierda | derecha | apagado");

  // Deshabilitar motores al inicio
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, LOW);
}

void loop() {
  // Leer comandos seriales
  if (Serial.available() > 0) {
    String option = Serial.readStringUntil('\n');
    option.trim();
    if (option.equalsIgnoreCase("avanza")) moveFront();
    else if (option.equalsIgnoreCase("retrocede")) moveBack();
    else if (option.equalsIgnoreCase("derecha")) turnRight();
    else if (option.equalsIgnoreCase("izquierda")) turnLeft();
    else if (option.equalsIgnoreCase("apagado")) stopMotors();
  }

  // Calcular RPM y posición cada 1 segundo
  unsigned long currentTime = millis();
  if (currentTime - lastTime >= 1000) {
    noInterrupts();
    long countM1 = pulsesM1;
    long countM2 = pulsesM2;
    pulsesM1 = 0;
    pulsesM2 = 0;
    interrupts();

    rpmM1 = (countM1 * 60.0) / pulsesPerRevolution;
    rpmM2 = (countM2 * 60.0) / pulsesPerRevolution;

    vueltasM1 += countM1 / (float)pulsesPerRevolution;
    vueltasM2 += countM2 / (float)pulsesPerRevolution;

    gradosM1 = vueltasM1;
    gradosM2 = vueltasM2;

    Serial.println("---------------");
    Serial.print("Motor 1 -> RPM: "); Serial.print(rpmM1);
    Serial.print(" | Vueltas: "); Serial.print(vueltasM1, 3);
    Serial.print(" | Grados: "); Serial.println(gradosM1, 1);

    Serial.print("Motor 2 -> RPM: "); Serial.print(rpmM2);
    Serial.print(" | Vueltas: "); Serial.print(vueltasM2, 3);
    Serial.print(" | Grados: "); Serial.println(gradosM2, 1);
    Serial.println("---------------");

    lastTime = currentTime;
  }
}

// ----------------------
// Funciones de interrupción
// ----------------------
void encoderM1ISR() {
  pulsesM1++;
}

void encoderM2ISR() {
  pulsesM2++;
}

// ----------------------
// Funciones de control de movimiento
// ----------------------
void moveFront() {
  digitalWrite(IN1, LOW);   // Motor 1 adelante
  digitalWrite(EN1, HIGH);
  digitalWrite(IN3, LOW);   // Motor 2 adelante
  digitalWrite(EN2, HIGH);
  Serial.println("Avanzando");
}

void moveBack() {
  digitalWrite(IN1, HIGH);  // Motor 1 atrás
  digitalWrite(EN1, HIGH);
  digitalWrite(IN3, HIGH);  // Motor 2 atrás
  digitalWrite(EN2, HIGH);
  Serial.println("Retrocediendo");
}

void turnRight() {
  digitalWrite(IN1, HIGH);  // Motor 1 atrás
  digitalWrite(EN1, HIGH);
  digitalWrite(IN3, LOW);   // Motor 2 adelante
  digitalWrite(EN2, HIGH);
  Serial.println("Girando a la derecha");
}

void turnLeft() {
  digitalWrite(IN1, LOW);   // Motor 1 adelante
  digitalWrite(EN1, HIGH);
  digitalWrite(IN3, HIGH);  // Motor 2 atrás
  digitalWrite(EN2, HIGH);
  Serial.println("Girando a la izquierda");
}

void stopMotors() {
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, LOW);
  Serial.println("Motores apagados");
}
