// Definición de pines de control para el L293D
// Asignamos pines digitales del Arduino a las entradas de control (INPUTs)
const int IN1 = 13; // Pin de control 1 del Motor 1 (L293D Pin 2)
//const int IN2 = 6; // Pin de control 2 del Motor 1 (L293D Pin 7)
const int IN3 = 12; // Pin de control 1 del Motor 2 (L293D Pin 10)
//const int IN4 = 5; // Pin de control 2 del Motor 2 (L293D Pin 15)

// Pines de Habilitación (ENABLEs) - Para el ejercicio 2, se mantienen en HIGH para usar solo 4 señales de dirección.
const int EN1 = 11; // Pin Enable Motor 1 (L293D Pin 1)
const int EN2 = 10; // Pin Enable Motor 2 (L293D Pin 9)
String option;
const int C1M1 = 3;
const int C1M2 = 2;
void setup() {
  // Configurar los pines de control como salidas
  pinMode(IN1, OUTPUT);
  //pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  //pinMode(IN4, OUTPUT);
  pinMode(C1M1, INPUT);
  pinMode(C1M2, INPUT);
  // Configurar los pines de Enable como salidas
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);

  // Habilitar los motores al inicio (HIGH)
  // Esto permite que los motores se controlen con las 4 señales de dirección (IN1-IN4)
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, LOW);

  // Inicializar la comunicación serial para recibir comandos
  Serial.begin(9600);
  Serial.println("Control de Motores");
  Serial.println("Comandos: A1 on left/right | A1 off | A2 on left/right | A2 off");
}

void loop() {
  // Bucle para leer comandos de la interfaz serial de Arduino
  if (Serial.available() > 0) {
    String option = Serial.readStringUntil('\n');
    //command.trim(); // Eliminar espacios en blanco
    if (option.equals("avanza")){
    moveFront();
    }
    if (option.equals("retrocede")){
    moveBack();
    }
    if (option.equals("derecha")){
    turnRight();
    }
    if (option.equals("izquierda")){
    turnLeft();
    }if (option.equals("apagado")){
    digitalWrite(EN1, LOW);
    digitalWrite(EN2, LOW);
    }
  }
  Serial.println(analogRead(C1M1));
  Serial.println(analogRead(C1M2));
  delay(1000);
}

  //Function move front
void moveFront(){
    //Girar a la izquierda Motor 1
    digitalWrite(IN1, LOW);
    digitalWrite(EN1, HIGH);
    //Girar a la derecha Motor 2
    digitalWrite(IN3, LOW);
    digitalWrite(EN2, HIGH);
    Serial.println("Avanzando");
  }
  //Function move back
void moveBack(){
    //Girar a la derecha Motor 1
    digitalWrite(IN1, HIGH);
    digitalWrite(EN1, HIGH);
    //Girar a la izquierda Motor 2
    digitalWrite(IN3, HIGH); // Nota: Cambiar HIGH/LOW puede invertir el sentido 'left'/'right' real
    digitalWrite(EN2, HIGH);
    Serial.println("Retrocediendo");
  }
  //Function turn right
void turnRight(){
    //Girar a la derecha Motor 1
    digitalWrite(IN1, HIGH);
    digitalWrite(EN1, HIGH);
    //Girar a la derecha Motor 2
    digitalWrite(IN3, LOW);
    digitalWrite(EN2, HIGH);
    Serial.println("Giro a la derecha");
  }
void turnLeft(){
    //Girar a la izquierda Motor 1
    digitalWrite(IN1, LOW);
    digitalWrite(EN1, HIGH);
    //Girar a la izquierda Motor 2
    digitalWrite(IN3, HIGH); // Nota: Cambiar HIGH/LOW puede invertir el sentido 'left'/'right' real
    digitalWrite(EN2, HIGH);
    Serial.println("Giro a la derecha");
  }
