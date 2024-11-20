
////TX 4.3
//unsigned long int X = 0;
//void setup() {
//  pinMode(3, OUTPUT);
//  pinMode(5, OUTPUT);
//
//}
//void loop() {
//  delayMicroseconds(350);
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
//          digitalWrite(5, HIGH);
//        }
//        else {
//          digitalWrite(5, LOW);
//        }
//        TwoWire_PulseState = digitalRead(5);            //TwoWire_PulseState = Pulse();
//      }
//      if (TwoWire_PulseState == LOW) {
//        if (TwoWire_X < 8) {
//          TwoWire_PulseEnable = HIGH;
//          if (Code[TwoWire_X] == false) {
//            digitalWrite(3, LOW);
//          }
//          else {
//            digitalWrite(3, HIGH);
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









////RX 4.3
//int X = 0;
//void setup() {
//  pinMode(3, INPUT_PULLUP);
//  pinMode(5, INPUT_PULLUP);
//  Serial.begin(2000000);
//  delay(500);
//}
//
//void loop() {
//  if (digitalRead(5) == HIGH) {
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
//    if (digitalRead(3) == HIGH) {
//      CheckData_Result += true << CheckData_X;
//    }
//    else {
//      CheckData_Result += false << CheckData_X;
//    }
//    CheckData_M1 = micros();
//    CheckData_M1 += 100;
//    while (digitalRead(5) == HIGH and CheckData_Done == false) {
//      if (micros() > CheckData_M1) {
//        CheckData_Done = true;
//      }
//    }
//    while (digitalRead(5) == LOW and CheckData_Done == false) {
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
