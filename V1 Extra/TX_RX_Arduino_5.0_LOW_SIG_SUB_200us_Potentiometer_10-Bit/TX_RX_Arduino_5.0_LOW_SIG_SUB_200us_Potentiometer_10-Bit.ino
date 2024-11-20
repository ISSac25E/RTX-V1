////PORTB = PORTB | B00000100;        10/SDAT HIGH
////PORTB = PORTB & B11111011;        10/SDAT LOW
////
////PORTB = PORTB | B00001000;        11/SCLT HIGH
////PORTB = PORTB & B11110111;        11/SCLT LOW
////
////PORTB = PORTB | B00100000;
////PORTB = PORTB & B11011111;



//TX 6.0 LOW
#define NOP __asm__ __volatile__ ("nop\n\t")
const int SDAT = 10;
const int SCLT = 11;
long int X = 0;
long int anaRead = 0;
void setup() {
  pinMode(SDAT, OUTPUT);
  pinMode(SCLT, OUTPUT);
  pinMode(12, INPUT_PULLUP);
  //  pinMode(SDAT, INPUT_PULLUP);
  //  pinMode(SCLT, INPUT_PULLUP);
  pinMode(13, OUTPUT);

}

void loop() {
  anaRead = analogRead(0);
  bitWrite(anaRead, 10, 0);
  bitWrite(anaRead, 11, 0);
  Send(anaRead, 12);
  delayMicroseconds(200);
  anaRead = analogRead(1);
  bitWrite(anaRead, 10, 1);
  bitWrite(anaRead, 11, 0);
  Send(anaRead, 12);
  delayMicroseconds(200);
  anaRead = analogRead(2);
  bitWrite(anaRead, 10, 0);
  bitWrite(anaRead, 11, 1);
  Send(anaRead, 12);
  delayMicroseconds(200);
  anaRead = analogRead(3);
  bitWrite(anaRead, 10, 1);
  bitWrite(anaRead, 11, 1);
  Send(anaRead, 12);
  delayMicroseconds(200);
  
}

void Send(const int Message, const int PacketLength) {
  PORTB = PORTB | B00100000;
  int PacketCount = 0;
  unsigned long int MicrosCount = 0;

  boolean Packet[PacketLength];
  boolean Done = false;
  boolean PulseState = HIGH;
  boolean PulseEnable = false;
  for (int I = 0; I < PacketLength; I++) {
    Packet[I] = bitRead(Message, I);
  }
  //pinMode(SDAT, OUTPUT);
  //  pinMode(SCLT, OUTPUT);
  while (!Done) {
    //if((micros() - MicrosCount) >= Interval){
    //MicrosCount = micros();
    if (PulseEnable) {
      if (PulseState) {
        PORTB = PORTB & B11110111;
        PulseState = LOW;
      }
      else {
        PORTB = PORTB | B00001000;
        PulseState = HIGH;
      }
    }
    else {
      NOP;
      NOP;
      NOP;
      NOP;
      NOP;
    }
    if (PulseState) {
      if (PacketCount < PacketLength) {
        if (Packet[PacketCount]) {
          PORTB = PORTB | B00000100;
        }
        else {
          PORTB = PORTB & B11111011;
        }
        PacketCount++;
      }
      else {
        Done = true;
      }
      if (!PulseEnable) {
        PulseEnable = true;
      }


      //      }
    }
    else {
      NOP;
      NOP;
      NOP;
      NOP;
      NOP;
      NOP;
      NOP;
      NOP;
      NOP;
      NOP;
    }
    for (int X = 0; X <= 20; X++) {
      NOP;
    }
  }
  PORTB = PORTB | B00000100;
  PORTB = PORTB | B00001000;
  //  pinMode(SDAT, INPUT_PULLUP);
  //  pinMode(SCLT, INPUT_PULLUP);
  PORTB = PORTB & B11011111;
}




//bitRead(PINB,2);   10
//bitRead(PINB,3);   11




////RX 6.0 LOW
//#define NOP __asm__ __volatile__ ("nop\n\t")
//int X = 0;
//void setup() {
//  pinMode(10, INPUT_PULLUP);
//  pinMode(11, INPUT_PULLUP);
//  Serial.begin(2000000);
//  delay(500);
//}
//
//void loop() {
//  if (!bitRead(PINB, 3)) {
//    X = CheckData(12);
//    if (X == -1) {
//      Serial.println("ERROR");
//    }
//    else {
//      if(bitRead(X, 10)){
//        if(bitRead(X, 11)){
//        bitWrite(X, 10, 0);
//        bitWrite(X, 11, 0);
//        Serial.print(" - ");
//        Serial.println(X);
//        }
//        else{
//          bitWrite(X, 10, 0);
//        bitWrite(X, 11, 0);
//        Serial.print(" - ");
//        Serial.print(X);
//        }
//      }
//      else{
//        if(bitRead(X, 11)){
//        bitWrite(X, 10, 0);
//        bitWrite(X, 11, 0);
//        Serial.print(" - ");
//        Serial.print(X);
//        }
//        else{
//          bitWrite(X, 10, 0);
//        bitWrite(X, 11, 0);
//        Serial.print(" - ");
//        Serial.print(X);
//        }
//      }
//    }
//  }
//}
//int CheckData(int CheckData_Y) {
//  uint16_t CheckData_Result = B0;
//  boolean CheckData_Done = false;
//  unsigned long int CheckData_M1 = 0;
//  int CheckData_Z = 0;
//  long unsigned NOPCOUNT = 0;
//  CheckData_Done = false;
//  //  CheckData_M1 = micros();
//  //    CheckData_M1 += 150;
//  for (int CheckData_X = 0; CheckData_X < CheckData_Y and !CheckData_Done; CheckData_X++) {
//    //    if(!bitRead(PINB,3)){
//    CheckData_Result += bitRead(PINB, 2) << CheckData_X;
//    //    CheckData_M1 = micros();
//    //    CheckData_M1 += 150;
//    NOPCOUNT = 0;
//    while (!bitRead(PINB, 3) and !CheckData_Done) {
//      //      NOP;
//      NOPCOUNT++;
//      if (NOPCOUNT > 30) {
//        CheckData_Done = true;
//      }
//    }
//    NOPCOUNT = 0;
//    while (bitRead(PINB, 3) and !CheckData_Done) {
//      //      NOP;
//      NOPCOUNT++;
//      if (NOPCOUNT > 30) {
//        CheckData_Done = true;
//      }
//    }
//
//    CheckData_Z++;
//
//  }
//  if (CheckData_Z == CheckData_Y) {
//    return CheckData_Result;
//  }
//  else {
//    Serial.print("ERROR ");
//    Serial.print(CheckData_Z);
//    Serial.print(" - ");
//    return CheckData_Result;
//  }
//}
