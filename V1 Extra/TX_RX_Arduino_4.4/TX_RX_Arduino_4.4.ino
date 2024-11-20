//
////TX 4.0
//unsigned long int X = 0;
//const int IP = 1;
//void setup() {
//  pinMode(10, OUTPUT);
//  pinMode(11, OUTPUT);
//
//}
//void loop() {
//  delay(200);
//  SendMessage(2,X);
//  X++;
//}
//
//
//
//void SendMessage(int SendMessage_DestIp, int SendMessage_Message) {
//
//  for(int SendMessage_DataCount = 0; SendMessage_DataCount < 3; SendMessage_DataCount++){
//    int SendMessage_DataSize;
//    int SendMessage_Packet;
//    switch(SendMessage_DataCount){
//      case 0:
//      SendMessage_DataSize = 4;
//      SendMessage_Packet = IP;
//      break;
//      case 1:
//      SendMessage_DataSize = 4;
//      SendMessage_Packet = SendMessage_DestIp;
//      break;
//      case 2:
//      SendMessage_DataSize = 8;
//      SendMessage_Packet = SendMessage_Message;
//      break;
//    }
//  boolean SendMessage_Code[SendMessage_DataSize];
//  for(int i = 0; i < SendMessage_DataSize; i++){
//  SendMessage_Code[i] = bitRead(SendMessage_Packet, i);
//}
//  boolean SendMessage_Done = false;
//boolean SendMessage_PulseEnable = LOW;
//boolean SendMessage_PulseState = LOW;
//unsigned long int SendMessage_M1 = 0;
//int SendMessage_X = 0;
//  while (SendMessage_Done == false) {
//    if (micros() >= SendMessage_M1) {
//      SendMessage_M1 = micros();
//      SendMessage_M1 += 0;      //Clock();
//      if (SendMessage_PulseEnable == HIGH or SendMessage_PulseState == HIGH) {
//        if (SendMessage_PulseState == LOW) {
//          digitalWrite(11, HIGH);
//        }
//        else {
//          digitalWrite(11, LOW);
//        }
//        SendMessage_PulseState = digitalRead(11);            //SendMessage_PulseState = Pulse();
//      }
//      if (SendMessage_PulseState == LOW) {
//        if (SendMessage_X < SendMessage_DataSize) {
//          SendMessage_PulseEnable = HIGH;
//          if (SendMessage_Code[SendMessage_X] == false) {
//            digitalWrite(10, LOW);
//          }
//          else {
//            digitalWrite(10, HIGH);
//          }
//          SendMessage_X++;
//        }
//        else {
//          SendMessage_PulseEnable = LOW;
//          SendMessage_X = 0;
//          SendMessage_Done = true;
//        }                            //WriteData();
//      }
//    }
//  }
//  delayMicroseconds(200);
//  }
//}









//RX 4.2
int X = 0;
void setup() {
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  Serial.begin(2000000);
  delay(500);
}

void loop() {
  if (digitalRead(11) == HIGH) {
    X = CheckData(8);
    if(X == -1){
    Serial.println("ERROR");
    }
    else{
      Serial.println(X);
    }
  }
}
int CheckData(int CheckData_Y) {
  byte CheckData_Result = B00000000;
  boolean CheckData_Done = false;
  unsigned long int CheckData_M1 = 0;
  int CheckData_Z = 0;
  CheckData_Done = false;
  for (int CheckData_X = 0; CheckData_X < CheckData_Y and CheckData_Done == false; CheckData_X++) {
    if (digitalRead(10) == HIGH) {
      CheckData_Result += true << CheckData_X;
    }
    else {
      CheckData_Result += false << CheckData_X;
    }
    CheckData_M1 = micros();
    CheckData_M1 += 100;
    while (digitalRead(11) == HIGH and CheckData_Done == false) {
      if (micros() > CheckData_M1) {
        CheckData_Done = true;
      }
    }
    while (digitalRead(11) == LOW and CheckData_Done == false) {
      if (micros() > CheckData_M1) {
        CheckData_Done = true;
      }
    }
    CheckData_Z++;
  }
  if(CheckData_Z < CheckData_Y or CheckData_Z > CheckData_Y){
//    return -1;
return CheckData_Result;
  }
  else{
  return CheckData_Result;
  }
}
