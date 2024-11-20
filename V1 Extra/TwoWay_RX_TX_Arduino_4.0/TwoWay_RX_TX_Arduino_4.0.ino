//Master ▼▼▼

uint8_t const TwoWire_G_BufferSize = 10;
uint8_t const TwoWire_G_MasterIP = 1;
uint8_t const TwoWire_G_SlaveIP[] = {2, 3, 4, 5};
uint8_t const TwoWire_G_SlaveIPCount = (sizeof(TwoWire_G_SlaveIP));

bool TwoWire_TX_Buffer[21];
uint8_t TwoWire_RX_Buffer[5];

bool TwoWire_SlaveConnected[TwoWire_G_SlaveIPCount];
uint8_t TwoWire_Buffer[TwoWire_G_SlaveIPCount][(TwoWire_G_BufferSize + 2)][2];
unsigned long int TwoWire_SlaveTimer[TwoWire_G_SlaveIPCount];
int TwoWire_DisconnectedCounter = 0;
uint8_t TwoWire_BufferSend[2];


void setup() {
  Serial.begin(2000000);
  TwoWire_PinSet(0, 0);
  TwoWire_PinSet(1, 0);
}

void loop() {
  TwoWire_Run();
}

void TwoWire_Run() {
  for (uint8_t I = 0; I < TwoWire_G_SlaveIPCount; I++) {
    
    if (TwoWire_CheckConnectionInterval(I, 0)) {
      if (TwoWire_Safe()) {
        TwoWire_CompileMessage(TwoWire_G_SlaveIP[I], TwoWire_Command(1, CheckBuffer(I), 0), 0, 0, LOW);
        TwoWire_Send();
        if (TwoWire_Read(HIGH, HIGH)) {
          switch (TwoWire_CheckMessage(I)) {
              boolean TwoWire_CheckBuffer;
            case 1:
              TwoWire_CheckBuffer = BufferRX(I);
              TwoWire_CompileMessage(TwoWire_G_SlaveIP[I], TwoWire_Command(1, TwoWire_CheckBuffer, 0), TwoWire_BufferSend[0], TwoWire_BufferSend[1], TwoWire_CheckBuffer);
              if (TwoWire_Safe()) {
                TwoWire_Send();
              }
              break;
            case 2:
              TwoWire_CompileMessage(TwoWire_G_SlaveIP[I], TwoWire_Command(1, 0, TwoWire_BufferStore(I, TwoWire_RX_Buffer[3], TwoWire_RX_Buffer[4])), 0, 0, LOW);
              if (TwoWire_Safe()) {
                TwoWire_Send();
              }
              break;

            case 4:
              TwoWire_CheckConnectionInterval(I, 1);
              TwoWire_CompileMessage(TwoWire_G_SlaveIP[I], TwoWire_Command(1, 0, 0), 0, 0, LOW);
              if (TwoWire_Safe()) {
                TwoWire_Send();
              }
              break;

            default:
              //ERROR
              break;
          }
        }
      }
    }
    else {

        if (TwoWire_Safe()) {
          TwoWire_CompileMessage(TwoWire_G_SlaveIP[I], TwoWire_Command(0, 0, 0), 0, 0, LOW);
          if(TwoWire_Safe()){
          TwoWire_Send();
          }
          if (TwoWire_Read(HIGH, LOW)) {
            if (TwoWire_CheckMessage(I) == 4) {
              TwoWire_CompileMessage(TwoWire_G_SlaveIP[I], TwoWire_Command(1, 0, 0), 0, 0, LOW);
              TwoWire_Send();
              TwoWire_CheckConnectionInterval(I, 1);
              Serial.print(TwoWire_G_SlaveIP[I]);
              Serial.println(" Connected");
            }
          }
        }
    }
  }
}

