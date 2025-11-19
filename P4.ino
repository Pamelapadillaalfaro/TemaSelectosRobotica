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

const int infraInPin1 = A3;
const int infraInPin2 = A5;

const int speedT = 150; //Turn speed
String option;

int TimeDistance;
int TimeDegree;


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
  //
  pinMode(infraInPin1, INPUT);
  pinMode(infraInPin2, INPUT);
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

// Esta función hace que el robot primero gire un angulo y después avance una distancia
void move_robot(float distancia, float angulo){
  // ....
  delay(1000);
  }
  
// Esta función lee el valor de un sensor, indicando su tipo y numero, y regresa su valor
  float shs(char *sensor, int num_sensor){
  float x=1.0;
  // ....
  return(x);
}

// Algoritmo de evasión de obstáculos

void loop(){
  int estado = 0;
  float Si,Sd;
  float AVANCE=.1;
  float GIRO= 0.7854;
  // Estado inicial
  estado = 0;
  // Loop infinito
  
  while(1){
    // Se leen los sensores
    Si = shs("infra",1);
    Sd = shs("infra",2);
    Serial.print("Estado Presente: ");
    Serial.println(estado);
    Serial.print("Si: ");
    Serial.println(Si);
    Serial.print("Sd: ");
    Serial.println(Sd);
    switch (estado){
    case 0:
    // est0
    if (Si < 200){
      if (Sd < 200){
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
    if (Sd < 200){
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
