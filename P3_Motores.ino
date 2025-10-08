//const int analogInPin = A0;  // Analog input pin
const int speed_1Pin = 2;
const int speed_2Pin = 4;
const int direction_1Pin = 7; // Digital output pin
const int direction_2Pin = 8; // Digital output pin

int speedValue = 0;        
int directionValue = 0;
String option;              // instructioón value

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  // pinMode
  pinMode(speed_1Pin, OUTPUT);
  pinMode(direction_1Pin, OUTPUT);
  pinMode(speed_2Pin, OUTPUT);
  pinMode(direction_2Pin, OUTPUT);
}

void loop() {
 int directionPin;
 int speedPin;
  if (Serial.available()>0){
    // Read instruction ...
    option=Serial.readStringUntil('\n');
    //int posicion = cadena.indexOf(aBuscar) --- Find index position un String
    if (option.indexOf("A1") >= 0) { //Motor A1 instruction
      directionPin = direction_1Pin;
      speedPin = speed_1Pin;
      Serial.println("Motor A1");
    }
    else if(option.indexOf("A2") >= 0) { //Motor A2 instruction
      directionPin = direction_2Pin;
      speedPin = speed_2Pin;
      Serial.println("Motor A2");
    }

    speedValue = on_offFunction(option);
    directionValue = turnFunction(option);
      
    digitalWrite(directionPin, directionValue);
    digitalWrite(speedPin, speedValue);

    Serial.print("Direction:");
    Serial.println(directionValue);
    
    Serial.print("ON / OFF:");
    Serial.println(speedValue);
    delay (2);
  }     
}

int turnFunction(String instruct){
    
    if (instruct.indexOf("left") >= 0 ) {
      return 0;
    }
    else if(instruct.indexOf("rigth") >= 0 ) {
      return 1;
    }
  }
  
int on_offFunction(String instruct){
    
    if (instruct.indexOf("on") >= 0 ) {
      return 1;
    }
    else if(instruct.indexOf("off") >= 0 ) {
      return 0;
    }
  } 