bool TwoWire_CheckConnectionInterval(int TwoWire_SlaveIP, bool TwoWire_Command) {
  if (TwoWire_Command) {
    TwoWire_SlaveTimer[TwoWire_SlaveIP] = micros();
    TwoWire_SlaveConnected[TwoWire_SlaveIP] = true;
    return true;
  }
  else {
    if (TwoWire_SlaveConnected[TwoWire_SlaveIP]) {
      if ((micros() - TwoWire_SlaveTimer[TwoWire_SlaveIP]) > 3000000) {
        TwoWire_SlaveConnected[TwoWire_SlaveIP] = false;
        TwoWire_BufferFlush(TwoWire_SlaveIP);
        Serial.print(TwoWire_G_SlaveIP[TwoWire_SlaveIP]);
              Serial.println(" Disconnected");
        return false;
      }
      else {
        TwoWire_SlaveConnected[TwoWire_SlaveIP] = true;
        return true;
      }
    }
    else {
      return false;
    }
  }
}

bool TwoWire_Safe() {
  uint32_t TwoWire_Timer = micros();
  while ((micros() - TwoWire_Timer) < 60) {
    if (!TwoWire_PinRead(0) or !TwoWire_PinRead(1)) {
      return false;
    }
  }
  return true;
}
void TwoWire_Send() {
  TwoWire_BitSend();
}

void TwoWire_BitSend() {
  const uint8_t TwoWire_Interval = 20;
  uint8_t TwoWire_PacketLength;
  uint8_t TwoWire_BitCount = 0;
  uint32_t TwoWire_Timer = 0;
  bool TwoWire_PulseState = HIGH;
  bool TwoWire_PulseEnable = false;
  if (TwoWire_TX_Buffer[20]) {
    TwoWire_PacketLength = 20;
  }
  else {
    TwoWire_PacketLength = 9;
  }
  TwoWire_PinSet(0, 1);
  TwoWire_PinSet(1, 1);
  while (1) {
    if ((micros() - TwoWire_Timer) >= TwoWire_Interval) {
      if (TwoWire_PulseEnable) {
        TwoWire_PinWrite(1, !TwoWire_PulseState);
        TwoWire_PulseState = !TwoWire_PulseState;
      }
      else {
        TwoWire_PulseEnable = true;
      }
      if (TwoWire_PulseState) {
        if (TwoWire_BitCount < TwoWire_PacketLength) {
          TwoWire_PinWrite(0, TwoWire_TX_Buffer[TwoWire_BitCount]);
          TwoWire_BitCount++;
        }
        else {
          TwoWire_PinSet(0, 0);
          TwoWire_PinSet(1, 0);
          return;
        }
      }
      TwoWire_Timer = micros();
    }
  }
}

bool TwoWire_Read(bool TwoWire_LengthOne, bool TwoWire_LenghtTwo) {
  uint32_t TwoWire_Timer = micros();
  while (TwoWire_PinRead(1) and (micros() - TwoWire_Timer) < 100) {}
  if (!TwoWire_PinRead(1)) {
    if (TwoWire_BitRead(TwoWire_LengthOne, TwoWire_LenghtTwo)) {
      return true;
    }
  }
  return false;
}

bool TwoWire_BitRead(bool TwoWire_LengthOne, bool TwoWire_LenghtTwo) {
  const uint8_t TwoWire_TimerInterval = 60;
  uint8_t TwoWire_MaxPacketLength;
  bool TwoWire_RX_TempBuffer[21];
  unsigned long int TwoWire_Timer;
  uint8_t TwoWire_PacketCount;
  bool TwoWire_Done = false;
  if (TwoWire_LengthOne or TwoWire_LenghtTwo) {
    if (TwoWire_LenghtTwo) {
      TwoWire_MaxPacketLength = 20;
    }
    else {
      TwoWire_MaxPacketLength = 9;
    }
  }
  else {
    return false;
  }
  for (TwoWire_PacketCount = 0; TwoWire_PacketCount <= TwoWire_MaxPacketLength and !TwoWire_Done; TwoWire_PacketCount++) {
    TwoWire_RX_TempBuffer[TwoWire_PacketCount] = TwoWire_PinRead(0);
    TwoWire_Timer = micros();
    while (!TwoWire_PinRead(1) and !TwoWire_Done) {
      if ((micros() - TwoWire_Timer) > TwoWire_TimerInterval) {
        TwoWire_Done = true;
      }
    }
    while (TwoWire_PinRead(1) and !TwoWire_Done) {
      if ((micros() - TwoWire_Timer) > TwoWire_TimerInterval) {
        TwoWire_Done = true;
      }
    }
  }
  if ((TwoWire_LengthOne and TwoWire_PacketCount == 9) or (TwoWire_LenghtTwo and TwoWire_PacketCount == 20)) {
    for (int X = 0; X < 3; X++) {
      bitWrite(TwoWire_RX_Buffer[0], X , TwoWire_RX_TempBuffer[X]);
    }
    for (int X = 0; X < 3; X++) {
      bitWrite(TwoWire_RX_Buffer[1], X , TwoWire_RX_TempBuffer[X + 3]);
    }
    for (int X = 0; X < 3; X++) {
      bitWrite(TwoWire_RX_Buffer[2], X , TwoWire_RX_TempBuffer[X + 6]);
    }
    if (TwoWire_PacketCount == 20) {
      for (int X = 0; X < 3; X++) {
        bitWrite(TwoWire_RX_Buffer[3], X , TwoWire_RX_TempBuffer[X + 9]);
      }
      for (int X = 0; X < 8; X++) {
        bitWrite(TwoWire_RX_Buffer[4], X , TwoWire_RX_TempBuffer[X + 12]);
      }
    }
    return true;
  }
  else {
    return false;
  }
}

