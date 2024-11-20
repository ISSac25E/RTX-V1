//void setup() {
//  PinSet(0);
//}
//
//void loop() {
//  Send(155, 8);
//  delayMicroseconds(400);
//}
//
//void Send(const uint8_t Message, const uint8_t PacketLength) {
//   bool Packet[PacketLength];
//   for(int X = 0; X < PacketLength; X++){
//    Packet[X] = bitRead(Message, X);
//   }
//   PinSet(1);
//   PinWrite(0);
//   delayMicroseconds(10);
//   for(int X = 0; X < PacketLength; X++){
//    PinWrite(Packet[X]);
//    delayMicroseconds(20);
//    PinWrite(!Packet[X]);
//    delayMicroseconds(10);
//   }
//   PinSet(0);
//}
//
//void PinWrite(bool DIR) {
//  if (DIR) {
//    PORTB = PORTB | B00000100;
//  }
//  else {
//    PORTB = PORTB & B11111011;
//  }
//}
//
//void PinSet(bool DIR) {
//  if (DIR) {
//    DDRB = DDRB | B00000100;
//  }
//  else {
//    PORTB = PORTB | B00000100;
//    DDRB = DDRB & B11111011;
//  }
//}





int X;
void setup(){
  pinMode(10,INPUT_PULLUP);
  Serial.begin(2000000);
}

void loop(){
  if(!bitRead(PINB,2)){
    X = Read(8);
    Serial.println(X);
  }
}

int Read(uint8_t PacketLength){
  bool TempBitHold = false;
  bool Packet[PacketLength];
  bool CheckData_Done = false;
  int Y = 0;
  for(int B = 0; B < PacketLength; B++){
    Packet[B] = false;
  }
  int A;
  for(A = 0; A <= PacketLength + 1 and !CheckData_Done; A++){
    delayMicroseconds(18);
    TempBitHold = bitRead(PINB,2);
    while(bitRead(PINB,2) == TempBitHold and !CheckData_Done){
      delayMicroseconds(1);
      Y++;
      if(Y > 40) CheckData_Done = true;
    }
    Y = 0;
    if(!CheckData_Done) Packet[A] = TempBitHold;
  }
  int Result = 0;
  for(int X = 0; X < PacketLength; X++){
    bitWrite(Result, X, Packet[X]);
  }
  if(A != PacketLength + 1) return -1;
  return Result;
}
