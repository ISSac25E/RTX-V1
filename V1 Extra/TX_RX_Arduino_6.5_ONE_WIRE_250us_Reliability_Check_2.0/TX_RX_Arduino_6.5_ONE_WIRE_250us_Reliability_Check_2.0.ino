//int X = 0;
//void setup() {
//  PinSet(0);
//  pinMode(13,OUTPUT);
//}
//
//void loop() {
//  Send(X, 8);
//  delayMicroseconds(280);
//  X++;
//}
//
//void Send(const uint8_t Message, const uint8_t PacketLength) {
//   bool Packet[PacketLength];
//   for(int X = 0; X < PacketLength; X++){
//    Packet[X] = bitRead(Message, X);
//   }
//   DebugWrite(1);
//   PinSet(1);
//   PinWrite(0);
//   delayMicroseconds(10);
//   for(int X = 0; X < PacketLength; X++){
//    PinWrite(Packet[X]);
//    delayMicroseconds(25);
//    PinWrite(!Packet[X]);
//    delayMicroseconds(10);
//   }
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
//    PORTB = PORTB | B00000001;
//  }
//  else {
//    PORTB = PORTB & B11111110;
//  }
//}
// 
//void PinSet(bool DIR) {
//  if (DIR) {
//    DDRB = DDRB | B00000001;
//  }
//  else {
//    PORTB = PORTB | B00000001;
//    DDRB = DDRB & B11111110;
//  }
//}




uint32_t Timer;
uint32_t Counter;

uint32_t TotalMessages;

int ReadResult;
int ExpectedResult;
void setup(){
  pinMode(8,INPUT_PULLUP);
//  pinMode(11,INPUT_PULLUP);
  Serial.begin(2000000);
  Timer = millis();
}

void loop(){
//  if(!bitRead(PINB,3)){
//    Serial.print("Total: ");
//    Serial.println(TotalMessages);
//    uint32_t Seconds = 0;
//    uint32_t Minutes = 0;
//    uint32_t Hours = 0;
//    Counter += (millis() - Timer);
//    uint32_t TempCounter = Counter;
//    while (TempCounter >= 1000) {
//      Seconds++;
//      TempCounter -= 1000;
//    }
//    while (Seconds >= 60) {
//      Minutes++;
//      Seconds -= 60;
//    }
//    while (Minutes >= 60) {
//      Hours++;
//      Minutes -= 60;
//    }
//    Serial.print(Hours);
//    Serial.print(":");
//    Serial.print(Minutes);
//    Serial.print(":");
//    Serial.print(Seconds);
//    Serial.print(".");
//    Serial.println(TempCounter);
//    while(!bitRead(PINB,3)){}
//     while (!Serial.available()) {}
//        while (Serial.available()) Serial.read();
//        while(bitRead(PINB,2)){}
//        Read(8);
//        while(bitRead(PINB,2)){}
//        ReadResult = Read(8);
//        ExpectedResult = ReadResult;
//        if (ExpectedResult < 255) ExpectedResult++;
//        else ExpectedResult = 0;
//        Timer = millis();
//  }
  if(!bitRead(PINB,0)){
    ReadResult = Read(8);
    Serial.println(ReadResult);
    if(ReadResult != ExpectedResult){
      while (Serial.available()) Serial.read();
        Serial.print("ERROR: ");
        Serial.print(ExpectedResult);
        Serial.println(" :<<<<<<");
        Serial.print("Total: ");
    Serial.println(TotalMessages);
    uint32_t Seconds = 0;
    uint32_t Minutes = 0;
    uint32_t Hours = 0;
    Counter += (millis() - Timer);
    uint32_t TempCounter = Counter;
    while (TempCounter >= 1000) {
      Seconds++;
      TempCounter -= 1000;
    }
    while (Seconds >= 60) {
      Minutes++;
      Seconds -= 60;
    }
    while (Minutes >= 60) {
      Hours++;
      Minutes -= 60;
    }
    Serial.print(Hours);
    Serial.print(":");
    Serial.print(Minutes);
    Serial.print(":");
    Serial.print(Seconds);
    Serial.print(".");
    Serial.println(TempCounter);
         while (!Serial.available()) {}
        while (Serial.available()) Serial.read();
        while(bitRead(PINB,0)){}
        Read(8);
        while(bitRead(PINB,0)){}
        ReadResult = Read(8);
        ExpectedResult = ReadResult;
        if (ExpectedResult < 255) ExpectedResult++;
        else ExpectedResult = 0;
        Timer = millis();
    }
    else{
      TotalMessages++;
      if (ExpectedResult < 255) ExpectedResult++;
        else ExpectedResult = 0;
    }
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
    delayMicroseconds(22);
    TempBitHold = bitRead(PINB,0);
    while(bitRead(PINB,0) == TempBitHold and !CheckData_Done){
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
  if(A < PacketLength + 1) return -1;
  if(A > PacketLength + 1) return -2;
  return Result;
}
