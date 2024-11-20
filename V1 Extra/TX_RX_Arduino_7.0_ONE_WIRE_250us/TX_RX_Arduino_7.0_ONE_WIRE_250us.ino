//int X = 0;
//void setup() {
//  PinSet(0);
//  pinMode(13,OUTPUT);
//}
//
//void loop() {
//  delayMicroseconds(280);
//  Send(X, 8);
//  X++;
//}
//
//void Send(const uint8_t Message, const uint8_t PacketLength) {
//    const uint8_t StartBitDelay_us = 20;
//    const uint8_t SpikeBitDelay_us = 10;
//    const uint8_t DataBitDelay_us = 20;
//
//   bool Packet[PacketLength];
//   for(int X = 0; X < PacketLength; X++){
//    Packet[X] = bitRead(Message, X);
//   }
//   DebugWrite(1);
//   PinSet(1);
//   PinWrite(0);
//   delayMicroseconds(StartBitDelay_us);
//   PinWrite(1);
//   for(int X = 0; X < PacketLength; X++){
//    delayMicroseconds(SpikeBitDelay_us);
//    PinWrite(Packet[X]);
//    delayMicroseconds(DataBitDelay_us);
//    PinWrite(!Packet[X]);
//   }
//   delayMicroseconds(SpikeBitDelay_us);
//   PinSet(0);
//   DebugWrite(0);
//}
//
//void DebugWrite(bool DIR) {
//  if (DIR) {
//    PORTB = PORTB | B00100000;
//  }
//  else {
//    PORTB = PORTB & B11011111;
//  }
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
  bool Packet[PacketLength + 1];
  bool CheckData_Done = false;
  int Y = 0;
  for(int B = 0; B < PacketLength; B++){
    Packet[B] = false;
  }
  int A = 0;
  while(!bitRead(PINB,2) and !CheckData_Done){
      delayMicroseconds(1);
      Y++;
      if(Y > 40) CheckData_Done = true;
    }
    Y = 0;
  for(A = 0; A <= PacketLength and !CheckData_Done; A++){
    delayMicroseconds(18);
    TempBitHold = bitRead(PINB,2);
    while(bitRead(PINB,2) == TempBitHold and !CheckData_Done){
      delayMicroseconds(1);
      Y++;
      if(Y > 40){
        CheckData_Done = true;
        A--;
      }
    }
    Y = 0;
    if(!CheckData_Done) Packet[A] = TempBitHold;
  }
  int Result = 0;
  for(int X = 0; X < PacketLength; X++){
    bitWrite(Result, X, Packet[X]);
  }
  if(A < PacketLength) return -1;
  if(A > PacketLength) return -2;
  return Result;
}
