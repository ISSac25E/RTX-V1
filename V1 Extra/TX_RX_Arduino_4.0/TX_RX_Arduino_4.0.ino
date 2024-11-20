/*
   int DA = 10;
  int CL = 11;
  void setup() {
  pinMode(DA, OUTPUT);
  pinMode(CL, OUTPUT);
  delay(1000);
  digitalWrite(DA, HIGH);
  delay(200);
  digitalWrite(CL, HIGH);
  delay(200);
  digitalWrite(DA, LOW);
  digitalWrite(CL, LOW);
  delay(200);
  digitalWrite(CL, HIGH);
  delay(200);
  digitalWrite(CL, LOW);
  digitalWrite(DA, HIGH);
  delay(200);
  digitalWrite(CL, HIGH);
  delay(200);
  digitalWrite(CL, LOW);
  delay(200);
  digitalWrite(CL, HIGH);
  delay(200);
  digitalWrite(CL, LOW);
  digitalWrite(DA, LOW);
  delay(200);
  digitalWrite(CL, HIGH);
  delay(200);
  digitalWrite(CL, LOW);
  delay(200);
  digitalWrite(CL, HIGH);
  delay(200);
  digitalWrite(CL, LOW);


  }

  void loop() {


  }
*/
/*



  //TX

  int X = 0;
  unsigned long int M1 = 0;

  boolean state = false;

  boolean Pulse1 = false;
  boolean PulseEn = false;


  boolean Code[][6] = {
  {true, false, false, false, false, false}, //0
  {true, true, false, false, false, false}, //1
  {true, false, true, false, false, false}, //2
  {true, true, true, false, false, false}, //3
  {true, false, false, true, false, false}, //4
  {true, true, false, true, false, false}, //5
  {true, false, true, true, false, false}, //6
  {true, true, true, true, false, false}, //7
  {true, false, false, false, true, false}, //8
  {true, true, false, false, true, false}, //9
  {true, false, true, false, true, false}, //10
  {true, true, true, false, true, false}, //11
  {true, false, false, true, true, false}, //12
  {true, true, false, true, true, false}, //13
  {true, false, true, true, true, false}, //14
  {true, true, true, true, true, false}, //15
  };


  void setup() {
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(2000000);
  delay(1000);
  }

  void loop() {
  if (millis() >= M1 and (PulseEn == HIGH or Pulse1 == HIGH)) {
    Pulse1 = Pulse();
  }
  if (Pulse1 == LOW) {
    WriteData();
  }

  }
  boolean Pulse() {
  if (Pulse1 == LOW) {
    digitalWrite(11, HIGH);
  }
  else {
    digitalWrite(11, LOW);
  }
  M1 = millis();
  M1 += 200;

  return digitalRead(11);
  }

  void WriteData() {
  if(X<6){
    PulseEn = HIGH;
    if (Code[13][X] == false) {
      digitalWrite(10, LOW);
    }
    else {
      digitalWrite(10, HIGH);
    }
    X++;
  }
  else{
    PulseEn = LOW;
  }
  }
*/

int X = 0;
boolean Code[][6] = {
  {true, false, false, false, false, false}, //0
  {true, true, false, false, false, false}, //1
  {true, false, true, false, false, false}, //2
  {true, true, true, false, false, false}, //3
  {true, false, false, true, false, false}, //4
  {true, true, false, true, false, false}, //5
  {true, false, true, true, false, false}, //6
  {true, true, true, true, false, false}, //7
  {true, false, false, false, true, false}, //8
  {true, true, false, false, true, false}, //9
  {true, false, true, false, true, false}, //10
  {true, true, true, false, true, false}, //11
  {true, false, false, true, true, false}, //12
  {true, true, false, true, true, false}, //13
  {true, false, true, true, true, false}, //14
  {true, true, true, true, true, false}, //15
};
void setup() {
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

}
void loop() {
  delay(1000);
  SendMessage(X);
  X++;
}



void SendMessage(int TwoWire_Y) {
  while(TwoWire_Y > 15){
    TwoWire_Y -= 15;
  }
  boolean TwoWire_Done = false;
boolean TwoWire_PulseEnable = LOW;
boolean TwoWire_PulseState = LOW;
unsigned long int TwoWire_M1 = 0;
int TwoWire_X = 0;
  while (TwoWire_Done == false) {
    if (micros() >= TwoWire_M1) {
      TwoWire_M1 = micros();
      TwoWire_M1 += 10;      //Clock();
      if (TwoWire_PulseEnable == HIGH or TwoWire_PulseState == HIGH) {
        if (TwoWire_PulseState == LOW) {
          digitalWrite(11, HIGH);
        }
        else {
          digitalWrite(11, LOW);
        }
        TwoWire_PulseState = digitalRead(11);            //TwoWire_PulseState = Pulse();
      }
      if (TwoWire_PulseState == LOW) {
        if (TwoWire_X < 6) {
          TwoWire_PulseEnable = HIGH;
          if (Code[TwoWire_Y][TwoWire_X] == false) {
            digitalWrite(10, LOW);
          }
          else {
            digitalWrite(10, HIGH);
          }
          TwoWire_X++;
        }
        else {
          TwoWire_PulseEnable = LOW;
          TwoWire_X = 0;
          TwoWire_Done = true;
        }                            //WriteData();
      }
    }
  }
}



//void Clock() {
//  TwoWire_M1 = micros();
//  TwoWire_M1 += 10;
//}
//boolean Pulse() {
//  if (TwoWire_PulseState == LOW) {
//    digitalWrite(11, HIGH);
//  }
//  else {
//    digitalWrite(11, LOW);
//  }
//  return digitalRead(11);
//}
//void WriteData() {
//  if (TwoWire_X < 6) {
//    TwoWire_PulseEnable = HIGH;
//    if (Code[13][TwoWire_X] == false) {
//      digitalWrite(10, LOW);
//    }
//    else {
//      digitalWrite(10, HIGH);
//    }
//    TwoWire_X++;
//  }
//  else {
//    TwoWire_PulseEnable = LOW;
//    TwoWire_X = 0;
//    TwoWire_Done = true;
//  }
//}









////RX
//
//boolean Done = false;
//unsigned long int M1 = 0;
//void setup() {
//  pinMode(10,INPUT);
//  pinMode(11,INPUT);
//  Serial.begin(2000000);
//  delay(500);
//}
//
//void loop() {
//if(digitalRead(11) == HIGH){
//  CheckData();
//}
//}
//void CheckData(){
//  Done = false;
//  while(Done == false){
//  if(digitalRead(10) == HIGH){
//    Serial.print("1");
//  }
//  else{
//    Serial.print("0");
//  }
//  M1 = millis();
//  M1 += 1;
//  while(digitalRead(11) == HIGH and Done == false){
//if(millis() > M1){
//  Done = true;
//  Serial.println();
//}
//  }
//  while(digitalRead(11) == LOW and Done == false){
//if(millis() > M1){
//  Done = true;
//  Serial.println();
//}
//  }
//}
//}
