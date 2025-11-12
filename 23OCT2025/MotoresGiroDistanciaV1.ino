// Pines motores
const int IN1 = 13;
const int IN3 = 12;
const int EN1 = 6;
const int EN2 = 5;

// Pines encoder
const int ENC1 = 2;
const int ENC2 = 3;

const int speedT = 150; //Turn speed
int PWM1 = 180;         //PWM motor 1
int PWM2 = 180;         //PWM motor 2

volatile long count1 = 0;
volatile long count2 = 0;

void ISR_encoder1() { count1++; }
void ISR_encoder2() { count2++; }

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(ENC1, INPUT_PULLUP);
  pinMode(ENC2, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(ENC1), ISR_encoder1, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC2), ISR_encoder2, RISING);

  Serial.begin(9600);
  Serial.println("Listo para comandos: MV 10,90 o detener");
}

void loop() {
  if (Serial.available()) {
    String option = Serial.readStringUntil('\n');
    option.trim();

    if (option.startsWith("MV")) {
      int coma = option.indexOf(',');
      float distancia = option.substring(2, coma).toFloat();
      float grados = option.substring(coma + 1).toFloat();

      Serial.print("Distancia (cm): "); Serial.println(distancia);
      Serial.print("Grados: "); Serial.println(grados);

      if (grados != 0) girar(grados);
      if (distancia != 0) avanzar(distancia);
    } 
    else if (option.equals("detener")) {
      stopMotors();
    }else if (option.equals("calibrar")) {
      calibratePWM();
    }
  }
}

// ---------------- FUNCIONES ----------------

void avanzar(float cm) {
  long target = (abs(cm) * 53);  // 53 pulsos por cm
  count1 = count2 = 0;

  if (cm > 0) moveFront();
  else moveBack();

  while ((count1 < target) && (count1 < target)){
    // Espera a que se cumpla el número de pulsos
  }
  stopMotors();
}

void girar(float grados) {
  long target = abs(grados) * 5.5; // 5.5 pulsos por grado
  count1 = count2 = 0;

  if (grados > 0) turnLeft();
  else turnRight();

  while ((count1 < target) && (count1 < target)) {
    // Espera a que se cumpla el número de pulsos
  }
  stopMotors();
}

//-------------Calibrar PWM motores--------------

void calibratePWM(){
  do{
    count1 = count2 = 0;
    moveFront();
    delay(1000);
    int dif = abs(count1 - count2) / 2;
    if (count1 < count2){
        PWM1 = PWM1 - dif;
        PWM2 = PWM2 + dif;
    }else if (count1 > count2){
        PWM1 = PWM1 + dif;
        PWM2 = PWM2 - dif;
    }
  }while(abs(count1 - count2) < 2);
  }

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
