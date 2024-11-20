//Master ▼▼▼
///*

//PORTD = PORTD | B10000000;        7/SDAT HIGH
//PORTD = PORTD & B01111111;        7/SDAT LOW
//
//PORTB = PORTB | B00000001;        8/SCLT HIGH
//PORTB = PORTB & B11111110;        8/SCLT LOW
//
//PORTB = PORTB | B00100000;
//PORTB = PORTB & B11011111;


#define NOP __asm__ __volatile__ ("nop\n\t")
uint8_t const BufferSize = (3 + 1);
uint8_t const MasterIP = 1;
uint8_t const SlaveIP[] = {2, 3, 4};
uint8_t const SlaveIP_Count = (sizeof(SlaveIP));

uint8_t Buffer[SlaveIP_Count][(BufferSize + 1)][2];
bool TX_Buffer[21];
uint8_t RX_Buffer[5];

uint8_t BufferSend[2];

void setup() {
  pinMode(13, OUTPUT);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
}

void loop() {
  delay(3);
  Run_TwoWire();
}

void Run_TwoWire() {
  for (uint8_t I = 0; I < SlaveIP_Count; I++) {
    if (Safe()) {
      CompileMessage(SlaveIP[I], Command(1, CheckBuffer(I), 0), 0, 0, LOW);       //(Slave IP, Command Bit, IP, Data, Lenght (0 = 9, 1 = 20))
      Send();
      if (Read(HIGH, HIGH)) {
        switch (CheckMessage(I)) {
            boolean CheckBuffer;
          case 1:
            CheckBuffer = BufferRX(I);
            CompileMessage(SlaveIP[I], Command(0, CheckBuffer, 0), BufferSend[0], BufferSend[1], CheckBuffer);
            if (Safe()) {
              Send();
            }
            break;
          case 2:
            CompileMessage(SlaveIP[I], Command(0, 0, BufferStore(I, RX_Buffer[3], RX_Buffer[4])), 0, 0, LOW);
            if (Safe()) {
              Send();
            }
            break;

          default:
            //ERROR
            break;
        }
      }
    }
  }
}