int8_t TwoWire_CheckMessage(uint8_t I) {  //Check command bit of recieved message from slave device
  if (TwoWire_RX_Buffer[0] == TwoWire_G_MasterIP and TwoWire_RX_Buffer[1] == TwoWire_G_SlaveIP[I]) {
    switch (TwoWire_RX_Buffer[2]) {
      case 1:
        return TwoWire_RX_Buffer[2];
        break;

      case 2:
        return TwoWire_RX_Buffer[2];
        break;

      case 4:
        return TwoWire_RX_Buffer[2];
        break;

      default:
        return -1;
        break;
    }
  }
  return -1;
}

boolean BufferRX(uint8_t TwoWire_SlaveIP) {
  if(TwoWire_SlaveConnected[TwoWire_SlaveIP]){
  if (CheckBuffer(TwoWire_SlaveIP)) {
    if (TwoWire_Buffer[TwoWire_SlaveIP][TwoWire_G_BufferSize][0] < (TwoWire_G_BufferSize - 1)) {
      TwoWire_Buffer[TwoWire_SlaveIP][TwoWire_G_BufferSize][0]++;
    }
    else {
      TwoWire_Buffer[TwoWire_SlaveIP][TwoWire_G_BufferSize][0] = 0;
    }
    TwoWire_BufferSend[0] = TwoWire_Buffer[TwoWire_SlaveIP][TwoWire_Buffer[TwoWire_SlaveIP][TwoWire_G_BufferSize][0]][0];
    TwoWire_BufferSend[1] = TwoWire_Buffer[TwoWire_SlaveIP][TwoWire_Buffer[TwoWire_SlaveIP][TwoWire_G_BufferSize][0]][1];
    return true;
  }
  else {
    TwoWire_BufferSend[0] = 0;
    TwoWire_BufferSend[1] = 0;
    return false;
  }
  }
  else{
    return false;
  }
}

boolean CheckBuffer(uint8_t TwoWire_SlaveIP) {  //Checks if anythink available in buffer of specific IP
  if (TwoWire_Buffer[TwoWire_SlaveIP][TwoWire_G_BufferSize][0] == TwoWire_Buffer[TwoWire_SlaveIP][TwoWire_G_BufferSize][1]) {
    return false;
  }
  else {
    return true;
  }
}

void TwoWire_CompileMessage(uint8_t TwoWire_TX_IP, uint8_t TwoWire_COM, uint8_t TwoWire_MSG_IP, uint8_t TwoWire_MSG_DATA, bool TwoWire_Length) {
  for (uint8_t X = 0; X < 3; X++) {
    TwoWire_TX_Buffer[X] = bitRead(TwoWire_TX_IP, X);
  }
  for (uint8_t X = 0; X < 3; X++) {
    TwoWire_TX_Buffer[(X + 3)] = bitRead(TwoWire_G_MasterIP, X);
  }
  for (uint8_t X = 0; X < 3; X++) {
    TwoWire_TX_Buffer[(X + 6)] = bitRead(TwoWire_COM, X);
  }
  if (TwoWire_Length) {
    for (uint8_t X = 0; X < 3; X++) {
      TwoWire_TX_Buffer[(X + 9)] = bitRead(TwoWire_MSG_IP, X);
    }
    for (uint8_t X = 0; X < 8; X++) {
      TwoWire_TX_Buffer[(X + 12)] = bitRead(TwoWire_MSG_DATA, X);
    }
  }
  TwoWire_TX_Buffer[20] = TwoWire_Length;
}

