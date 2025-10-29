// Asignamos pines digitales del Arduino a las entradas de control (INPUTs)
const int IN1 = 13; // Pin de control 1 del Motor 1 (L293D Pin 2)
const int IN3 = 12;     // Pin de control 1 del Motor 2 (L293D Pin 10)

// Pines de Habilitación (ENABLEs) - Para el ejercicio 2, se mantienen en HIGH para usar solo 4 señales de dirección.
const int EN1 = 6;     // Pin Enable Motor 1 (L293D Pin 1)
const int EN2 = 5;     // Pin Enable Motor 2 (L293D Pin 9)

// Pines de Encoders
const int C1M1 = 2;
const int C1M2 = 3;

// Contadores de pulsos
volatile long pulsesM1 = 0;
volatile long pulsesM2 = 0;

// Variables para RPM y posición
unsigned long lastTime = 0;
float rpmM1 = 0, rpmM2 = 0;
float vueltasM1 = 0, vueltasM2 = 0;
float gradosM1 = 0, gradosM2 = 0;

// CONFIGURA según tu motor:
const int pulsesPerRevolution = 700; // 11 pulsos * 30 relación = 330

const int speedT = 170; //Turn speed
String option;


void setup() {
  // Configurar los pines de control como salidas
  pinMode(IN1, OUTPUT);
  pinMode(IN3, OUTPUT);
  // Configurar los pines de Enable como salidas
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  // Confugure Encoder Pins as input
  pinMode(C1M1, INPUT_PULLUP);
  pinMode(C1M2, INPUT_PULLUP);

  // Interrupciones para leer los encoders
  attachInterrupt(digitalPinToInterrupt(C1M1), encoderM1ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(C1M2), encoderM2ISR, RISING);
  
  // Ser up default all motors ON
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);

  // Inicializar la comunicación serial para recibir comandos
  Serial.begin(9600);
  Serial.println("Control de Motores");
  Serial.println("avanza | retrocede | derecha | izquierda | apagar");
}

void loop() {
  // Bucle para leer comandos de la interfaz serial de Arduino
  if (Serial.available() > 0) {
    String option = Serial.readStringUntil('\n');
    //command.trim(); // Eliminar espacios en blanco
    if (option.equals("avanza")){ moveFront();}
    if (option.equals("retrocede")){ moveBack();}
    if (option.equals("derecha")){turnRight();}
    if (option.equals("izquierda")){turnLeft();}
    if (option.equals("detener")){stopMotors();}
  }
  
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


//Interruptin functionsó
void encoderM1ISR() {
  pulsesM1++;
}

void encoderM2ISR() {
  pulsesM2++;
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
    analogWrite(EN1, speedT);
    //Girar a la derecha Motor 2
    digitalWrite(IN3, LOW);
    analogWrite(EN2, speedT);
    Serial.println("Giro a la derecha");
  }
void turnLeft(){
    //Girar a la izquierda Motor 1
    digitalWrite(IN1, LOW);
    analogWrite(EN1, speedT);
    //Girar a la izquierda Motor 2
    digitalWrite(IN3, HIGH); // Nota: Cambiar HIGH/LOW puede invertir el sentido 'left'/'right' real
    analogWrite(EN2, speedT);
    Serial.println("Giro a la izquierda");
  }
 void stopMotors(){
    digitalWrite(EN1, LOW);
    digitalWrite(EN2, LOW);
  }
