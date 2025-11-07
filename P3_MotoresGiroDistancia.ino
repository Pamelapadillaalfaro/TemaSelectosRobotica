// Asignamos pines digitales del Arduino a las entradas de control (INPUTs)
const int IN1 = 13; // Pin de control 1 del Motor 1 (L293D Pin 2)
const int IN3 = 12;     // Pin de control 1 del Motor 2 (L293D Pin 10)

// Pines de Habilitación (ENABLEs) - Para el ejercicio 2, se mantienen en HIGH para usar solo 4 señales de dirección.
const int EN1 = 25;     // Pin Enable Motor 1 (L293D Pin 1)
const int EN2 = 26;     // Pin Enable Motor 2 (L293D Pin 9)

String option;

int TimeDistance;
int TimeDegree;

//PWM constnts configuration
const int freq = 5000;       // Frecuencia PWM
const int resolution = 8;    // 8 bits (0-255)
const int channel1 = 0;      // Canal PWM Motor 1
const int channel2 = 1;      // Canal PWM Motor 2
const int speedT = 150;      // PWM Speed value


void setup() {
  // Configurar los pines de control como salidas
  pinMode(IN1, OUTPUT);
  pinMode(IN3, OUTPUT);
  // Configurar los pines de Enable como salidas
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  // Confugure Encoder Pins as input

// Configurar canales PWM
  ledcSetup(channel1, freq, resolution);
  ledcSetup(channel2, freq, resolution);

  // Asociar pines a canales PWM
  ledcAttachPin(EN1, channel1);
  ledcAttachPin(EN2, channel2);

  // Estado inicial motores apagados
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  ledcWrite(channel1, 0);
  ledcWrite(channel2, 0);

  // Inicializar la comunicación serial para recibir comandos
  Serial.begin(115200);
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


void moveFront() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  ledcWrite(channel1, 255);
  ledcWrite(channel2, 255);
  Serial.println("Avanzando");
}

void moveBack() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, HIGH);
  ledcWrite(channel1, 255);
  ledcWrite(channel2, 255);
  Serial.println("Retrocediendo");
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN3, LOW);
  ledcWrite(channel1, speedT);
  ledcWrite(channel2, speedT);
  Serial.println("Giro a la derecha");
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, HIGH);
  ledcWrite(channel1, speedT);
  ledcWrite(channel2, speedT);
  Serial.println("Giro a la izquierda");
}

void stopMotors() {
  ledcWrite(channel1, 0);
  ledcWrite(channel2, 0);
  Serial.println("Motores detenidos");
}