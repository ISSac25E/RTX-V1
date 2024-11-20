


//TX 4.0
unsigned long int X = 0;
void setup() {
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

}
void loop() {
  delay(5);
  SendMessage(X);
  X++;
}



void SendMessage(int TwoWire_Y) {
  boolean Code[][6] = {
  {false, false, false, false, false, false}, //0
  {true, false, false, false, false, false}, //1
  {false, true, false, false, false, false}, //2
  {true, true, false, false, false, false}, //3
  {false, false, true, false, false, false}, //4
  {true, false, true, false, false, false}, //5
  {false, true, true, false, false, false}, //6
  {true, true, true, false, false, false}, //7
  {false, false, false, true, false, false}, //8
  {true, false, false, true, false, false}, //9
  {false, true, false, true, false, false}, //10
  {true, true, false, true, false, false}, //11
  {false, false, true, true, false, false}, //12
  {true, false, true, true, false, false}, //13
  {false, true, true, true, false, false}, //14
  {true, true, true, true, false, false}, //15
  {false, false, false, false, true, false}, //16
  {true, false, false, false, true, false}, //17
  {false, true, false, false, true, false}, //18
  {true, true, false, false, true, false}, //19
  {false, false, true, false, true, false}, //20
  {true, false, true, false, true, false}, //21
  {false, true, true, false, true, false}, //22
  {true, true, true, false, true, false}, //23
  {false, false, false, true, true, false}, //24
  {true, false, false, true, true, false}, //25
  {false, true, false, true, true, false}, //26
  {true, true, false, true, true, false}, //27
  {false, false, true, true, true, false}, //28
  {true, false, true, true, true, false}, //29
  {false, true, true, true, true, false}, //30
  {true, true, true, true, true, false}, //31
  {false, false, false, false, false, true}, //32
  {true, false, false, false, false, true}, //33
  {false, true, false, false, false, true}, //34
  {true, true, false, false, false, true}, //35
  {false, false, true, false, false, true}, //36
  {true, false, true, false, false, true}, //37
  {false, true, true, false, false, true}, //38
  {true, true, true, false, false, true}, //39
  {false, false, false, true, false, true}, //40
  {true, false, false, true, false, true}, //41
  {false, true, false, true, false, true}, //42
  {true, true, false, true, false, true}, //43
  {false, false, true, true, false, true}, //44
  {true, false, true, true, false, true}, //45
  {false, true, true, true, false, true}, //46
  {true, true, true, true, false, true}, //47
  {false, false, false, false, true, true}, //48
  {true, false, false, false, true, true}, //49
  {false, true, false, false, true, true}, //50
  {true, true, false, false, true, true}, //51
  {false, false, true, false, true, true}, //52
  {true, false, true, false, true, true}, //53
  {false, true, true, false, true, true}, //54
  {true, true, true, false, true, true}, //55
  {false, false, false, true, true, true}, //56
  {true, false, false, true, true, true}, //57
  {false, true, false, true, true, true}, //58
  {true, true, false, true, true, true}, //59
  {false, false, true, true, true, true}, //60
  {true, false, true, true, true, true}, //61
  {false, true, true, true, true, true}, //62
  {true, true, true, true, true, true}, //63
};
  while(TwoWire_Y > 63){
    TwoWire_Y -= 63;
  }
  boolean TwoWire_Done = false;
boolean TwoWire_PulseEnable = LOW;
boolean TwoWire_PulseState = LOW;
unsigned long int TwoWire_M1 = 0;
int TwoWire_X = 0;
  while (TwoWire_Done == false) {
    if (micros() >= TwoWire_M1) {
      TwoWire_M1 = micros();
      TwoWire_M1 += 0;      //Clock();
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
//    X = CheckData(6);
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
//    CheckData_M1 = millis();
//    CheckData_M1 += 2;
//    while (digitalRead(11) == HIGH and CheckData_Done == false) {
//      if (millis() > CheckData_M1) {
//        CheckData_Done = true;
//      }
//    }
//    while (digitalRead(11) == LOW and CheckData_Done == false) {
//      if (millis() > CheckData_M1) {
//        CheckData_Done = true;
//      }
//    }
//    CheckData_Z++;
//  }
//  if(CheckData_Z < CheckData_Y or CheckData_Z > CheckData_Y){
//    return -1;
//  }
//  else{
//  return CheckData_Result;
//  }
//}
