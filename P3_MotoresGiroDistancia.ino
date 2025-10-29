// Asignamos pines digitales del Arduino a las entradas de control (INPUTs)
const int IN1 = 13; // Pin de control 1 del Motor 1 (L293D Pin 2)
const int IN3 = 12;     // Pin de control 1 del Motor 2 (L293D Pin 10)

// Pines de Habilitación (ENABLEs) - Para el ejercicio 2, se mantienen en HIGH para usar solo 4 señales de dirección.
const int EN1 = 6;     // Pin Enable Motor 1 (L293D Pin 1)
const int EN2 = 5;     // Pin Enable Motor 2 (L293D Pin 9)

const int speedT = 150; //Turn speed
String option;

int TimeDistance;
int TimeDegree;

void setup() {
  // Configurar los pines de control como salidas
  pinMode(IN1, OUTPUT);
  pinMode(IN3, OUTPUT);
  // Configurar los pines de Enable como salidas
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  // Confugure Encoder Pins as input

  // Set up default all motors ON
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);

  // Inicializar la comunicación serial para recibir comandos
  Serial.begin(9600);
  Serial.println("Control de Motores");
  Serial.println("Comandos: MV 5 , -7");
}

void loop() {
  // Bucle para leer comandos de la interfaz serial de Arduino
  if (Serial.available() > 0) {
    String option = Serial.readStringUntil('\n');
      int Distance = 0;
      int Degree = 0;
    if (option.startsWith("MV")){
      // Stract substring after "MV"
      String valueStr1 = option.substring(option.indexOf("MV") + 2);
      Distance = valueStr1.toInt();
      // Stract substring after coma","
      String valueStr2 = option.substring(option.indexOf(",") + 1);
      Degree = valueStr2.toInt();
      }
     if (option.equals("detener")){stopMotors();}
     
      Serial.print("Degree: ");
      Serial.println(Degree);
      Serial.print("Distance: ");
      Serial.println(Distance);
      
      TimeDistance = Distance*25;
      TimeDegree = Degree*10;

      if (Degree < 0) {
        turnRight();
        delay(-TimeDegree);
        stopMotors();
        }
      else if (Degree > 0 ){
        turnLeft();
        delay(TimeDegree);
        stopMotors();
        }
      if (Distance > 0){
        moveFront();
        delay(TimeDistance);
        stopMotors();
        }
      else if (Distance < 0){
        moveBack();
        delay(-TimeDistance);
        stopMotors();
      }
  }
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
