//OneWire 7.6 TX
uint32_t X = 0;
void setup() {
}

void loop() {
  Send(X, 8);
  delayMicroseconds(280);
  PIN_SET(1);
  PIN_WRITE(0);
  delayMicroseconds(3);
  PIN_WRITE(1);
  delayMicroseconds(50);
  PIN_SET(0);
  X++;
}

void Send(uint32_t Packet, uint8_t PacketLength) {
  const uint8_t StartBitDelay_us = 20;
  const uint8_t SpikeBitDelay_us = 10;
  const uint8_t DataBitDelay_us = 20;

  bool BitPacket[PacketLength];
  for(int X = 0; X < PacketLength; X++) BitPacket[X] = bitRead(Packet, X);

  PIN_SET(1);
  PIN_WRITE(0);
  delayMicroseconds(StartBitDelay_us);
  PIN_WRITE(1);
  for(uint8_t BitCount = 0; BitCount < PacketLength; BitCount++){
    delayMicroseconds(SpikeBitDelay_us);
    PIN_WRITE(BitPacket[BitCount]);
    delayMicroseconds(DataBitDelay_us);
    PIN_WRITE(!BitPacket[BitCount]);
  }
  delayMicroseconds(SpikeBitDelay_us);
  PIN_SET(0);
}

bool PIN_READ() {
  return bitRead(PINB, 2);
}

void PIN_SET(bool DIR) {
  if (DIR) DDRB |= B00000100;
  else {
    PORTB |= B00000100;
    DDRB &= B11111011;
  }
}

void PIN_WRITE(bool DIR) {
  if (DIR) PORTB |= B00000100;
  else PORTB &= B11111011;
}

bool DEBUG_READ() {
  return bitRead(PINB, 5);
}

void DEBUG_SET(bool DIR) {
  if (DIR) DDRB |= B00100000;
  else {
    PORTB |= B00100000;
    DDRB &= B11011111;
  }
}

void DEBUG_WRITE(bool DIR) {
  if (DIR) PORTB |= B00100000;
  else PORTB &= B11011111;
}













////OneWire 7.6 RX
//void setup() {
//PIN_SET(0);
//Serial.begin(2000000);
//}
//
//void loop() {
//if(!PIN_READ()){
//  int16_t TempReadResult = Read(8);
//  if(TempReadResult == -3);
//  else Serial.println(TempReadResult);
//}
//}
//
//int16_t Read(const uint8_t PacketLength) {
//  uint16_t TimerCounter = 0;
//  uint16_t TimerDelay = 20;
//  uint16_t TimerDelayMin = 5;
//  TimerDelay *= 2.5;
//  TimerDelayMin *= 2.5;
//
//  bool TempBitHold;
//  uint8_t PacketCount = 0;
//  bool TempBuffer[PacketLength + 1];
//  bool Done = false;
//  while (!PIN_READ() and TimerCounter < TimerDelay) TimerCounter++;
//  if (TimerCounter < TimerDelay and TimerCounter > TimerDelayMin) {
//    TimerDelay = 10 * 2.5;
//    while(PacketCount <= PacketLength and !Done) {
//      delayMicroseconds(18);
//      TempBitHold = PIN_READ();
//      TimerCounter = 0;
//      while (PIN_READ() == TempBitHold and TimerCounter < TimerDelay) TimerCounter++;
//      if(TimerCounter >= TimerDelay) Done = true;
//      else {
//        TempBuffer[PacketCount] = TempBitHold;
//        PacketCount++;
//      }
//    }
//  } else{
//    if(!(TimerCounter > TimerDelayMin))return -3;
//    else return -4;
//  }
//  if (PacketCount > PacketLength) return -2;
//  if (PacketCount < PacketLength) return -1;
//  uint16_t Result = 0;
//  for(int X = 0; X < PacketCount; X++) bitWrite(Result, X, TempBuffer[X]);
//  return Result;
//}
//
//bool PIN_READ() {
//  return bitRead(PINB, 2);
//}
//
//void PIN_SET(bool DIR) {
//  if (DIR) DDRB |= B00000100;
//  else {
//    PORTB |= B00000100;
//    DDRB &= B11111011;
//  }
//}
//
//void PIN_WRITE(bool DIR) {
//  if (DIR) PORTB |= B00000100;
//  else PORTB &= B11111011;
//}
//
//bool DEBUG_READ() {
//  return bitRead(PINB, 5);
//}
//
//void DEBUG_SET(bool DIR) {
//  if (DIR) DDRB |= B00100000;
//  else {
//    PORTB |= B00100000;
//    DDRB &= B11011111;
//  }
//}
//
//void DEBUG_WRITE(bool DIR) {
//  if (DIR) PORTB |= B00100000;
//  else PORTB &= B11011111;
//}
