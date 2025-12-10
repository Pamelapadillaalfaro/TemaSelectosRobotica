/*******************************************************************
* * * *Algoritmo de Evasión de Obstaculos
* Programa en C de Arduino para un robot móvil que evade obstáculos.
* * *Lab. Bio-Robótica * * FI-UNAM * * *
********************************************************************/
// =========================================================
//                CONFIGURACIÓN GENERAL
// =========================================================

// Macros de movimiento
#define ADELANTE   move_robot(AVANCE, 0.0f)
#define ATRAS      move_robot(-AVANCE, 0.0f)
#define GIRO_IZQ   move_robot(0.0f,  GIRO)
#define GIRO_DER   move_robot(0.0f, -GIRO)
#define ALTO       move_robot(0.0f, 0.0f)

// ----- Pines motores (L293D) -----
const int IN1 = 13;   // Motor 1 dirección
const int IN3 = 12;   // Motor 2 dirección
const int EN1 = 6;    // PWM Motor 1
const int EN2 = 5;    // PWM Motor 2

// ----- Pines encoder -----
const int ENC1 = 2;
const int ENC2 = 3;

// ----- Pines sensores directos -----
const int infra1   = A3;
const int infra2   = A4;
const int infra3   = A5;
const int contact1 = 7;
const int contact2 = 4;

// ----- Pines multiplexor 4067 -----
const int muxSIG = A0;
const int muxC0 = 8;
const int muxC1 = 9;
const int muxC2 = 10;
const int muxC3 = 11;

// ----- Parámetros de movimiento -----
const int speedT = 150;  // Velocidad para giros
int PWM1 = 180;
int PWM2 = 190;

// ----- Variables encoder -----
volatile long count1 = 0;
volatile long count2 = 0;


// =========================================================
//                INTERRUPTS ENCODERS
// =========================================================
void ISR_encoder1() { count1++; }
void ISR_encoder2() { count2++; }


// =========================================================
//                CONFIGURACIÓN (SETUP)
// =========================================================
void setup() {
  Serial.begin(9600);

  // Motores
  pinMode(IN1, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);

  // Sensores
  pinMode(infra1, INPUT);
  pinMode(infra2, INPUT);
  pinMode(infra3, INPUT);
  pinMode(contact1, INPUT);
  pinMode(contact2, INPUT);

  // Encoders
  pinMode(ENC1, INPUT_PULLUP);
  pinMode(ENC2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC1), ISR_encoder1, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC2), ISR_encoder2, RISING);

  // Multiplexor (como salidas)
  pinMode(muxC0, OUTPUT);
  pinMode(muxC1, OUTPUT);
  pinMode(muxC2, OUTPUT);
  pinMode(muxC3, OUTPUT);
  pinMode(muxSIG, INPUT);

  // Estado inicial: motores apagados
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
}


// =========================================================
//                MOVIMIENTO DE ALTO NIVEL
// =========================================================
void move_robot(float Distance, float Degree) {
  if (Degree != 0) girar(Degree);
  if (Distance != 0) avanzar(Distance);
}


// =========================================================
//              AVANZAR Y GIRAR (con encoders)
// =========================================================

// Avance lineal basado en pulsos (53 pulsos/cm)
void avanzar(float cm) {
  long target = abs(cm) * 53;
  count1 = count2 = 0;

  if (cm > 0) moveFront();
  else        moveBack();

  while (count1 < target && count2 < target) {
    Serial.println("Estamos en el While de avance");
  }

  stopMotors();
}

// Giro en grados (5.8 pulsos/grado)
void girar(float grados) {
  long target = abs(grados) * 5.8;
  count1 = count2 = 0;

  if (grados > 0) turnLeft();
  else            turnRight();

  while (count1 < target && count2 < target) {
    Serial.println("Estamos en el While de giro");
  }

  stopMotors();
}