uint8_t TwoWire_Command (boolean TwoWire_X, boolean TwoWire_Y, boolean TwoWire_Z) {
  uint8_t TwoWire_Final = TwoWire_X;
  TwoWire_Final += TwoWire_Y << 1;
  TwoWire_Final += TwoWire_Z << 2;
  return TwoWire_Final;
}

void TwoWire_BufferFlush(uint8_t TwoWire_SlaveIP) {

    TwoWire_Buffer[TwoWire_SlaveIP][TwoWire_G_BufferSize][0] = 0;
    TwoWire_Buffer[TwoWire_SlaveIP][TwoWire_G_BufferSize][1] = 0;

  return;
}

boolean TwoWire_BufferStore(uint8_t TwoWire_SenderIP, uint8_t TwoWire_RecieverIP, uint8_t TwoWire_Data) {  //Check if IP valid and if there is space in buffer for it then stores it in
  boolean TwoWire_FoundIP = false;
  uint8_t TwoWire_SlaveIP_Result = 0;
  for ( uint8_t I = 0; I < TwoWire_G_SlaveIPCount and TwoWire_FoundIP == false; I++) {
    if (TwoWire_RecieverIP == TwoWire_G_SlaveIP[I]) {
      TwoWire_FoundIP = true;
      TwoWire_SlaveIP_Result = I;
    }
  }
  if (TwoWire_FoundIP == true) {
    if (TwoWire_SlaveConnected[TwoWire_SlaveIP_Result]) {
      uint8_t TwoWire_BufferTest = TwoWire_Buffer[TwoWire_SlaveIP_Result][TwoWire_G_BufferSize][1];
      if (TwoWire_Buffer[TwoWire_SlaveIP_Result][TwoWire_G_BufferSize][1] == (TwoWire_G_BufferSize - 1)) {
        TwoWire_BufferTest = 0;
      }
      else {
        TwoWire_BufferTest++;
      }
      if (TwoWire_BufferTest == TwoWire_Buffer[TwoWire_SlaveIP_Result][TwoWire_G_BufferSize][0]) {
        return false;
      }
      else {
        TwoWire_Buffer[TwoWire_SlaveIP_Result][TwoWire_G_BufferSize][1] = TwoWire_BufferTest;
        TwoWire_Buffer[TwoWire_SlaveIP_Result][TwoWire_Buffer[TwoWire_SlaveIP_Result][TwoWire_G_BufferSize][1]][0] = TwoWire_G_SlaveIP[TwoWire_SenderIP];
        TwoWire_Buffer[TwoWire_SlaveIP_Result][TwoWire_Buffer[TwoWire_SlaveIP_Result][TwoWire_G_BufferSize][1]][1] = TwoWire_Data;
        return true;
      }
    }
    else {
      return false;
    }
  }
  else {
    return false;
  }
}

bool TwoWire_PinRead(bool TwoWire_PIN) {
  if (TwoWire_PIN) {
    return bitRead(PINB, 3);
  }
  else {
    return bitRead(PINB, 2);
  }
}

void TwoWire_PinWrite(bool TwoWire_PIN, bool TwoWire_DIR) {
  if (TwoWire_PIN) {
    if (TwoWire_DIR) {
      PORTB = PORTB | B00001000;
    }
    else {
      PORTB = PORTB & B11110111;
    }
  }
  else {
    if (TwoWire_DIR) {
      PORTB = PORTB | B00000100;
    }
    else {
      PORTB = PORTB & B11111011;
    }
  }
}

