const int speed_1Pin = A1;  // Analog OUTPUT
const int speed_2Pin = A2;  // Analog OUTPUT

int speedValue = 0;        
String option;              // Instruction string

void setup() {
  Serial.begin(9600);
  pinMode(speed_1Pin, OUTPUT);
  pinMode(speed_2Pin, OUTPUT);
}

void loop() {
  int speedPin;

  if (Serial.available() > 0) {
    option = Serial.readStringUntil('\n');

    if (option.indexOf("A1") >= 0) {
      speedPin = speed_1Pin;
      Serial.println("Motor A1");
    } else if (option.indexOf("A2") >= 0) {
      speedPin = speed_2Pin;
      Serial.println("Motor A2");
    } else {
      return; // No valid motor
    }

    speedValue = speedFunction(option);

    // Sacles  -127 a 127 -> 0 a 254 to PWM 
    int pwmValue = map(speedValue, -127, 127, 0, 254);
    analogWrite(speedPin, pwmValue);

    Serial.print("Speed original: ");
    Serial.println(speedValue);
    Serial.print("PWM aplicado: ");
    Serial.println(pwmValue);
  }
}

int speedFunction(String instruct) {
  // Find "speed" in instuction
  int posSpeed = instruct.indexOf("speed");
  if (posSpeed == -1) return 0;

  // Stract substrinf after "speed"
  String valueStr = instruct.substring(posSpeed + 6);
  valueStr.trim(); // Delate aditional spaces

  int speed = valueStr.toInt(); 
  return speed;
}
