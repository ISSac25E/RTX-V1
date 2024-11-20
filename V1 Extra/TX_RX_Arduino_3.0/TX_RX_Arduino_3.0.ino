
//TX

void setup() {
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  delay(1000);
}

void loop() {
  Send();
}
void Send(){
  digitalWrite(10, HIGH);
  delayMicroseconds(50);
  digitalWrite(10, LOW);
  delayMicroseconds(50);
  digitalWrite(10, HIGH);
  delayMicroseconds(50);
  digitalWrite(10, LOW);
  delayMicroseconds(25);
  digitalWrite(10, HIGH);
  delayMicroseconds(50);
  delayMicroseconds(50);
  digitalWrite(10, LOW);
  delayMicroseconds(50);
  digitalWrite(10, HIGH);
  delayMicroseconds(50);
  delayMicroseconds(50);
  digitalWrite(10, LOW);
  delay(1000);
   digitalWrite(10, HIGH);
  delayMicroseconds(50);
  digitalWrite(10, LOW);
  delayMicroseconds(50);
  digitalWrite(10, HIGH);
  delayMicroseconds(50);
  digitalWrite(10, LOW);
  delayMicroseconds(25);
  digitalWrite(10, HIGH);
  delayMicroseconds(50);
  digitalWrite(10, LOW);
  delayMicroseconds(50);
  digitalWrite(10, HIGH);
  delayMicroseconds(50);
  delayMicroseconds(50);
  digitalWrite(10, LOW);
  delayMicroseconds(50);
  delay(1000);
  digitalWrite(10, HIGH);
  delayMicroseconds(50);
  digitalWrite(10, LOW);
  delayMicroseconds(50);
  digitalWrite(10, HIGH);
  delayMicroseconds(50);
  digitalWrite(10, LOW);
  delayMicroseconds(25);
  delayMicroseconds(50);
  delayMicroseconds(50);
  digitalWrite(10, HIGH);
  delayMicroseconds(50);
  digitalWrite(10, LOW);
  delayMicroseconds(50);
  delayMicroseconds(50);
  delayMicroseconds(1000);
}


////RX
//unsigned long int M1 = 0;
//unsigned long int M2 = 0;
//unsigned long int M3 = 0;
//unsigned long int M4 = 0;
//int X = 0;
//void setup() {
//  Serial.begin(2000000);
//  pinMode(10, INPUT);
//  delay(500);
//}
//
//void loop() {
//  if (digitalRead(10) == HIGH) {
//    Clock();
//  }
//}
//void Clock() {
//  while (digitalRead(10) == HIGH) {
//  }
//  while (digitalRead(10) == LOW) {
//  }
//  M1 = micros();
//  while (digitalRead(10) == HIGH) {
//  }
//  M2 = micros();
//  M4 = M2;
//  M3 = (M2 -= M1);
//  Read();
//}
//void Read() {
//
//  while (X < 8) {
//    M2 = M4;
//    if (micros() >= (M2 += M3)) {
//      M4 = micros();
//      if (digitalRead(10) == HIGH) {
//        Serial.print("1");
//      }
//      else {
//        Serial.print("0");
//      }
//      X++;
//    }
//  }
//  X = 0;
//  Serial.println();
//}