void TwoWire_PinSet(bool TwoWire_PIN, bool TwoWire_DIR) {
  if (TwoWire_PIN) {
    if (TwoWire_DIR) {
      DDRB = DDRB | B00001000;
    }
    else {
      PORTB = PORTB | B00001000;
      DDRB = DDRB & B11110111;
    }
  }
  else {
    if (TwoWire_DIR) {
      DDRB = DDRB | B00000100;
    }
    else {
      PORTB = PORTB | B00000100;
      DDRB = DDRB & B11111011;
    }
  }
}

//Master ▲▲▲






//Slave ▼▼▼
const uint8_t TwoWire_SlaveIP = 3;
const uint8_t TwoWire_MasterIP = 1;

uint32_t TwoWire_ConnectionTimer = 0;
uint8_t TwoWire_RX_BitBuffer[5];
bool TwoWire_TX_BitBuffer[21];
bool TwoWire_Connected = false;

uint8_t TwoWire_TX_Buffer[2];
uint8_t TwoWire_RX_Buffer[2];

void setup() {
  TwoWire_TX_Buffer[0] = 4;
  TwoWire_PinSet(0, 0);
  TwoWire_PinSet(1, 0);
  pinMode(13,OUTPUT);
}

void loop() {
  Check_Interval();
  if(TwoWire_Run(1)){
    digitalWrite(13,HIGH);
  }
  else{
    digitalWrite(13,LOW);
  }
  TwoWire_Run(0);
}

void Check_Interval() {
    if ((micros() - TwoWire_ConnectionTimer) > 1000000) {
      TwoWire_CompileMessage(4, 0, 0, false);
      if (TwoWire_ParsePacket(1, 0, 1)) {
          TwoWire_Send();
          if (TwoWire_Read(1, 0)) {
            if (TwoWire_CheckIP()) {
              if (bitRead(TwoWire_RX_BitBuffer[2], 0)) {
                TwoWire_Connected = true;
                TwoWire_ConnectionTimer = micros();
              }
              else {
                TwoWire_Connected = false;
              }
            }
          }
        
        else {
          TwoWire_Connected = false;
        }
      }
    }
}

bool TwoWire_Connect() {
  if (!TwoWire_Connected) {
    TwoWire_CompileMessage(4, 0, 0, false);
    if (TwoWire_ParsePacket(1, 0, 1)) {
      if (!bitRead(TwoWire_RX_BitBuffer[2], 0)) {
        TwoWire_Send();
        if (TwoWire_Read(1, 0)) {
          if (TwoWire_CheckIP()) {
            if (bitRead(TwoWire_RX_BitBuffer[2], 0)) {
              TwoWire_Connected = true;
              return true;
            }
          }
        }
      }
    }
    return false;
  }
  return true;
}

bool TwoWire_Run(bool TwoWire_Command) {
  if (TwoWire_Connected) {
    if (!TwoWire_Command) {
      TwoWire_CompileMessage(1, 0, 0, false);
      if (TwoWire_ParsePacket(1, 0, 0)) {
        if (bitRead(TwoWire_RX_BitBuffer[2], 0)) {
          if (bitRead(TwoWire_RX_BitBuffer[2], 1)) {
            TwoWire_Send();
            if (TwoWire_Read(1, 1)) {
              if (TwoWire_CheckIP()) {
                if (bitRead(TwoWire_RX_BitBuffer[2], 1)) {
                  TwoWire_RX_Buffer[0] = TwoWire_RX_BitBuffer[3];
                  TwoWire_RX_Buffer[1] = TwoWire_RX_BitBuffer[4];
                  return true;
                }
              }
            }
          }
        }
        else {
          TwoWire_Connected = false;
          return false;
        }
      }
    }
    else {
      TwoWire_CompileMessage(2, TwoWire_TX_Buffer[0], TwoWire_TX_Buffer[1], HIGH);
      if (TwoWire_ParsePacket(1, 0, 0)) {
        if (bitRead(TwoWire_RX_BitBuffer[2], 0)) {
          TwoWire_Send();
          if (TwoWire_Read(1, 0)) {
            if (TwoWire_CheckIP()) {
              if (bitRead(TwoWire_RX_BitBuffer[2], 2)) {
                return true;
              }
            }
          }
        }
        else {
          TwoWire_Connected = false;
          return false;
        }
      }
    }
  }
  return false;
}