bool Safe() {
  unsigned long int TEMP_TIMER = micros();
  while ((micros() - TEMP_TIMER) < 30) {
    if (!bitRead(PINB, 0) or !bitRead(PIND, 7)) {
      return false;
    }
  }
  return true;
}
void Send() {
  digitalWrite(13, HIGH);
  uint8_t PacketLength;
  uint8_t PacketCount;
  bool Done = false;
  boolean PulseState = HIGH;
  boolean PulseEnable = false;
  if (!TX_Buffer[20]) {
    PacketLength = 9;
  }
  else {
    PacketLength = 20;
  }
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  while (!Done) {
    if (PulseEnable) {
      if (PulseState) {
        TwoWire_PinWrite(1, 0);
        PulseState = LOW;
      }
      else {
        TwoWire_PinWrite(1, 1);
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
        TwoWire_PinWrite(0, TX_Buffer[PacketCount]);
        PacketCount++;
      }
      else {
        Done = true;
        pinMode(7, INPUT_PULLUP);
        pinMode(8, INPUT_PULLUP);
        digitalWrite(13, LOW);
        return;
      }
      if (!PulseEnable) {
        PulseEnable = true;
      }
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
}

bool Read(bool Length_One, bool Length_Two) {
  bool Result[21];
  uint8_t MaxLength;
  uint32_t Timer = micros();
  bool Done = false;
  uint8_t NOPCOUNT;
  uint8_t BitCount2 = 0;
  if (Length_One or Length_Two) {
    if (Length_Two) {
      MaxLength = 20;
    }
    else {
      MaxLength = 9;
    }
  }
  else {
    return false;
  }
  while (bitRead(PINB, 0) and (micros() - Timer) > 100) {}
  if (bitRead(PINB, 0)) {
    return false;
  }
  for (int BitCount = 0; BitCount < (MaxLength + 1) and !Done; BitCount++) {
    Result[BitCount] = bitRead(PINB, 7);
    NOPCOUNT = 0;
    while (!bitRead(PINB, 8) and !Done) {
      NOPCOUNT++;
      if (NOPCOUNT > 30) {
        Done = true;
      }
    }
    NOPCOUNT = 0;
    while (bitRead(PINB, 8) and !Done) {
      NOPCOUNT++;
      if (NOPCOUNT > 30) {
        Done = true;
      }
    }
    BitCount2++;
  }
  if ((Length_One and BitCount2 == 9) or (Length_Two and BitCount2 == 20)) {
    for (int X = 0; X < 3; X++) {
      bitWrite(RX_Buffer[0], X , Result[X]);
    }
    for (int X = 0; X < 3; X++) {
      bitWrite(RX_Buffer[1], X , Result[X + 3]);
    }
    for (int X = 0; X < 3; X++) {
      bitWrite(RX_Buffer[2], X , Result[X + 6]);
    }
    if (BitCount2 == 20) {
      for (int X = 0; X < 3; X++) {
        bitWrite(RX_Buffer[3], X , Result[X + 9]);
      }
      for (int X = 0; X < 8; X++) {
        bitWrite(RX_Buffer[3], X , Result[X + 12]);
      }
    }
    return true;
  }
  else {
    return false;
  }
}

int8_t CheckMessage(uint8_t I) {  //Check command bit of recieved message from slave device
  if (RX_Buffer[0] == MasterIP and RX_Buffer[1] == SlaveIP[I]) {
    switch (RX_Buffer[2]) {
      case 1:
        return RX_Buffer[2];
        break;

      case 2:
        return RX_Buffer[2];
        break;

      default:
        return -1;
        break;
    }
  }
  return -1;
}

boolean BufferRX(uint8_t SlaveIP) {  //Checks if buffer available for IP and pulls it out into global array BufferSend
  if (CheckBuffer(SlaveIP)) {
    if (Buffer[SlaveIP][BufferSize][0] < (BufferSize - 1)) {
      Buffer[SlaveIP][BufferSize][0]++;
    }
    else {
      Buffer[SlaveIP][BufferSize][0] = 0;
    }
    BufferSend[0] = Buffer[SlaveIP][Buffer[SlaveIP][BufferSize][0]][0];
    BufferSend[1] = Buffer[SlaveIP][Buffer[SlaveIP][BufferSize][0]][1];
    return true;
  }
  else {
    BufferSend[0] = 0;
    BufferSend[1] = 0;
    return false;
  }
}

boolean CheckBuffer(uint8_t SlaveIP) {  //Checks if anythink available in buffer of specific IP
  if (Buffer[SlaveIP][BufferSize][0] == Buffer[SlaveIP][BufferSize][1]) {
    return false;
  }
  else {
    return true;
  }
}

void CompileMessage(uint8_t TX_IP, uint8_t COM, uint8_t MSG_IP, uint8_t MSG_DATA, bool Length) {
  for (uint8_t X = 0; X < 3; X++) {
    TX_Buffer[X] = bitRead(TX_IP, X);
  }
  for (uint8_t X = 0; X < 3; X++) {
    TX_Buffer[(X + 3)] = bitRead(MasterIP, X);
  }
  for (uint8_t X = 0; X < 3; X++) {
    TX_Buffer[(X + 6)] = bitRead(COM, X);
  }
  if (Length) {
    for (uint8_t X = 0; X < 3; X++) {
      TX_Buffer[(X + 9)] = bitRead(MSG_IP, X);
    }
    for (uint8_t X = 0; X < 8; X++) {
      TX_Buffer[(X + 12)] = bitRead(MSG_DATA, X);
    }
  }
  TX_Buffer[20] = Length;
}

uint8_t Command (boolean X, boolean Y, boolean Z) {
  uint8_t Final = X;
  Final += Y << 1;
  Final += Z << 2;
  return Final;
}

boolean BufferStore(uint8_t SenderIP, uint8_t RecieverIP, uint8_t Data) {  //Check if IP valid and if there is space in buffer for it then stores it in
  boolean FoundIP = false;
  uint8_t SlaveIP_Result = 0;
  for ( uint8_t I = 0; I < SlaveIP_Count and FoundIP == false; I++) {
    if (RecieverIP == SlaveIP[I]) {
      FoundIP = true;
      SlaveIP_Result = I;
    }
  }
  if (FoundIP == true) {
    uint8_t BufferTest = Buffer[SlaveIP_Result][BufferSize][1];
    if (Buffer[SlaveIP_Result][BufferSize][1] == (BufferSize - 1)) {
      BufferTest = 0;
    }
    else {
      BufferTest++;
    }
    if (BufferTest == Buffer[SlaveIP_Result][BufferSize][0]) {
      return false;
    }
    else {
      Buffer[SlaveIP_Result][BufferSize][1] = BufferTest;
      Buffer[SlaveIP_Result][Buffer[SlaveIP_Result][BufferSize][1]][0] = SlaveIP[SenderIP];
      Buffer[SlaveIP_Result][Buffer[SlaveIP_Result][BufferSize][1]][1] = Data;
      return true;
    }
  }
  else {
    return false;
  }
}

void TwoWire_PinWrite(bool PIN, bool DIR) {
  if (PIN) {
    if (DIR) {
      PORTD = PORTD | B10000000;
    }
    else {
      PORTD = PORTD & B01111111;
    }
  }
  else {
    if (DIR) {
      PORTB = PORTB | B00000001;
    }
    else {
      PORTB = PORTB & B11111110;
    }
  }
}

//*///Master ▲▲▲






////Slave ▼▼▼
//#define NOP __asm__ __volatile__ ("nop\n\t")
//const uint8_t SlaveIP = 2;
//const uint8_t MasterIP = 1;
//uint8_t Recieved[5];
//uint8_t TwoWire_Send[2];
//uint8_t TwoWire_Receive[2];
//
//uint8_t RX_Buffer[5];
//bool TX_Buffer[21];
//
//void setup() {
//  pinMode(13, OUTPUT);
//  pinMode(7, INPUT_PULLUP);
//  pinMode(8, INPUT_PULLUP);
//}
//
//void loop() {
//}
//
//bool TwoWire(bool Command) {
//  if (!Command) {
//    CompileMessage(1, 0, 0, false);
//    if (Parse_Packet(1, 0)) {
//      if (bitRead(RX_Buffer[2], 0) && bitRead(RX_Buffer[2], 1)) {
//        Send();
//        if (Read(1, 1)) {
//        if (CheckIP) {
//            if (bitRead(RX_Buffer[2], 1)) {
//              return true;
//            }
//          }
//        }
//      }
//    }
//  }
//  else {
//    CompileMessage(2, TwoWire_Send[0], TwoWire_Send[1], true);
//    if (Parse_Packet(1, 0)) {
//      if (bitRead(RX_Buffer[2], 0)) {
//        Send();
//        if (Read(1, 0)) {
//          if (CheckIP) {
//            if (bitRead(RX_Buffer[2], 2)) {
//              return true;
//            }
//          }
//        }
//      }
//    }
//  }
//  return false;
//}
//
//void Send() {
//  digitalWrite(13, HIGH);
//  uint8_t PacketLength;
//  uint8_t PacketCount;
//  bool Done = false;
//  boolean PulseState = HIGH;
//  boolean PulseEnable = false;
//  if (!TX_Buffer[20]) {
//    PacketLength = 9;
//  }
//  else {
//    PacketLength = 20;
//  }
//  pinMode(7, OUTPUT);
//  pinMode(8, OUTPUT);
//  while (!Done) {
//    if (PulseEnable) {
//      if (PulseState) {
//        TwoWire_PinWrite(1, 0);
//        PulseState = LOW;
//      }
//      else {
//        TwoWire_PinWrite(1, 1);
//        PulseState = HIGH;
//      }
//    }
//    else {
//      NOP;
//      NOP;
//      NOP;
//      NOP;
//      NOP;
//    }
//    if (PulseState) {
//      if (PacketCount < PacketLength) {
//        TwoWire_PinWrite(0, TX_Buffer[PacketCount]);
//        PacketCount++;
//      }
//      else {
//        Done = true;
//        pinMode(7, INPUT_PULLUP);
//        pinMode(8, INPUT_PULLUP);
//        digitalWrite(13, LOW);
//        return;
//      }
//      if (!PulseEnable) {
//        PulseEnable = true;
//      }
//    }
//    else {
//      NOP;
//      NOP;
//      NOP;
//      NOP;
//      NOP;
//      NOP;
//      NOP;
//      NOP;
//      NOP;
//      NOP;
//    }
//    for (int X = 0; X <= 20; X++) {
//      NOP;
//    }
//  }
//}
//
//bool Read(bool Length_One, bool Length_Two) {
//  bool Result[21];
//  uint8_t MaxLength;
//  uint32_t Timer = micros();
//  bool Done = false;
//  uint8_t NOPCOUNT;
//  uint8_t BitCount2 = 0;
//  if (Length_One or Length_Two) {
//    if (Length_Two) {
//      MaxLength = 20;
//    }
//    else {
//      MaxLength = 9;
//    }
//  }
//  else {
//    return false;
//  }
//  while (bitRead(PINB, 0) and (micros() - Timer) > 500) {}
//  if (bitRead(PINB, 0)) {
//    return false;
//  }
//  for (int BitCount = 0; BitCount < (MaxLength + 1) and !Done; BitCount++) {
//    Result[BitCount] = bitRead(PINB, 7);
//    NOPCOUNT = 0;
//    while (!bitRead(PINB, 8) and !Done) {
//      NOPCOUNT++;
//      if (NOPCOUNT > 30) {
//        Done = true;
//      }
//    }
//    NOPCOUNT = 0;
//    while (bitRead(PINB, 8) and !Done) {
//      NOPCOUNT++;
//      if (NOPCOUNT > 30) {
//        Done = true;
//      }
//    }
//    BitCount2++;
//  }
//  if ((Length_One and BitCount2 == 9) or (Length_Two and BitCount2 == 20)) {
//    for (int X = 0; X < 3; X++) {
//      bitWrite(RX_Buffer[0], X , Result[X]);
//    }
//    for (int X = 0; X < 3; X++) {
//      bitWrite(RX_Buffer[1], X , Result[X + 3]);
//    }
//    for (int X = 0; X < 3; X++) {
//      bitWrite(RX_Buffer[2], X , Result[X + 6]);
//    }
//    if (BitCount2 == 20) {
//      for (int X = 0; X < 3; X++) {
//        bitWrite(RX_Buffer[3], X , Result[X + 9]);
//      }
//      for (int X = 0; X < 8; X++) {
//        bitWrite(RX_Buffer[3], X , Result[X + 12]);
//      }
//    }
//    return true;
//  }
//  else {
//    return false;
//  }
//}
//
//void CompileMessage(uint8_t COM, uint8_t MSG_IP, uint8_t MSG_DATA, bool Length) {
//  for (uint8_t X = 0; X < 3; X++) {
//    TX_Buffer[X] = bitRead(MasterIP, X);
//  }
//  for (uint8_t X = 0; X < 3; X++) {
//    TX_Buffer[(X + 3)] = bitRead(SlaveIP, X);
//  }
//  for (uint8_t X = 0; X < 3; X++) {
//    TX_Buffer[(X + 6)] = bitRead(COM, X);
//  }
//  if (Length) {
//    for (uint8_t X = 0; X < 3; X++) {
//      TX_Buffer[(X + 9)] = bitRead(MSG_IP, X);
//    }
//    for (uint8_t X = 0; X < 8; X++) {
//      TX_Buffer[(X + 12)] = bitRead(MSG_DATA, X);
//    }
//  }
//  TX_Buffer[20] = Length;
//}
//
//bool Insert() {
//  uint32_t Timer = micros();
////  for(int I = 0; I < 3; I++){
////    if(!bitRead(PINB,0)){
////      return false;
////    }
////  }
//  while((micros() - Timer) < 30){
//    if(!bitRead(PINB,0)){
//      return false;
//    }
//  }
//  return true;
//}
//
//bool Parse_Packet(bool Length1, bool Length2) {
//  unsigned long int Timer = micros();
//  while ((micros() - Timer) < 20000) {
//    if (Insert()) {
//      if (Read(Length1, Length2)) {
//        if (CheckIP()) {
//          return true;
//        }
//      }
//    }
//  }
//  return false;
//}
//
//bool CheckIP() {
//  if (RX_Buffer[0] == SlaveIP and RX_Buffer[1] == MasterIP) {
//    return true;
//  }
//  return false;
//}
//void TwoWire_PinWrite(bool PIN, bool DIR) {
//  if (PIN) {
//    if (DIR) {
//      PORTD = PORTD | B10000000;
//    }
//    else {
//      PORTD = PORTD & B01111111;
//    }
//  }
//  else {
//    if (DIR) {
//      PORTB = PORTB | B00000001;
//    }
//    else {
//      PORTB = PORTB & B11111110;
//    }
//  }
//}
////Slave ▲▲▲
