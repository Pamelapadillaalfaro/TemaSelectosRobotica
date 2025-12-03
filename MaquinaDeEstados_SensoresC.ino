/*******************************************************************
* * * *Algoritmo de Evasión de Obstaculos
* Programa en C de Arduino para un robot móvil que evade obstáculos.
* * *Lab. Bio-Robótica * * FI-UNAM * * *
********************************************************************/
// Definicion de constantes
#define ADELANTE move_robot(AVANCE, 0.0f)
#define ATRAS move_robot(-AVANCE, 0.0)
#define GIRO_IZQ move_robot(0.0, GIRO)
#define GIRO_DER move_robot(0.0, -GIRO)
#define ALTO move_robot(0.0,0.0)

const int IN1 = 13; // Pin de control 1 del Motor 1 (L293D Pin 2)
const int IN3 = 12;     // Pin de control 1 del Motor 2 (L293D Pin 10)
// Pines de Habilitación (ENABLEs) - Para el ejercicio 2, se mantienen en HIGH para usar solo 4 señales de dirección.
const int EN1 = 6;     // Pin Enable Motor 1 (L293D Pin 1)
const int EN2 = 5;     // Pin Enable Motor 2 (L293D Pin 9)
// Pines encoder
const int ENC1 = 2;
const int ENC2 = 3;
// Pines sensores
const int light1 = A0;
const int light3 = A1;
const int light2 = A2;
const int infra1 = A3;
const int infra2 = A4;
const int infra3 = A5;
const int contact1 =7;
const int contact2 =4;


const int speedT = 150; //Turn speed
int PWM1 = 180;         //PWM motor 1
int PWM2 = 190;         //PWM motor 2

volatile long count1 = 0;
volatile long count2 = 0;

void ISR_encoder1() { count1++; }
void ISR_encoder2() { count2++; }


// Configuración de los pines de entrada y salida
void setup() {
  int i;
  Serial.begin(9600);
  // Inicializa comunicación serial
  pinMode(IN1, OUTPUT);
  pinMode(IN3, OUTPUT);
  // Configurar los pines de Enable como salidas
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  // C0nfigurar pines de sensores como entradas
  pinMode(infra1, INPUT);
  pinMode(infra2, INPUT);
  pinMode(infra3, INPUT);
  pinMode(contact1, INPUT);
  pinMode(contact2, INPUT);
  pinMode(ENC1, INPUT_PULLUP);
  pinMode(ENC2, INPUT_PULLUP);
  // Confugure Encoder Pins as input

  // Set up default all motors ON
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);

  attachInterrupt(digitalPinToInterrupt(ENC1), ISR_encoder1, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC2), ISR_encoder2, RISING);

  // Inicializar la comunicación serial para recibir comandos
  Serial.begin(9600);
}


// Esta función hace que el robot primero gire un angulo y después avance una distancia
void move_robot(float Distance, float Degree){      
  if (Degree != 0) girar(Degree);
  if (Distance != 0) avanzar(Distance);
}

// ---------------- FUNCIONES AVANZAR Y GIRAR ----------------

void avanzar(float cm) {
  long target = (abs(cm) * 53);  // 53 pulsos por cm
  count1 = count2 = 0;

  if (cm > 0) moveFront();
  else moveBack();

  while ((count1 < target) and (count2 < target)){
    // Espera a que se cumpla el número de pulsos
    Serial.println("Estamos en el While de avance");
  }
  stopMotors();
}

void girar(float grados) {
  long target = abs(grados) * 5.8; // 5.5 pulsos por grado
  count1 = count2 = 0;

  if (grados > 0) turnLeft();
  else turnRight();

  while ((count1 < target) and (count2 < target)) {
    // Espera a que se cumpla el número de pulsos
    Serial.println("Estamos en el While de giro");
  }
  stopMotors();
}

float shs(const char *sensor, int num_sensor) {
  float x = -1.0; // valor por defecto si algo sale mal
  
  // ---------- SENSORES INFRARROJOS ----------
  if (strcmp(sensor, "infra") == 0) {
    int pin = -1;
    switch (num_sensor) {
      case 1: pin = infra3; break;
      case 2: pin = infra2; break;
      case 3: pin = infra1; break;
      default: 
        Serial.println("Error: número de sensor infra inválido");
        return -1.0;
    }
    int raw = analogRead(pin);
    x = map(raw, 0, 1023, 0, 255);
  }

  // ---------- SENSORES DE CONTACTO ----------
  else if (strcmp(sensor, "contact") == 0) {
    int pin = -1;
    switch (num_sensor) {
      case 1: pin = contact1; break;
      case 2: pin = contact2; break;
      default:
        Serial.println("Error: número de sensor contact inválido");
        return -1.0;
    }
    x = digitalRead(pin); // 0 o 1
  }

  // ---------- SENSORES DE LUZ ----------
  else if (strcmp(sensor, "light") == 0) {
    int pin = -1;
    switch (num_sensor) {
      case 1: pin = light1; break;
      case 2: pin = light2; break;
      case 3: pin = light3; break;
      default:
        Serial.println("Error: número de sensor contact inválido");
        return -1.0;
    }
    x = digitalRead(pin); // 0 o 1
  }
  
  // ---------- SENSOR DESCONOCIDO ----------
  else {
    Serial.print("Error: tipo de sensor desconocido -> ");
    Serial.println(sensor);
  }

  return x;
}