bool TwoWire_ParsePacket(bool TwoWire_LengthOne, bool TwoWire_LengthTwo, bool TwoWire_TimerLength) {
  uint32_t TwoWire_Timer = micros();
  uint32_t TwoWire_Interval;
  if (TwoWire_TimerLength) {
    TwoWire_Interval = 5000000;
  }
  else {
    TwoWire_Interval = 20000;
  }
  while ((micros() - TwoWire_Timer) < TwoWire_Interval) {
    if (TwoWire_Insert()) {
      if (TwoWire_Read(TwoWire_LengthOne, TwoWire_LengthTwo)) {
        if (TwoWire_CheckIP()) {
          return true;
        }
      }
    }
  }
  return false;
}

bool TwoWire_Insert() {
  uint32_t TwoWire_Timer = micros();
  while ((micros() - TwoWire_Timer) < 80) {
    if (!TwoWire_PinRead(1)) {
      return false;
    }
  }
  return true;
}

bool TwoWire_Read(bool TwoWire_Length_One, bool TwoWire_Length_Two) {
  uint32_t TwoWire_Timer = micros();
  while ((micros() - TwoWire_Timer) < 300) {
    if (!TwoWire_PinRead(1)) {
      return TwoWire_BitRead(TwoWire_Length_One, TwoWire_Length_Two);
    }
  }
  TwoWire_Connected = false;
  return false;
}

bool TwoWire_BitRead(bool TwoWire_Length_One, bool TwoWire_Length_Two) {
  const uint8_t TwoWire_TimerInterval = 60;
  uint8_t TwoWire_MaxPacketLength;
  bool TwoWire_RX_TempBuffer[21];
  uint32_t TwoWire_Timer;
  uint8_t TwoWire_PacketCount;
  bool TwoWire_Done = false;
  if (TwoWire_Length_One or TwoWire_Length_Two) {
    if (TwoWire_Length_One) {
      TwoWire_MaxPacketLength = 20;
    }
    else {
      TwoWire_MaxPacketLength = 9;
    }
  }
  else {
    return false;
  }
  for (TwoWire_PacketCount = 0; TwoWire_PacketCount <= TwoWire_MaxPacketLength and !TwoWire_Done; TwoWire_PacketCount++) {
    TwoWire_RX_TempBuffer[TwoWire_PacketCount] = TwoWire_PinRead(0);
    TwoWire_Timer = micros();
    while (!TwoWire_PinRead(1) and !TwoWire_Done) {
      if ((micros() - TwoWire_Timer) > TwoWire_TimerInterval) {
        TwoWire_Done = true;
      }
    }
    while (TwoWire_PinRead(1) and !TwoWire_Done) {
      if ((micros() - TwoWire_Timer) > TwoWire_TimerInterval) {
        TwoWire_Done = true;
      }
    }
  }

  if ((TwoWire_Length_One and TwoWire_PacketCount == 9) or (TwoWire_Length_Two and TwoWire_PacketCount == 20)) {
    for (int X = 0; X < 3; X++) {
      bitWrite(TwoWire_RX_BitBuffer[0], X , TwoWire_RX_TempBuffer[X]);
    }
    for (int X = 0; X < 3; X++) {
      bitWrite(TwoWire_RX_BitBuffer[1], X , TwoWire_RX_TempBuffer[X + 3]);
    }
    for (int X = 0; X < 3; X++) {
      bitWrite(TwoWire_RX_BitBuffer[2], X , TwoWire_RX_TempBuffer[X + 6]);
    }
    if (TwoWire_PacketCount == 20) {
      for (int X = 0; X < 3; X++) {
        bitWrite(TwoWire_RX_BitBuffer[3], X , TwoWire_RX_TempBuffer[X + 9]);
      }
      for (int X = 0; X < 8; X++) {
        bitWrite(TwoWire_RX_BitBuffer[4], X , TwoWire_RX_TempBuffer[X + 12]);
      }
    }
    return true;
  }
  else {
    return false;
  }
}

