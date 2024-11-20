
////TX 4.0
//unsigned long int X = 0;
//void setup() {
//  pinMode(10, OUTPUT);
//  pinMode(11, OUTPUT);
//
//}
//void loop() {
//  delay(50);
//  SendMessage(X);
//  X++;
//}
//
//
//
//void SendMessage(int TwoWire_Y) {
//  boolean Code[8];
//  while(TwoWire_Y > 255){
//    TwoWire_Y -= 255;
//  }
//  for(int i = 0; i < 8; i++){
//  Code[i] = bitRead(TwoWire_Y, i);
//}
//  boolean TwoWire_Done = false;
//boolean TwoWire_PulseEnable = LOW;
//boolean TwoWire_PulseState = LOW;
//unsigned long int TwoWire_M1 = 0;
//int TwoWire_X = 0;
//  while (TwoWire_Done == false) {
//    if (micros() >= TwoWire_M1) {
//      TwoWire_M1 = micros();
//      TwoWire_M1 += 0;      //Clock();
//      if (TwoWire_PulseEnable == HIGH or TwoWire_PulseState == HIGH) {
//        if (TwoWire_PulseState == LOW) {
//          digitalWrite(11, HIGH);
//        }
//        else {
//          digitalWrite(11, LOW);
//        }
//        TwoWire_PulseState = digitalRead(11);            //TwoWire_PulseState = Pulse();
//      }
//      if (TwoWire_PulseState == LOW) {
//        if (TwoWire_X < 8) {
//          TwoWire_PulseEnable = HIGH;
//          if (Code[TwoWire_X] == false) {
//            digitalWrite(10, LOW);
//          }
//          else {
//            digitalWrite(10, HIGH);
//          }
//          TwoWire_X++;
//        }
//        else {
//          TwoWire_PulseEnable = LOW;
//          TwoWire_X = 0;
//          TwoWire_Done = true;
//        }                            //WriteData();
//      }
//    }
//  }
//}









////RX 4.2
//int X = 0;
//void setup() {
//  pinMode(10, INPUT);
//  pinMode(11, INPUT);
//  Serial.begin(2000000);
//  delay(500);
//}
//
//void loop() {
//  if (digitalRead(11) == HIGH) {
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
//    CheckData_M1 += 100;
//    while (digitalRead(11) == HIGH and CheckData_Done == false) {
//      if (micros() > CheckData_M1) {
//        CheckData_Done = true;
//      }
//    }
//    while (digitalRead(11) == LOW and CheckData_Done == false) {
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
