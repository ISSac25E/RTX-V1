////TX 5.0 LOW
//const int SDAT = 10;
//const int SCLT = 11;
//long int X = 0;
//void setup() {
//  pinMode(SDAT, INPUT_PULLUP);
//  pinMode(SCLT, INPUT_PULLUP);
//
//}
//
//void loop() {
//  delayMicroseconds(350);
//Send(X, 8);
//X++;
//}
//
//void Send(const int Message, const int PacketLength){
//  int PacketCount = 0;
//  unsigned long int MicrosCount = 0;
//  const int Interval = 20;
//  boolean Packet[PacketLength];
//  boolean Done = false;
//  boolean PulseState = HIGH;
//  boolean PulseEnable = false;
//      for(int I = 0; I < PacketLength; I++){
// Packet[I] = bitRead(Message, I);
//}
//pinMode(SDAT, OUTPUT);
//  pinMode(SCLT, OUTPUT);
//  while(!Done){
//    if((micros() - MicrosCount) >= Interval){
//      if(PulseEnable){
//        if(PulseState){
//          digitalWrite(SCLT, LOW);
//          PulseState = LOW;
//        }
//        else{
//          digitalWrite(SCLT, HIGH);
//          PulseState = HIGH;
//        }
//      }
//      if(PulseState){
//        if(PacketCount < PacketLength){
//        if(Packet[PacketCount]){
//          digitalWrite(SDAT, HIGH);
//        }
//        else{
//          digitalWrite(SDAT, LOW);
//        }
//          PacketCount++;
//        }
//        else{
//          Done = true;
//        }
//        if(!PulseEnable){
//          PulseEnable = true;
//        }
//      }
//      MicrosCount = micros();
//    }
//  }
//  pinMode(SDAT, INPUT_PULLUP);
//  pinMode(SCLT, INPUT_PULLUP);
//}









////RX 5.0 LOW
//int X = 0;
//void setup() {
//  pinMode(10, INPUT_PULLUP);
//  pinMode(11, INPUT_PULLUP);
//  Serial.begin(2000000);
//  delay(500);
//}
//
//void loop() {
//  if (digitalRead(11) == LOW) {
//    X = CheckData(8);
//    if(X == -1){
//    Serial.println("ERROR");
//    }
//    else{
//      Serial.println(X);
//    }
//  }
//}
//int CheckData(int CheckData_Y) {
//  byte CheckData_Result = B00000000;
//  boolean CheckData_Done = false;
//  unsigned long int CheckData_M1 = 0;
//  int CheckData_Z = 0;
//  CheckData_Done = false;
//  for (int CheckData_X = 0; CheckData_X < CheckData_Y and CheckData_Done == false; CheckData_X++) {
//    if (digitalRead(10) == HIGH) {
//      CheckData_Result += true << CheckData_X;
//    }
//    else {
//      CheckData_Result += false << CheckData_X;
//    }
//    CheckData_M1 = micros();
//    CheckData_M1 += 200;
//    while (digitalRead(11) == LOW and CheckData_Done == false) {
//      if (micros() > CheckData_M1) {
//        CheckData_Done = true;
//      }
//    }
//    while (digitalRead(11) == HIGH and CheckData_Done == false) {
//      if (micros() > CheckData_M1) {
//        CheckData_Done = true;
//      }
//    }
//    CheckData_Z++;
//  }
//  if(CheckData_Z < CheckData_Y or CheckData_Z > CheckData_Y){
//    return -1;
//
//  }
//  else{
//  return CheckData_Result;
//  }
//}
