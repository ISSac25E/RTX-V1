//Master ▼▼▼
///*
uint8_t const BufferSize = (3 + 1);
uint8_t const MasterIP = 1;
uint8_t const SlaveIP[] = {2, 3, 4};
uint8_t const SlaveIP_Count = (sizeof(SlaveIP));

uint8_t Buffer[SlaveIP_Count][(BufferSize + 1)][2];
bool TX_Buffer[21];
uint8_t RX_Buffer[5];

uint8_t BufferSend[2];

const int SDAT = 10;
const int SCLT = 11;

void setup() {
  Serial.begin(9600);
  pinMode(SDAT, INPUT_PULLUP);
  pinMode(SCLT, INPUT_PULLUP);
}

void loop() {
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
  return HIGH;
}
void Send() {
  TwoWireSend();
  uint8_t Message[5] = {0, 0, 0, 0, 0};
  for (uint8_t I = 0; I < 3; I++) {
    bitWrite(Message[0], I, TX_Buffer[I]);
  }
  for (uint8_t I = 0; I < 3; I++) {
    bitWrite(Message[1], I, TX_Buffer[I + 3]);
  }
  for (uint8_t I = 0; I < 3; I++) {
    bitWrite(Message[2], I, TX_Buffer[I + 6]);
  }
  for (uint8_t I = 0; I < 3; I++) {
    bitWrite(Message[3], I, TX_Buffer[I + 9]);
  }
  for (uint8_t I = 0; I < 8; I++) {
    bitWrite(Message[4], I, TX_Buffer[I + 12]);
  }

  for (uint8_t I = 0; I < 5; I++) {
    Serial.print(Message[I]);
    Serial.print(",");
  }
  Serial.print("Length = ");
  Serial.println(TX_Buffer[20]);
}

void TwoWireSend() {
  const uint8_t Interval = 20;
  uint8_t PacketLength;
  uint8_t BitCount = 0;
  uint32_t Timer = 0;
  bool PulseState = HIGH;
  bool PulseEnable = false;
  if (TX_Buffer[20]) {
    PacketLength = 20;
  }
  else {
    PacketLength = 9;
  }
  pinMode(SDAT, OUTPUT);
  pinMode(SCLT, OUTPUT);
  while (1) {
    if ((micros() - Timer) >= Interval) {
      if (PulseEnable) {
        TwoWire_PinWrite(1, !PulseState);
        PulseState = !PulseState;
      }
      else {
        PulseEnable = true;
      }
      if (PulseState) {
        if (BitCount < PacketLength) {
          TwoWire_PinWrite(0, TX_Buffer[BitCount]);
          BitCount++;
        }
        else {
          pinMode(SDAT, INPUT_PULLUP);
          pinMode(SCLT, INPUT_PULLUP);
          return;
        }
      }
      Timer = micros();
    }
  }
}
void TwoWire_PinWrite(bool PIN, bool DIR) {
  if (PIN) {
    if (DIR) {
      PORTB = PORTB | B00001000;
    }
    else {
      PORTB = PORTB & B11110111;
    }
  }
  else {
    if (DIR) {
      PORTB = PORTB | B00000100;
    }
    else {
      PORTB = PORTB & B11111011;
    }
  }
}