// Algoritmo de evasión de obstáculos

void loop(){
  int estado = 0;
  float Si,Sd, Sm,Ci,Cd,Li,Ld,La; // Sensores infrarojo, de contancto y de luz
  float AVANCE = 1;
  float GIRO = 45;
  // Estado inicial
  estado = 0;
  // Loop infinito
  
  while(1){
    // Se leen los sensores
    Si = shs("infra",1);
    Sm = shs("infra",2);
    Sd = shs("infra",3);
    Ci = shs("contact",1);
    Cd = shs("contact",2);
    Li = shs("light",1);
    Ld = shs("light",2);
    La = shs("light",3);
    Serial.print("Estado Presente: ");
    Serial.println(estado);
    Serial.print("Ci: ");
    Serial.println(Ci);
    Serial.print("Cd: ");
    Serial.println(Cd);
    Serial.print("Contador Enc1: ");
    Serial.println(count1);
   
    switch (estado){
      case 0: // est0
        if (Ci == 1){
          estado = 3;
          ALTO;
          ATRAS;
        }
        if (Cd == 1){
          estado = 1;
          ALTO;
          ATRAS;
        }
        if (Sm > 100){
          estado = 5;
          ALTO;
        }      
        if (Si < 100){
          if (Sd < 100){
            estado = 0;
            ADELANTE;
            Serial.println("ADELANTE");
          }
          else{
            estado = 1;
            ALTO;
            Serial.println("ALTO");
          }
        }
        else{
          if (Sd < 100){
            estado = 3;
            ALTO;
            Serial.println("ALTO");
          }
          else{
            estado = 5;
            ALTO;
            Serial.println("ALTO");
          }
        }
        break;

      case 1:// est1
        estado = 2;
        ATRAS;
        Serial.println("ATRAS");
        break;
      case 2:// est2
        estado = 0;
        GIRO_IZQ;
        Serial.println("GIRO_IZQ");
        break;
      case 3:// est3
        estado = 4;
        ATRAS;
        Serial.println("ATRAS");
        break;
      case 4:// est4
        estado = 0;
        GIRO_DER;
        Serial.println("GIRO_DER");
        break;
      case 5:// est5
        estado = 6;
        ATRAS;
        Serial.println("ATRAS");
        break;
      case 6:// est6
        estado = 7;
        GIRO_IZQ;
        Serial.println("GIRO_IZQ");
        break;
      case 7:// est7
        estado = 8;
        GIRO_IZQ;
        Serial.println("GIRO_IZQ");
        break;
      case 8:// est8
        estado = 9;
        ADELANTE;
        Serial.println("ADELANTE");
        break;
      case 9:// est9
        estado = 10;
        ADELANTE;
        Serial.println("ADELANTE");
        break;
      case 10:// est10
        estado = 11;
        GIRO_DER;
        Serial.println("GIRO_DER");
        break;
      case 11:// est11
        estado = 0;
        GIRO_DER;
        Serial.println("GIRO_DER");
        break;
      } // end case
  } // end while
} // end Main (loop)



// ----- control de motores -----
void moveFront() {
  digitalWrite(IN1, LOW);
  analogWrite(EN1, PWM1);
  digitalWrite(IN3, LOW);
  analogWrite(EN2, PWM2);
  Serial.println("Avanzando...");
}

void moveBack() {
  digitalWrite(IN1, HIGH);
  analogWrite(EN1, PWM1);
  digitalWrite(IN3, HIGH);
  analogWrite(EN2, PWM2);
  Serial.println("Retrocediendo...");
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  analogWrite(EN1, speedT);
  digitalWrite(IN3, LOW);
  analogWrite(EN2, speedT);
  Serial.println("Giro derecha...");
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  analogWrite(EN1, speedT);
  digitalWrite(IN3, HIGH);
  analogWrite(EN2, speedT);
  Serial.println("Giro izquierda...");
}

void stopMotors() {
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, LOW);
  Serial.println("Parado.");
}