void TwoWire_Send() {
  TwoWire_BitSend();
}

void TwoWire_BitSend() {
  const uint8_t TwoWire_Interval = 20;
  uint8_t TwoWire_PacketLength;
  uint8_t TwoWire_BitCount = 0;
  uint32_t TwoWire_Timer = 0;
  bool TwoWire_PulseState = HIGH;
  bool TwoWire_PulseEnable = false;
  if (TwoWire_TX_BitBuffer[20]) {
    TwoWire_PacketLength = 20;
  }
  else {
    TwoWire_PacketLength = 9;
  }
  TwoWire_PinSet(0, 1);
  TwoWire_PinSet(1, 1);
  while (1) {
    if ((micros() - TwoWire_Timer) >= TwoWire_Interval) {
      if (TwoWire_PulseEnable) {
        TwoWire_PinWrite(1, !TwoWire_PulseState);
        TwoWire_PulseState = !TwoWire_PulseState;
      }
      else {
        TwoWire_PulseEnable = true;
      }
      if (TwoWire_PulseState) {
        if (TwoWire_BitCount < TwoWire_PacketLength) {
          TwoWire_PinWrite(0, TwoWire_TX_BitBuffer[TwoWire_BitCount]);
          TwoWire_BitCount++;
        }
        else {
          TwoWire_PinSet(0, 0);
          TwoWire_PinSet(1, 0);
          return;
        }
      }
      TwoWire_Timer = micros();
    }
  }
}


bool TwoWire_CheckIP() {
  if (TwoWire_RX_BitBuffer[0] == TwoWire_SlaveIP and TwoWire_RX_BitBuffer[1] == TwoWire_MasterIP) {
    return true;
  }
  return false;
}

void TwoWire_CompileMessage(uint8_t TwoWire_COM, uint8_t TwoWire_MSG_IP, uint8_t TwoWire_MSG_DATA, bool TwoWire_Length) {
  for (uint8_t X = 0; X < 3; X++) {
    TwoWire_TX_BitBuffer[X] = bitRead(TwoWire_MasterIP, X);
  }
  for (uint8_t X = 0; X < 3; X++) {
    TwoWire_TX_BitBuffer[(X + 3)] = bitRead(TwoWire_SlaveIP, X);
  }
  for (uint8_t X = 0; X < 3; X++) {
    TwoWire_TX_BitBuffer[(X + 6)] = bitRead(TwoWire_COM, X);
  }
  if (TwoWire_Length) {
    for (uint8_t X = 0; X < 3; X++) {
      TwoWire_TX_BitBuffer[(X + 9)] = bitRead(TwoWire_MSG_IP, X);
    }
    for (uint8_t X = 0; X < 8; X++) {
      TwoWire_TX_BitBuffer[(X + 12)] = bitRead(TwoWire_MSG_DATA, X);
    }
  }
  TwoWire_TX_BitBuffer[20] = TwoWire_Length;
}

bool TwoWire_PinRead(bool TwoWire_PIN) {
  if (TwoWire_PIN) {
    return bitRead(PINB, 3);
  }
  else {
    return bitRead(PINB, 2);
  }
}

void TwoWire_PinWrite(bool TwoWire_PIN, bool TwoWire_DIR) {
  if (TwoWire_PIN) {
    if (TwoWire_DIR) {
      PORTB = PORTB | B00001000;
    }
    else {
      PORTB = PORTB & B11110111;
    }
  }
  else {
    if (TwoWire_DIR) {
      PORTB = PORTB | B00000100;
    }
    else {
      PORTB = PORTB & B11111011;
    }
  }
}

void TwoWire_PinSet(bool TwoWire_PIN, bool TwoWire_DIR) {
  if (TwoWire_PIN) {
    if (TwoWire_DIR) {
      DDRB = DDRB | B00001000;
    }
    else {
      PORTB = PORTB | B00001000;
      DDRB = DDRB & B11110111;
    }
  }
  else {
    if (TwoWire_DIR) {
      DDRB = DDRB | B00000100;
    }
    else {
      PORTB = PORTB | B00000100;
      DDRB = DDRB & B11111011;
    }
  }
}
//Slave ▲▲▲