bool Read(bool Length_One, bool Lenght_Two) {
  boolean DoneRead = false;
  while (!DoneRead) {
    delay(20);
    while (Serial.available()) {
      Serial.read();
    }

    Serial.print("EnterMessage? Y or N:");
    while (!Serial.available()) {

    }
    if (Serial.read() == 89) {
      Serial.println(" YES");
    }
    else {
      Serial.println(" NO");
      return false;
    }
    delay(20);
    while (Serial.available()) {
      Serial.read();
    }
    Serial.println("EnterMessage:");
    while (!Serial.available()) {

    }
    delay(30);
    int RecievedCount = 0;
    boolean Done = false;
    int SerialR = 0;
    int SerialRead[3];
    int SerialCount = 0;
    int SerialResult = 0;
    int Multiply[] = {100, 10, 1};
    while (Serial.available() or !Done) {
      SerialR = (Serial.read() - 48);
      if (SerialCount < 3 and SerialR >= 0 and SerialR <= 9) {
        Done = false;
        SerialRead[SerialCount] = SerialR;
        SerialCount++;
      }
      else {
        Done = true;

        for (int I = 0; I < SerialCount; I++) {
          SerialResult += SerialRead[I] * Multiply[(3 - SerialCount) + I];
        }
        RX_Buffer[RecievedCount] = SerialResult;
        if (RecievedCount < 4) {
          RecievedCount++;
        }

        SerialR = 0;
        SerialRead[0] = 0;
        SerialRead[1] = 0;
        SerialRead[2] = 0;
        SerialCount = 0;
        SerialResult = 0;
      }
    }
    for (int I = 0; I < 5; I++) {
      Serial.print(RX_Buffer[I]);
      Serial.print(",");
    }
    Serial.println();
    Serial.print("Y or N: ");
    while (!Serial.available()) {

    }
    if (Serial.read() == 89) {
      Serial.println("YES");
      delay(20);
      while (Serial.available()) {
        Serial.read();
      }
      DoneRead = true;
    }
    else {
      Serial.println("NO");
      delay(20);
      while (Serial.available()) {
        Serial.read();
      }
      DoneRead = false;
    }
  }
  return true;
}

bool TwoWire_Read(bool Length1, bool Length2){
  const uint8_t TimerInterval = 60;
  uint8_t MaxPacketLength;
  bool RX_TempBuffer[21];
  unsigned long int Timer;
  uint8_t PacketCount;
  bool Done = false;
  if(Length1 or Length2){
    if(Length2){
      MaxPacketLength = 20;
    }
    else{
      MaxPacketLength = 9;
    }
  }
  else{
    return false;
  }
  for(PacketCount = 0; PacketCount <= MaxPacketLength and !Done; PacketCount++){
    RX_TempBuffer[PacketCount] = TwoWire_PinRead(0);
    Timer = micros();
    while(!TwoWire_PinRead(1) and !Done){
      if((micros() - Timer) > TimerInterval){
        Done = true;
      }
    }
      while(TwoWire_PinRead(1) and !Done){
      if((micros() - Timer) > TimerInterval){
        Done = true;
      }
    }
  }
  if((Length1 and PacketCount == 9) or (Length2 and PacketCount == 20)){
    for (int X = 0; X < 3; X++) {
      bitWrite(RX_Buffer[0], X , RX_TempBuffer[X]);
    }
    for (int X = 0; X < 3; X++) {
      bitWrite(RX_Buffer[1], X , RX_TempBuffer[X + 3]);
    }
    for (int X = 0; X < 3; X++) {
      bitWrite(RX_Buffer[2], X , RX_TempBuffer[X + 6]);
    }
    if ((PacketCount + 1) == 20) {
      for (int X = 0; X < 3; X++) {
        bitWrite(RX_Buffer[3], X , RX_TempBuffer[X + 9]);
      }
      for (int X = 0; X < 8; X++) {
        bitWrite(RX_Buffer[3], X , RX_TempBuffer[X + 12]);
      }
    }
    return true;
  }
  else{
    Serial.println(PacketCount);
    return false;
  }
}



bool TwoWire_PinRead(bool PIN){
  if(PIN){
    return bitRead(PINB, 3);
  }
  else{
    return bitRead(PINB, 2);
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

//*///Master ▲▲▲






////Slave ▼▼▼
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
//}
//
//void loop() {
//}
//
//bool TwoWire(bool Command) {
//  if (!Command) {
//    CompileMessage(1, 0, 0, false);
//    if (Parse_Packet(1,0)) {
//      if (bitRead(RX_Buffer[2], 0) && bitRead(RX_Buffer[2], 1)) {
//        Send();
//        if (Read(1, 1) {
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
//    if (Parse_Packet(1,0)) {
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
//bool Read(bool Length_One, bool Lenght_Two) {
//
//}
//
//void Send() {
//
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
//return true;
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
//  if (RX_Buffer[0] = SlaveIP and RX_Buffer[1] = MasterIP) {
//    return true;
//  }
//  return false;
//}
////Slave ▲▲▲
