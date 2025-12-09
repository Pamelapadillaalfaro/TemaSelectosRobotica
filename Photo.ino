// ----- Pines multiplexor 4067 -----
const int muxSIG = A0;
const int muxC0 = 8;
const int muxC1 = 9;
const int muxC2 = 10;
const int muxC3 = 11;



void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  // Multiplexor (como salidas)
  pinMode(muxC0, OUTPUT);
  pinMode(muxC1, OUTPUT);
  pinMode(muxC2, OUTPUT);
  pinMode(muxC3, OUTPUT);
  pinMode(muxSIG, INPUT);
  //
}

void selectMuxChannel(int ch) {
  digitalWrite(muxC0, ch & 0x01);
  digitalWrite(muxC1, (ch >> 1) & 0x01);
  digitalWrite(muxC2, (ch >> 2) & 0x01);
  digitalWrite(muxC3, (ch >> 3) & 0x01);
  delay(1); // estabilización
}
void loop() {
  // put your main code here, to run repeatedly:
  float x = -1.0;
  selectMuxChannel(0);
  x = map(analogRead(muxSIG), 0, 1023, 0, 255);
  Serial.print("L1: ");
  Serial.println(x);
  selectMuxChannel(1);
  x = map(analogRead(muxSIG), 0, 1023, 0, 255);
  Serial.print("L2: ");
  Serial.println(x);
  selectMuxChannel(2);
  x = map(analogRead(muxSIG), 0, 1023, 0, 255);
  Serial.print("L3: ");
  Serial.println(x);
  selectMuxChannel(3);
  x = map(analogRead(muxSIG), 0, 1023, 0, 255);
  Serial.print("L4: ");
  Serial.println(x);
  delay(100);

}
