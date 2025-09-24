const int comparadorPin=2;
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int infraInPin = A1;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to
const int contactPin =3;
const int tempPin =A2;
const int photoDiPin =7;
const int pinPWM=10;


int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int valorPWM;
String option;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(comparadorPin, INPUT);
  //Pin para el PWM
  pinMode(pinPWM, OUTPUT);
  pinMode(photoDiPin, INPUT);
}

void loop() {

  // Leer puerto serial
  //si existe datos disponibles los leemos
  if (Serial.available()>0){
    //leemos la opcion enviada
    option=Serial.readStringUntil('\n');
    if(option.equals("shs photora")) {

      // read the analog in value:
      sensorValue = analogRead(analogInPin);
      // map it to the range of the analog out:
      outputValue = map(sensorValue, 0, 1023, 0, 255);
      // change the analog out value:
      analogWrite(analogOutPin, outputValue);
      Serial.println(sensorValue);
    }
    //Sensor infrarrojo
    else if(option.equals("shs infrared")) {
      //Serial.print("Valor = ");
      // put your main code here, to run repeatedly:
      // read the analog in value:
      sensorValue = analogRead(infraInPin);
      // map it to the range of the analog out:
      outputValue = map(sensorValue, 0, 1023, 0, 255);
      // change the analog out value:
      Serial.println(sensorValue);
    }
    //Contacto
    else if(option.equals("shs contact")) {
      //Serial.print("Valor = ");
      // put your main code here, to run repeatedly:
      // read the analog in value:
      sensorValue = digitalRead(contactPin);
      // map it to the range of the analog out:
      // change the analog out value:
      Serial.println(sensorValue);
    }
    //Sensor de temperatura
    else if(option.equals("shs temp")) {
      //Serial.print("Valor = ");
      // put your main code here, to run repeatedly:
      // read the analog in value:
      sensorValue = analogRead(tempPin);
      float temperatura=(5.0*sensorValue*100.0)/1024.0;
      // map it to the range of the analog out:
      // change the analog out value:
      Serial.println(temperatura);
    }
    //Contacto
    else if(option.equals("shs photord")) {
      analogWrite(pinPWM, valorPWM);
      sensorValue = digitalRead(photoDiPin);
      Serial.println(sensorValue);
    }
    else if(option.equals("shs calibrator")) {
      //Serial.print("Calibrabdo");
      valorPWM = funcionCalibrar();
      Serial.println("Finish Calibration");
      }
    else{
     Serial.println("Comando no disponible");
    }
  }
  delay(1000);
}

int funcionCalibrar(){

  int cicloTrab=0;
  int incremento=5;
  int photoDiPin=7;
  int sensorvalue=0;
  int contador=0;
  int cicloFijo=0;

  // put your main code here, to run repeatedly
  while (contador<=1) {
  analogWrite(pinPWM, cicloTrab);
  cicloTrab=cicloTrab+incremento;
  
  if (cicloTrab <= 0 || cicloTrab>=255){
    incremento=-incremento;
    }
//  Serial.print("CICLO DE TRABAJO: ");
//  Serial.println(cicloTrab);
  sensorvalue=digitalRead(photoDiPin);
  if (sensorvalue==0){
    contador=contador+1;
    }
  delay(300);
  }cicloFijo=cicloTrab-5;
  
//  Serial.print("CICLO DE TRABAJO FINAL: ");
//  Serial.println(cicloFijo);
//  Serial.print("VALOR COMPARADO: ");

  return cicloFijo;
}
