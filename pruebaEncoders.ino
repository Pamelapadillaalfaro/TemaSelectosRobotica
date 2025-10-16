// Pines de los Encoders
// Motor 1
const int encoder1PinA = 2; // DEBE SER UN PIN DE INTERRUPCIÓN (Pin 2 en Arduino Uno)
const int encoder1PinB = 4;

// Motor 2
const int encoder2PinA = 3; // DEBE SER UN PIN DE INTERRUPCIÓN (Pin 3 en Arduino Uno)
const int encoder2PinB = 5;

// Variables para el conteo de pulsos (volatile es crucial para interrupciones)
volatile long encoder1Count = 0;
volatile long encoder2Count = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando Encoders...");

  // Configuración de Pines de ENTRADA con Pull-up
  pinMode(encoder1PinA, INPUT_PULLUP);
  pinMode(encoder1PinB, INPUT_PULLUP);
  pinMode(encoder2PinA, INPUT_PULLUP);
  pinMode(encoder2PinB, INPUT_PULLUP);

  // 1. Adjuntar Interrupción para el Motor 1
  // 'CHANGE' permite contar los flancos ascendentes y descendentes (doble resolución).
  attachInterrupt(digitalPinToInterrupt(encoder1PinA), readEncoder1, CHANGE);

  // 2. Adjuntar Interrupción para el Motor 2
  attachInterrupt(digitalPinToInterrupt(encoder2PinA), readEncoder2, CHANGE);
}

// =======================================================
// RUTINAS DE SERVICIO DE INTERRUPCIÓN (ISR)
// NO deben contener Serial.print() o delay()
// =======================================================

// Función para el Motor 1
void readEncoder1() {
  // Comprueba si el pin B está alto o bajo para determinar la dirección
  if (digitalRead(encoder1PinB) == HIGH) {
    encoder1Count++;
  } else {
    encoder1Count--;
  }
}

// Función para el Motor 2
void readEncoder2() {
  // Comprueba si el pin B está alto o bajo para determinar la dirección
  if (digitalRead(encoder2PinB) == HIGH) {
    encoder2Count++;
  } else {
    encoder2Count--;
  }
}

// =======================================================
// BUCLE PRINCIPAL
// =======================================================

void loop() {
  // Se recomienda deshabilitar temporalmente las interrupciones
  // al leer las variables para asegurar una lectura atómica (sin cambios a mitad de lectura).
  noInterrupts();
  long currentCount1 = encoder1Count;
  long currentCount2 = encoder2Count;
  interrupts();

  Serial.print("E1 Pulsos: ");
  Serial.print(currentCount1);
  Serial.print(" | E2 Pulsos: ");
  Serial.println(currentCount2);

  delay(200);
}
