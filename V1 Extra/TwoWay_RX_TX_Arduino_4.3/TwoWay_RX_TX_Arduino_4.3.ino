//Master ▼▼▼

uint8_t const TwoWire_G_MasterIP = 0;
uint8_t const TwoWire_G_SlaveIP[] = {1, 2, 3, 4, 5};
uint8_t const TwoWire_G_SlaveIPCount = (sizeof(TwoWire_G_SlaveIP));
uint8_t const TwoWire_G_BufferSize = 6;

bool TwoWire_G_TX_Buffer[21];
uint8_t TwoWire_G_RX_Buffer[5];

bool TwoWire_G_SlaveConnected[TwoWire_G_SlaveIPCount];
uint8_t TwoWire_G_Buffer[TwoWire_G_SlaveIPCount][((TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 2 + TwoWire_G_SlaveIPCount)][2];
uint32_t TwoWire_G_SlaveTimer[TwoWire_G_SlaveIPCount];

uint8_t TwoWire_G_BufferSend[2];

bool TwoWire_G_TXStatus[2];

void setup() {
  TwoWireQ_PinSet(0, 0);
  TwoWireQ_PinSet(1, 0);
}

void loop() {
  TwoWireA_Run();
}

void TwoWireA_Run() {
  for (uint8_t TwoWireA_T_A = 0; TwoWireA_T_A < TwoWire_G_SlaveIPCount; TwoWireA_T_A++) {
    if (TwoWireB_CheckConnectionInterval(TwoWireA_T_A, 0)) {
      if (TwoWireC_Safe()) {
        TwoWireL_CompileMessage(TwoWire_G_SlaveIP[TwoWireA_T_A], TwoWireM_Command(1, TwoWireI_CheckBuffer(TwoWireA_T_A), 0), 0, 0, LOW);
        TwoWireF_Send();
        if (TwoWireD_Read(HIGH, HIGH)) {
          switch (TwoWireH_CheckMessage(TwoWireA_T_A)) {
              bool TwoWireA_T_CheckBuffer = false;
            case 1:
              TwoWireA_T_CheckBuffer = TwoWireJ_BufferRead(TwoWireA_T_A);
              TwoWireL_CompileMessage(TwoWire_G_SlaveIP[TwoWireA_T_A], TwoWireM_Command(1, TwoWireA_T_CheckBuffer, 0), TwoWire_G_BufferSend[0], TwoWire_G_BufferSend[1], TwoWireA_T_CheckBuffer);
              if (TwoWireC_Safe()) TwoWireF_Send();
              TwoWireB_CheckConnectionInterval(TwoWireA_T_A, 1);
              break;
            case 2:
              TwoWireK_BufferStore(TwoWireA_T_A, TwoWire_G_RX_Buffer[3], TwoWire_G_RX_Buffer[4]);
              TwoWireL_CompileMessage(TwoWire_G_SlaveIP[TwoWireA_T_A], TwoWireM_Command(1, !TwoWire_G_TXStatus[0], TwoWire_G_TXStatus[1]), 0, 0, LOW);
              if (TwoWireC_Safe()) TwoWireF_Send();
              TwoWireB_CheckConnectionInterval(TwoWireA_T_A, 1);
              break;
            case 4:
              TwoWireL_CompileMessage(TwoWire_G_SlaveIP[TwoWireA_T_A], TwoWireM_Command(1, 0, 0), 0, 0, LOW);
              if (TwoWireC_Safe()) TwoWireF_Send();
              TwoWireB_CheckConnectionInterval(TwoWireA_T_A, 1);
              break;
            default:
              break;
          }
        }
      }
    }
    else {
      TwoWireL_CompileMessage(TwoWire_G_SlaveIP[TwoWireA_T_A], 0, 0, 0, LOW);
      if (TwoWireC_Safe()) {
        TwoWireF_Send();
        if (TwoWireD_Read(HIGH, LOW)) {
          if (TwoWireH_CheckMessage(TwoWireA_T_A) == 4) {
            TwoWireL_CompileMessage(TwoWire_G_SlaveIP[TwoWireA_T_A], TwoWireM_Command(1, 0, 0), 0, 0, LOW);
            TwoWireF_Send();
            TwoWireB_CheckConnectionInterval(TwoWireA_T_A, 1);
          }
        }
      }
    }
  }
}

bool TwoWireB_CheckConnectionInterval(uint8_t TwoWireB_T_SlaveIP, bool TwoWireB_T_Command) {
  if (TwoWireB_T_Command) {
    TwoWire_G_SlaveTimer[TwoWireB_T_SlaveIP] = millis();
    TwoWire_G_SlaveConnected[TwoWireB_T_SlaveIP] = true;
    return true;
  }
  else {
    if (TwoWire_G_SlaveConnected[TwoWireB_T_SlaveIP]) {
      if ((millis() - TwoWire_G_SlaveTimer[TwoWireB_T_SlaveIP]) > 3000) {
        TwoWire_G_SlaveConnected[TwoWireB_T_SlaveIP] = false;
        TwoWireN_BufferFlush(TwoWireB_T_SlaveIP);
        return false;
      }
      else return true;
    }
    return false;
  }
}

bool TwoWireC_Safe() {
  uint32_t TwoWireC_T_Timer = micros();
  while ((micros() - TwoWireC_T_Timer) < 60) {
    if (!TwoWireO_PinRead(0) or !TwoWireO_PinRead(1)) return false;
  }
  return true;
}

bool TwoWireD_Read(bool TwoWireD_T_LengthOne, bool TwoWireD_T_LengthTwo) {
  uint32_t TwoWireD_T_Timer = micros();
  while (TwoWireO_PinRead(1) and (micros() - TwoWireD_T_Timer) < 100) {}
  if (!TwoWireO_PinRead(1)) {
    if (TwoWireE_BitRead(TwoWireD_T_LengthOne, TwoWireD_T_LengthTwo)) return true;
  }
  return false;
}

bool TwoWireE_BitRead(bool TwoWireE_T_LengthOne, bool TwoWireE_T_LengthTwo) {
  uint8_t TwoWireE_T_MaxPacketLength = 0;
  bool TwoWireE_T_RX_TempBuffer[21];
  uint32_t TwoWireE_T_Timer = 0;
  uint8_t TwoWireE_T_PacketCount = 0;
  bool TwoWireE_T_Done = false;
  if(TwoWireE_T_LengthOne or TwoWireE_T_LengthTwo){
    if(TwoWireE_T_LengthTwo) TwoWireE_T_MaxPacketLength = 20;
    else TwoWireE_T_MaxPacketLength = 9;
  }
  return false;
  for(TwoWireE_T_PacketCount = 0; TwoWireE_T_PacketCount <= TwoWireE_T_MaxPacketLength and !TwoWireE_T_Done; TwoWireE_T_PacketCount++){
    TwoWireE_T_RX_TempBuffer[TwoWireE_T_PacketCount] = TwoWireO_PinRead(0);
    TwoWireE_T_Timer = micros();
    while(!TwoWireO_PinRead(1) and !TwoWireE_T_Done){
      if((micros() - TwoWireE_T_Timer) > 60) TwoWireE_T_Done = true;
    }
    while(TwoWireO_PinRead(1) and !TwoWireE_T_Done){
      if((micros() - TwoWireE_T_Timer) > 60) TwoWireE_T_Done = true;
    }
  }
  if((TwoWireE_T_LengthOne and TwoWireE_T_PacketCount == 9) or (TwoWireE_T_LengthTwo and TwoWireE_T_PacketCount == 20)){
    for(uint8_t TwoWireE_T_A = 0; TwoWireE_T_A < 3; TwoWireE_T_A++){
      bitWrite(TwoWire_G_RX_Buffer[0], TwoWireE_T_A, TwoWireE_T_RX_TempBuffer[TwoWireE_T_A]);
    }
    for(uint8_t TwoWireE_T_B = 0; TwoWireE_T_B < 3; TwoWireE_T_B++){
      bitWrite(TwoWire_G_RX_Buffer[1], TwoWireE_T_B, TwoWireE_T_RX_TempBuffer[TwoWireE_T_B]);
    }
    for(uint8_t TwoWireE_T_C = 0; TwoWireE_T_C < 3; TwoWireE_T_C++){
      bitWrite(TwoWire_G_RX_Buffer[2], TwoWireE_T_C, TwoWireE_T_RX_TempBuffer[TwoWireE_T_C]);
    }
    if(TwoWireE_T_PacketCount == 20){
    for(uint8_t TwoWireE_T_D = 0; TwoWireE_T_D < 3; TwoWireE_T_D++){
      bitWrite(TwoWire_G_RX_Buffer[3], TwoWireE_T_D, TwoWireE_T_RX_TempBuffer[TwoWireE_T_D]);
    }
    for(uint8_t TwoWireE_T_E = 0; TwoWireE_T_E < 8; TwoWireE_T_E++){
      bitWrite(TwoWire_G_RX_Buffer[4], TwoWireE_T_E, TwoWireE_T_RX_TempBuffer[TwoWireE_T_E]);
    }
    }
    return true;
  }
  return false;
}

void TwoWireF_Send() {
  TwoWireG_BitSend();
}

void TwoWireG_BitSend() {
  uint8_t TwoWireG_T_PacketLength = 0;
  uint8_t TwoWireG_T_BitCount = 0;
  uint32_t TwoWireG_T_Timer = 0;
  bool TwoWireG_T_PulseState = HIGH;
  bool TwoWireG_T_PulseEnable = false;
  if(TwoWire_G_TX_Buffer[20]) TwoWireG_T_PacketLength = 20;
  else TwoWireG_T_PacketLength = 9;
  TwoWireQ_PinSet(0,1);
  TwoWireQ_PinSet(1,1);
  while(1){
    if((micros() - TwoWireG_T_Timer) >= 20){
      if(TwoWireG_T_PulseEnable){
        TwoWireP_PinWrite(1, !TwoWireG_T_PulseState);
        TwoWireG_T_PulseState = !TwoWireG_T_PulseState;
      }
      else TwoWireG_T_PulseEnable = true;
      if(TwoWireG_T_PulseState){
        if(TwoWireG_T_BitCount < TwoWireG_T_PacketLength){
          TwoWireP_PinWrite(0, TwoWire_G_TX_Buffer[TwoWireG_T_BitCount]);
          TwoWireG_T_BitCount++;
        }
        else{
          TwoWireQ_PinSet(0,0);
          TwoWireQ_PinSet(1,0);
          return;
        }
      }
      TwoWireG_T_Timer = micros();
    }
  }
}

int8_t TwoWireH_CheckMessage(uint8_t TwoWireH_T_SlaveIP) {
  if (TwoWire_G_RX_Buffer[0] == TwoWire_G_MasterIP and TwoWire_G_RX_Buffer[1] == TwoWire_G_SlaveIP[TwoWireH_T_SlaveIP]) {
    switch (TwoWire_G_RX_Buffer[2]) {
      case 1:
        return TwoWire_G_RX_Buffer[2];
        break;
      case 2:
        return TwoWire_G_RX_Buffer[2];
        break;
      case 4:
        return TwoWire_G_RX_Buffer[2];
        break;
      default:
        return -1;
        break;
    }
  }
}

bool TwoWireI_CheckBuffer(uint8_t TwoWireI_T_SlaveIP) {
  if(TwoWire_G_Buffer[TwoWireI_T_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0] == TwoWire_G_Buffer[TwoWireI_T_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][1]) return false;
  else return true;
}

bool TwoWireJ_BufferRead(uint8_t TwoWireJ_T_SlaveIP) {
  if(TwoWireI_CheckBuffer(TwoWireJ_T_SlaveIP)){
    if(TwoWire_G_Buffer[TwoWireJ_T_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0] < TwoWire_G_BufferSize)
    TwoWire_G_Buffer[TwoWireJ_T_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0]++;
    else
    TwoWire_G_Buffer[TwoWireJ_T_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0] = 0;
    TwoWire_G_BufferSend[0] = TwoWire_G_SlaveIP[TwoWire_G_Buffer[TwoWireJ_T_SlaveIP][TwoWire_G_Buffer[TwoWireJ_T_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0]][0]];
    TwoWire_G_BufferSend[1] = TwoWire_G_Buffer[TwoWireJ_T_SlaveIP][TwoWire_G_Buffer[TwoWireJ_T_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0]][1];
    TwoWire_G_Buffer[TwoWireJ_T_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 2 + TwoWire_G_Buffer[TwoWireJ_T_SlaveIP][TwoWire_G_Buffer[TwoWireJ_T_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0]][0]][0]--;
    return true;
  }
  TwoWire_G_BufferSend[0] = 0;
  TwoWire_G_BufferSend[1] = 0;
  return false;
}

void TwoWireK_BufferStore(uint8_t TwoWireK_T_SenderIP, uint8_t TwoWireK_T_ReRecieverIP, uint8_t TwoWireK_T_Data) {
  bool TwoWireK_T_FoundIP = false;
  uint8_t TwoWireK_T_SlaveIP_Result = 0;
  TwoWire_G_TXStatus[0] = false;
  TwoWire_G_TXStatus[1] = false;
  for(uint8_t TwoWireK_T_A = 0; TwoWireK_T_A < TwoWire_G_SlaveIPCount and !TwoWireK_T_FoundIP; TwoWireK_T_A++){
    if(TwoWireK_T_ReRecieverIP == TwoWire_G_SlaveIP[TwoWireK_T_A]){
      TwoWireK_T_SlaveIP_Result = TwoWireK_T_A;
      TwoWireK_T_FoundIP = true;
    }
  }
  if(TwoWireK_T_FoundIP and TwoWire_G_SlaveConnected[TwoWireK_T_SlaveIP_Result]){
    TwoWire_G_TXStatus[0] = true;
    uint8_t TwoWireK_T_BufferTest = 0;
    if(TwoWire_G_Buffer[TwoWireK_T_SlaveIP_Result][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][1] == (TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize)) TwoWireK_T_BufferTest = 0;
    else TwoWireK_T_BufferTest++;
    if(TwoWireK_T_BufferTest == TwoWire_G_Buffer[TwoWireK_T_SlaveIP_Result][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0]) return;
    if(TwoWire_G_Buffer[TwoWireK_T_SlaveIP_Result][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 2 + TwoWireK_T_SenderIP][0] < TwoWire_G_BufferSize){
      TwoWire_G_Buffer[TwoWireK_T_SlaveIP_Result][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 2 + TwoWireK_T_SenderIP][0]++;
      TwoWire_G_Buffer[TwoWireK_T_SlaveIP_Result][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][1] = TwoWireK_T_BufferTest;
      TwoWire_G_Buffer[TwoWireK_T_SlaveIP_Result][TwoWire_G_Buffer[TwoWireK_T_SlaveIP_Result][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][1]][0] = TwoWireK_T_SenderIP;
      TwoWire_G_Buffer[TwoWireK_T_SlaveIP_Result][TwoWire_G_Buffer[TwoWireK_T_SlaveIP_Result][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][1]][1] = TwoWireK_T_Data;
      TwoWire_G_TXStatus[1] = true;
    }
  }
  return;
}

void TwoWireL_CompileMessage(uint8_t TwoWireL_T_RecieverIP, uint8_t TwoWireL_T_Command, uint8_t TwoWireL_T_MessageIP, uint8_t TwoWireL_T_MessageData, bool TwoWireL_T_PacketLength) {
  for (uint8_t TwoWireL_T_A = 0; TwoWireL_T_A < 3; TwoWireL_T_A++) {
    TwoWire_G_TX_Buffer[TwoWireL_T_A] = bitRead(TwoWireL_T_RecieverIP, TwoWireL_T_A);
  }
  for (uint8_t TwoWireL_T_B = 0; TwoWireL_T_B < 3; TwoWireL_T_B++) {
    TwoWire_G_TX_Buffer[TwoWireL_T_B + 3] = bitRead(TwoWire_G_MasterIP, TwoWireL_T_B);
  }
  for (uint8_t TwoWireL_T_C = 0; TwoWireL_T_C < 3; TwoWireL_T_C++) {
    TwoWire_G_TX_Buffer[TwoWireL_T_C + 6] = bitRead(TwoWireL_T_Command, TwoWireL_T_C);
  }
  if (TwoWireL_T_PacketLength) {
    for (uint8_t TwoWireL_T_D = 0; TwoWireL_T_D < 3; TwoWireL_T_D++) {
      TwoWire_G_TX_Buffer[TwoWireL_T_D + 9] = bitRead(TwoWireL_T_MessageIP, TwoWireL_T_D);
    }
    for (uint8_t TwoWireL_T_E = 0; TwoWireL_T_E < 8; TwoWireL_T_E++) {
      TwoWire_G_TX_Buffer[TwoWireL_T_E + 12] = bitRead(TwoWireL_T_MessageData, TwoWireL_T_E);
    }
  }
  TwoWire_G_TX_Buffer[20] = TwoWireL_T_PacketLength;
}

uint8_t TwoWireM_Command(bool TwoWireM_T_X, bool TwoWireM_T_Y, bool TwoWireM_T_Z) {
  uint8_t TwoWireM_T_Final = 0;
  bitWrite(TwoWireM_T_Final, 0, TwoWireM_T_X);
  bitWrite(TwoWireM_T_Final, 1, TwoWireM_T_Y);
  bitWrite(TwoWireM_T_Final, 2, TwoWireM_T_Z);
}

void TwoWireN_BufferFlush(uint8_t TwoWireN_T_SlaveIP) {
  TwoWire_G_Buffer[TwoWireN_T_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0] = 0;
  TwoWire_G_Buffer[TwoWireN_T_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][1] = 0;
  for(uint8_t TwoWireN_T_A = 0; TwoWireN_T_A < TwoWireN_T_SlaveIP; TwoWireN_T_A++){
    TwoWire_G_Buffer[TwoWireN_T_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 2 + TwoWireN_T_A][0] = 0;
  }
  return;
}

bool TwoWireO_PinRead(bool TwoWireO_T_PIN) {
  if (TwoWireO_T_PIN) {
    return bitRead(PINB, 0);
  }
  else {
    return bitRead(PIND, 7);
  }
}

void TwoWireP_PinWrite(bool TwoWireP_T_PIN, bool TwoWireP_T_DIR) {
  if (TwoWireP_T_PIN) {
    if (TwoWireP_T_DIR) {
      PORTB = PORTB | B00000001;
    }
    else {
      PORTB = PORTB & B11111110;
    }
  }
  else {
    if (TwoWireP_T_DIR) {
      PORTD = PORTD | B10000000;
    }
    else {
      PORTD = PORTD & B01111111;
    }
  }
}

void TwoWireQ_PinSet(bool TwoWireQ_T_PIN, bool TwoWireQ_T_DIR) {
  if (TwoWireQ_T_PIN) {
    if (TwoWireQ_T_DIR) {
      DDRB = DDRB | B00000001;
    }
    else {
      PORTB = PORTB | B00000001;
      DDRB = DDRB & B11111110;
    }
  }
  else {
    if (TwoWireQ_T_DIR) {
      DDRD = DDRD | B10000000;
    }
    else {
      PORTD = PORTD | B10000000;
      DDRD = DDRD & B01111111;
    }
  }
}