// =========================================================
//                     FUNCIÓN SHS()
// =========================================================
// Lee sensores:
//  "infra"   -> infrarrojos directos (A3, A4, A5)
//  "contact" -> contacto (digitales)
//  "light"   -> sensores por multiplexor 4067
// =========================================================
float shs(const char *sensor, int num_sensor) {
  float x = -1.0;

  // ---------------- INFRA ----------------
  if (strcmp(sensor, "infra") == 0) {
    int pin = -1;
    if      (num_sensor == 1) pin = infra3;
    else if (num_sensor == 2) pin = infra2;
    else if (num_sensor == 3) pin = infra1;
    else {
      Serial.println("Error: número de sensor infra inválido");
      return -1.0;
    }

    x = map(analogRead(pin), 0, 1023, 0, 255);
  }

  // ---------------- CONTACT ----------------
  else if (strcmp(sensor, "contact") == 0) {
    int pin = (num_sensor == 1) ? contact1 :
              (num_sensor == 2) ? contact2 : -1;

    if (pin == -1) {
      Serial.println("Error: número de sensor contact inválido");
      return -1.0;
    }

    x = digitalRead(pin);
  }

  // ---------------- LIGHT (MUX 4067) ----------------
  else if (strcmp(sensor, "light") == 0) {
    int channel = (num_sensor >= 1 && num_sensor <= 4) ? num_sensor - 1 : -1;

    if (channel == -1) {
      Serial.println("Error: número de sensor light inválido");
      return -1.0;
    }

    selectMuxChannel(channel);
    x = map(analogRead(muxSIG), 0, 1023, 0, 255);
  }

  // ---------------- Desconocido ----------------
  else {
    Serial.print("Error: tipo de sensor desconocido -> ");
    Serial.println(sensor);
  }

  return x;
}


// =========================================================
//                 SELECCIÓN DE CANAL MUX
// =========================================================
void selectMuxChannel(int ch) {
  digitalWrite(muxC0, ch & 0x01);
  digitalWrite(muxC1, (ch >> 1) & 0x01);
  digitalWrite(muxC2, (ch >> 2) & 0x01);
  digitalWrite(muxC3, (ch >> 3) & 0x01);
  delayMicroseconds(5); // estabilización
}


// =========================================================
//                 ALGORITMO DE ESTADOS (FSM)
// =========================================================
void loop() {

  float Si, Sd, Sm, Ci, Cd, Li, Ld, Lai, Lad;
  float AVANCE = 1;
  float GIRO   = random(30,90);
  int estado = 0;

  while (1) {

    // ---- Lectura de sensores ----
    Si  = shs("infra",1);
    Sm  = shs("infra",2);
    Sd  = shs("infra",3);
    Ci  = shs("contact",1);
    Cd  = shs("contact",2);
    Li  = shs("light",1);
    Ld  = shs("light",2);
    Lai = shs("light",3);
    Lad = shs("light",4);

    Serial.print("Estado Presente: "); Serial.println(estado);

    // ---- FSM ----
    switch (estado) {

      case 0:
        if (Ci == 1) { estado = 3; ALTO; ATRAS; break; }
        if (Cd == 1) { estado = 1; ALTO; ATRAS; break; }
        if (Sm > 90) { estado = 5; ALTO; break; }

        if (Si < 130) {
          if (Sd < 130) { estado = 12; ADELANTE; }
          else          { estado = 1; ALTO; }
        } else {
          if (Sd < 130) { estado = 3; ALTO; }
          else          { estado = 5; ALTO; }
        }

        if (Si == 200 && Sd == 200){
          ALTO;
        }
        break;

      case 1: estado = 2; ATRAS; ATRAS; break;
      case 2: estado = 0; GIRO_IZQ; break;
      case 3: estado = 4; ATRAS; ATRAS; break;
      case 4: estado = 0; GIRO_DER; break;
      case 5: estado = 6; ATRAS; break;
      case 6: estado = 7; GIRO_IZQ; break;
      case 7: estado = 8; GIRO_IZQ; break;
      case 8: estado = 9; ADELANTE; break;
      case 9: estado = 10; ADELANTE; break;
      case 10: estado = 11; GIRO_DER; break;
      case 11: estado = 0; GIRO_DER; break;

      case 12:
        //if(Li+Ld > Lai+Lad){
          //estado = 0; ADELANTE;
        //}
        if (Li  > Ld ) {
          //dest = 3;
          GIRO   = 30;
          estado = 0; GIRO_DER;
        } 
        if (Li  > Ld ) {
          //dest = 3;
          GIRO   = 30;
          estado = 0; GIRO_IZQ;
        } 
        if (Li  > Lai && Li > Lad ) {
          //dest = 3;
          estado = 0; ADELANTE;
        } 
        if (Ld  > Lai && Ld > Lad ) {
          //dest = 2;
          //delay(800);
          estado = 0; ADELANTE;
        }
        if (Lai  > Ld && Lai > Lad && Lai > Li) {
          //dest = 1;
          //delay(1000);
          estado = 0; GIRO_IZQ ; GIRO_IZQ;
        } 
        if (Lad  > Ld && Lad > Lai && Lad > Li) {
          //dest = 0;
          //delay(1000);
          estado = 0; GIRO_DER; GIRO_DER;
        } 
        
        break;
    }
  }
}


// =========================================================
//                     CONTROL DE MOTORES
// =========================================================
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
