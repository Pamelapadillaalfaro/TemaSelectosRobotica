// Definición de pines de control para el L293D
// Asignamos pines digitales del Arduino a las entradas de control (INPUTs)
const int IN1 = 13; // Pin de control 1 del Motor 1 (L293D Pin 2)
const int IN2 = 6; // Pin de control 2 del Motor 1 (L293D Pin 7)
const int IN3 = 12; // Pin de control 1 del Motor 2 (L293D Pin 10)
const int IN4 = 5; // Pin de control 2 del Motor 2 (L293D Pin 15)

// Pines de Habilitación (ENABLEs) - Para el ejercicio 2, se mantienen en HIGH para usar solo 4 señales de dirección.
const int EN1 = 10; // Pin Enable Motor 1 (L293D Pin 1)
const int EN2 = 11; // Pin Enable Motor 2 (L293D Pin 9)

void setup() {
  // Configurar los pines de control como salidas
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Configurar los pines de Enable como salidas
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);

  // Habilitar los motores al inicio (HIGH)
  // Esto permite que los motores se controlen con las 4 señales de dirección (IN1-IN4)
  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, HIGH);

  // Inicializar la comunicación serial para recibir comandos
  Serial.begin(9600);
  Serial.println("Control de Motores");
  Serial.println("Comandos: A1 on left/right | A1 off | A2 on left/right | A2 off");
}

void loop() {
  // Bucle para leer comandos de la interfaz serial de Arduino
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Eliminar espacios en blanco
   
    // Procesar el comando
    if (command.startsWith("A1")) {
      controlMotor1(command);
    } else if (command.startsWith("A2")) {
      controlMotor2(command);
    }
  }
}

// Función para controlar el Motor 1
void controlMotor1(String command) {
  if (command.endsWith("on left")) {
    // Girar a la izquierda
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    Serial.println("Motor 1: Girando a la izquierda");
  } else if (command.endsWith("on right")) {
    // Girar a la derecha
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    Serial.println("Motor 1: Girando a la derecha");
  } else if (command.endsWith("off")) {
    // Apagar (frenar)
    // Poner ambas entradas en LOW detiene el motor (ya que EN1 está en HIGH)
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    Serial.println("Motor 1: Detenido (Off)");
  } else {
    Serial.println("Comando A1 no reconocido. Use 'on left', 'on right' o 'off'.");
  }
}

// Función para controlar el Motor 2
void controlMotor2(String command) {
  if (command.endsWith("on left")) {
    // Girar a la izquierda
    digitalWrite(IN3, HIGH); // Nota: Cambiar HIGH/LOW puede invertir el sentido 'left'/'right' real
    digitalWrite(IN4, LOW);
    Serial.println("Motor 2: Girando a la izquierda");
  } else if (command.endsWith("on right")) {
    // Girar a la derecha
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Motor 2: Girando a la derecha");
  } else if (command.endsWith("off")) {
    // Apagar (frenar)
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    Serial.println("Motor 2: Detenido (Off)");
  } else {
    Serial.println("Comando A2 no reconocido. Use 'on left', 'on right' o 'off'.");
  }
}
