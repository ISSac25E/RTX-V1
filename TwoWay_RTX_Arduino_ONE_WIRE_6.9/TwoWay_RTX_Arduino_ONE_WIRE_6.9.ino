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
  TwoWireQ_PinSet(0);
}

void loop() {
  TwoWireA_Run();
}

void TwoWireA_Run() {
  for (uint8_t TwoWireA_L_A = 0; TwoWireA_L_A < TwoWire_G_SlaveIPCount; TwoWireA_L_A++) {
    if (TwoWireB_CheckConnectionInterval(TwoWireA_L_A, 0)) {
      if (TwoWireC_Safe()) {
        TwoWireL_CompileMessage(TwoWire_G_SlaveIP[TwoWireA_L_A], TwoWireM_Command(1, TwoWireI_CheckBuffer(TwoWireA_L_A), 0), 0, 0, LOW);
        TwoWireF_Send();
        if (TwoWireD_Read(HIGH, HIGH)) {
          bool TwoWireA_L_CheckBuffer = false;
          switch (TwoWireH_CheckMessage(TwoWireA_L_A)) {
            case 1:
              TwoWireA_L_CheckBuffer = TwoWireJ_BufferRead(TwoWireA_L_A);
              TwoWireL_CompileMessage(TwoWire_G_SlaveIP[TwoWireA_L_A], TwoWireM_Command(1, TwoWireA_L_CheckBuffer, 0), TwoWire_G_BufferSend[0], TwoWire_G_BufferSend[1], TwoWireA_L_CheckBuffer);
              if (TwoWireC_Safe()) {
                TwoWireF_Send();
                TwoWireB_CheckConnectionInterval(TwoWireA_L_A, 1);
              }
              break;
            case 2:
              TwoWireK_BufferStore(TwoWireA_L_A, TwoWire_G_RX_Buffer[3], TwoWire_G_RX_Buffer[4]);
              TwoWireL_CompileMessage(TwoWire_G_SlaveIP[TwoWireA_L_A], TwoWireM_Command(1, !TwoWire_G_TXStatus[0], TwoWire_G_TXStatus[1]), 0, 0, LOW);
              if (TwoWireC_Safe()) {
                TwoWireF_Send();
                TwoWireB_CheckConnectionInterval(TwoWireA_L_A, 1);
              }
              break;
            case 4:
              TwoWireL_CompileMessage(TwoWire_G_SlaveIP[TwoWireA_L_A], TwoWireM_Command(1, 0, 0), 0, 0, LOW);
              if (TwoWireC_Safe()) {
                TwoWireF_Send();
                TwoWireB_CheckConnectionInterval(TwoWireA_L_A, 1);
              }
              break;
            default:
              break;
          }
          delayMicroseconds(30);
        }
      }
    }
    else {
      TwoWireL_CompileMessage(TwoWire_G_SlaveIP[TwoWireA_L_A], 0, 0, 0, LOW);
      if (TwoWireC_Safe()) {
        TwoWireF_Send();
        if (TwoWireD_Read(HIGH, LOW)) {
          if (TwoWireH_CheckMessage(TwoWireA_L_A) == 4) {
            TwoWireL_CompileMessage(TwoWire_G_SlaveIP[TwoWireA_L_A], TwoWireM_Command(1, 0, 0), 0, 0, LOW);
            if (TwoWireC_Safe()) {
              TwoWireF_Send();
              TwoWireB_CheckConnectionInterval(TwoWireA_L_A, 1);
            }
          }
          delayMicroseconds(30);
        }
      }
    }
  }
}

bool TwoWireB_CheckConnectionInterval(uint8_t TwoWireB_L_SlaveIP, bool TwoWireB_L_Command) {
  if (TwoWireB_L_Command) {
    TwoWire_G_SlaveTimer[TwoWireB_L_SlaveIP] = millis();
    TwoWire_G_SlaveConnected[TwoWireB_L_SlaveIP] = true;
    return true;
  }
  else {
    if (TwoWire_G_SlaveConnected[TwoWireB_L_SlaveIP]) {
      if ((millis() - TwoWire_G_SlaveTimer[TwoWireB_L_SlaveIP]) > 3000) {
        TwoWire_G_SlaveConnected[TwoWireB_L_SlaveIP] = false;
        TwoWireN_BufferFlush(TwoWireB_L_SlaveIP);
        return false;
      }
      else return true;
    }
    return false;
  }
}

bool TwoWireC_Safe() {
  uint16_t TwoWireC_L_Counter = 0;
  uint16_t TwoWireC_L_Delay = 15;
  TwoWireC_L_Delay *= 2.5;
  while (TwoWireC_L_Counter < TwoWireC_L_Delay and TwoWireO_PinRead()){
    TwoWireC_L_Counter++;
    delayMicroseconds(2);
  }
  if(TwoWireC_L_Counter >= TwoWireC_L_Delay) return true;
  else return false;
}

bool TwoWireD_Read(bool TwoWireD_L_LengthOne, bool TwoWireD_L_LengthTwo) {
  uint16_t TwoWireD_L_Counter = 0;
  uint16_t TwoWireD_L_Delay = 120;
  TwoWireD_L_Delay *= 2.5;
  while (TwoWireD_L_Counter < TwoWireD_L_Delay){
    TwoWireD_L_Counter++;
    if(!TwoWireO_PinRead()){
      uint8_t TwoWireD_L_ReadHold = TwoWireE_BitRead(TwoWireD_L_LengthOne, TwoWireD_L_LengthTwo);
      if(TwoWireD_L_ReadHold == 0) return false;
      else if(TwoWireD_L_ReadHold == 1)  return true;
    }
  }
  return false;
}

uint8_t TwoWireE_BitRead(bool TwoWireE_L_LengthOne, bool TwoWireE_L_LengthTwo) {
  uint16_t TwoWireE_L_Counter = 0;
  uint16_t TwoWireE_L_Delay = 20;
  uint16_t TwoWireE_L_DelayMin = 5;
  TwoWireE_L_Delay *= 2.5;
  TwoWireE_L_DelayMin *= 2.5;

  bool TwoWireE_L_TempBitHold = false;
  uint8_t TwoWireE_L_MaxPacketLength;
  uint8_t TwoWireE_L_PacketCount = 0;
  bool TwoWireE_L_RX_TempBuffer[21];
  bool TwoWireE_L_Done = false;
  if (TwoWireE_L_LengthOne or TwoWireE_L_LengthTwo) {
    if (TwoWireE_L_LengthTwo) TwoWireE_L_MaxPacketLength = 20;
    else TwoWireE_L_MaxPacketLength = 9;
  }
  else return 0;
  while (!TwoWireO_PinRead() and TwoWireE_L_Counter < TwoWireE_L_Delay) TwoWireE_L_Counter++;
  if (TwoWireE_L_Counter < TwoWireE_L_Delay and TwoWireE_L_Counter > TwoWireE_L_DelayMin) {
    TwoWireE_L_Delay = 15;
    while (TwoWireE_L_PacketCount <= TwoWireE_L_MaxPacketLength and !TwoWireE_L_Done) {
      delayMicroseconds(18);
      TwoWireE_L_TempBitHold = TwoWireO_PinRead();
      TwoWireE_L_Counter = 0;
      while (TwoWireO_PinRead() == TwoWireE_L_TempBitHold and TwoWireE_L_Counter < TwoWireE_L_Delay) {
        TwoWireE_L_Counter++;
        delayMicroseconds(2);
      }
      if (TwoWireE_L_Counter >= TwoWireE_L_Delay) TwoWireE_L_Done = true;
      else {
        TwoWireE_L_RX_TempBuffer[TwoWireE_L_PacketCount] = TwoWireE_L_TempBitHold;
        TwoWireE_L_PacketCount++;
      }
    }
    if ((TwoWireE_L_LengthOne and TwoWireE_L_PacketCount == 9) or (TwoWireE_L_LengthTwo and TwoWireE_L_PacketCount == 20)) {
      for (uint8_t TwoWireE_L_A = 0; TwoWireE_L_A < 3; TwoWireE_L_A++)
        bitWrite(TwoWire_G_RX_Buffer[0], TwoWireE_L_A, TwoWireE_L_RX_TempBuffer[TwoWireE_L_A]);
      for (uint8_t TwoWireE_L_B = 0; TwoWireE_L_B < 3; TwoWireE_L_B++)
        bitWrite(TwoWire_G_RX_Buffer[1], TwoWireE_L_B, TwoWireE_L_RX_TempBuffer[TwoWireE_L_B + 3]);
      for (uint8_t TwoWireE_L_C = 0; TwoWireE_L_C < 3; TwoWireE_L_C++)
        bitWrite(TwoWire_G_RX_Buffer[2], TwoWireE_L_C, TwoWireE_L_RX_TempBuffer[TwoWireE_L_C + 6]);
      if (TwoWireE_L_PacketCount == 20) {
        for (uint8_t TwoWireE_L_D = 0; TwoWireE_L_D < 3; TwoWireE_L_D++)
          bitWrite(TwoWire_G_RX_Buffer[3], TwoWireE_L_D, TwoWireE_L_RX_TempBuffer[TwoWireE_L_D + 9]);
        for (uint8_t TwoWireE_L_E = 0; TwoWireE_L_E < 8; TwoWireE_L_E++)
          bitWrite(TwoWire_G_RX_Buffer[4], TwoWireE_L_E, TwoWireE_L_RX_TempBuffer[TwoWireE_L_E + 12]);
      }
      return 1;
    }
  }
  else if(!(TwoWireE_L_Counter > TwoWireE_L_DelayMin)) return 2;
  return 0;
}

void TwoWireF_Send() {
  TwoWireG_BitSend();
}

void TwoWireG_BitSend() {
  const uint8_t TwoWireG_L_StartBitDelay_us = 20;
  const uint8_t TwoWireG_L_SpikeBitDelay_us = 10;
  const uint8_t TwoWireG_L_DataBitDelay_us = 20;

  uint8_t TwoWireG_L_PacketLength;

  if (TwoWire_G_TX_Buffer[20]) TwoWireG_L_PacketLength = 20;
  else TwoWireG_L_PacketLength = 9;

  TwoWireQ_PinSet(1);
  TwoWireP_PinWrite(0);
  delayMicroseconds(TwoWireG_L_StartBitDelay_us);
  TwoWireP_PinWrite(1);
  for (uint8_t TwoWireG_L_BitCount = 0; TwoWireG_L_BitCount < TwoWireG_L_PacketLength; TwoWireG_L_BitCount++) {
    delayMicroseconds(TwoWireG_L_SpikeBitDelay_us);
    TwoWireP_PinWrite(TwoWire_G_TX_Buffer[TwoWireG_L_BitCount]);
    delayMicroseconds(TwoWireG_L_DataBitDelay_us);
    TwoWireP_PinWrite(!TwoWire_G_TX_Buffer[TwoWireG_L_BitCount]);
  }
  delayMicroseconds(TwoWireG_L_SpikeBitDelay_us);
  TwoWireP_PinWrite(1);
  delayMicroseconds(5); 
  TwoWireQ_PinSet(0);
}

int8_t TwoWireH_CheckMessage(uint8_t TwoWireH_L_SlaveIP) {
  if (TwoWire_G_RX_Buffer[0] == TwoWire_G_MasterIP and TwoWire_G_RX_Buffer[1] == TwoWire_G_SlaveIP[TwoWireH_L_SlaveIP]) {
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
  return -1;
}

bool TwoWireI_CheckBuffer(uint8_t TwoWireI_L_SlaveIP) {
  if (TwoWire_G_Buffer[TwoWireI_L_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0] == TwoWire_G_Buffer[TwoWireI_L_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][1]) return false;
  else return true;
}

bool TwoWireJ_BufferRead(uint8_t TwoWireJ_L_SlaveIP) {
  if (TwoWireI_CheckBuffer(TwoWireJ_L_SlaveIP)) {
    if (TwoWire_G_Buffer[TwoWireJ_L_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0] < (TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize))
      TwoWire_G_Buffer[TwoWireJ_L_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0]++;
    else
      TwoWire_G_Buffer[TwoWireJ_L_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0] = 0;
    TwoWire_G_BufferSend[0] = TwoWire_G_SlaveIP[TwoWire_G_Buffer[TwoWireJ_L_SlaveIP][TwoWire_G_Buffer[TwoWireJ_L_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0]][0]];
    TwoWire_G_BufferSend[1] = TwoWire_G_Buffer[TwoWireJ_L_SlaveIP][TwoWire_G_Buffer[TwoWireJ_L_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0]][1];
    TwoWire_G_Buffer[TwoWireJ_L_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 2 + TwoWire_G_Buffer[TwoWireJ_L_SlaveIP][TwoWire_G_Buffer[TwoWireJ_L_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0]][0]][0]--;
    return true;
  }
  TwoWire_G_BufferSend[0] = 0;
  TwoWire_G_BufferSend[1] = 0;
  return false;
}

void TwoWireK_BufferStore(uint8_t TwoWireK_L_SenderIP, uint8_t TwoWireK_L_ReRecieverIP, uint8_t TwoWireK_L_Data) {
  bool TwoWireK_L_FoundIP = false;
  uint8_t TwoWireK_L_SlaveIP_Result = 0;
  TwoWire_G_TXStatus[0] = false;
  TwoWire_G_TXStatus[1] = false;
  for (uint8_t TwoWireK_L_A = 0; TwoWireK_L_A < TwoWire_G_SlaveIPCount and !TwoWireK_L_FoundIP; TwoWireK_L_A++) {
    if (TwoWireK_L_ReRecieverIP == TwoWire_G_SlaveIP[TwoWireK_L_A]) {
      TwoWireK_L_SlaveIP_Result = TwoWireK_L_A;
      TwoWireK_L_FoundIP = true;
    }
  }
  if (TwoWireK_L_FoundIP and TwoWire_G_SlaveConnected[TwoWireK_L_SlaveIP_Result]) {
    TwoWire_G_TXStatus[0] = true;
    uint8_t TwoWireK_L_BufferTest = TwoWire_G_Buffer[TwoWireK_L_SlaveIP_Result][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][1];
    if (TwoWire_G_Buffer[TwoWireK_L_SlaveIP_Result][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][1] == (TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize)) TwoWireK_L_BufferTest = 0;
    else TwoWireK_L_BufferTest++;
    if (TwoWireK_L_BufferTest == TwoWire_G_Buffer[TwoWireK_L_SlaveIP_Result][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0]) return;
    if (TwoWire_G_Buffer[TwoWireK_L_SlaveIP_Result][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 2 + TwoWireK_L_SenderIP][0] < TwoWire_G_BufferSize) {
      TwoWire_G_Buffer[TwoWireK_L_SlaveIP_Result][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 2 + TwoWireK_L_SenderIP][0]++;
      TwoWire_G_Buffer[TwoWireK_L_SlaveIP_Result][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][1] = TwoWireK_L_BufferTest;
      TwoWire_G_Buffer[TwoWireK_L_SlaveIP_Result][TwoWire_G_Buffer[TwoWireK_L_SlaveIP_Result][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][1]][0] = TwoWireK_L_SenderIP;
      TwoWire_G_Buffer[TwoWireK_L_SlaveIP_Result][TwoWire_G_Buffer[TwoWireK_L_SlaveIP_Result][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][1]][1] = TwoWireK_L_Data;
      TwoWire_G_TXStatus[1] = true;
    }
  }
  return;
}

void TwoWireL_CompileMessage(uint8_t TwoWireL_L_RecieverIP, uint8_t TwoWireL_L_Command, uint8_t TwoWireL_L_MessageIP, uint8_t TwoWireL_L_MessageData, bool TwoWireL_L_PacketLength) {
  for (uint8_t TwoWireL_L_A = 0; TwoWireL_L_A < 3; TwoWireL_L_A++) {
    TwoWire_G_TX_Buffer[TwoWireL_L_A] = bitRead(TwoWireL_L_RecieverIP, TwoWireL_L_A);
  }
  for (uint8_t TwoWireL_L_B = 0; TwoWireL_L_B < 3; TwoWireL_L_B++) {
    TwoWire_G_TX_Buffer[TwoWireL_L_B + 3] = bitRead(TwoWire_G_MasterIP, TwoWireL_L_B);
  }
  for (uint8_t TwoWireL_L_C = 0; TwoWireL_L_C < 3; TwoWireL_L_C++) {
    TwoWire_G_TX_Buffer[TwoWireL_L_C + 6] = bitRead(TwoWireL_L_Command, TwoWireL_L_C);
  }
  if (TwoWireL_L_PacketLength) {
    for (uint8_t TwoWireL_L_D = 0; TwoWireL_L_D < 3; TwoWireL_L_D++) {
      TwoWire_G_TX_Buffer[TwoWireL_L_D + 9] = bitRead(TwoWireL_L_MessageIP, TwoWireL_L_D);
    }
    for (uint8_t TwoWireL_L_E = 0; TwoWireL_L_E < 8; TwoWireL_L_E++) {
      TwoWire_G_TX_Buffer[TwoWireL_L_E + 12] = bitRead(TwoWireL_L_MessageData, TwoWireL_L_E);
    }
  }
  TwoWire_G_TX_Buffer[20] = TwoWireL_L_PacketLength;
}

uint8_t TwoWireM_Command(bool TwoWireM_L_X, bool TwoWireM_L_Y, bool TwoWireM_L_Z) {
  uint8_t TwoWireM_L_Final = 0;
  bitWrite(TwoWireM_L_Final, 0, TwoWireM_L_X);
  bitWrite(TwoWireM_L_Final, 1, TwoWireM_L_Y);
  bitWrite(TwoWireM_L_Final, 2, TwoWireM_L_Z);
  return TwoWireM_L_Final;
}

void TwoWireN_BufferFlush(uint8_t TwoWireN_L_SlaveIP) {
  TwoWire_G_Buffer[TwoWireN_L_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][0] = 0;
  TwoWire_G_Buffer[TwoWireN_L_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 1][1] = 0;
  for (uint8_t TwoWireN_L_A = 0; TwoWireN_L_A < TwoWireN_L_SlaveIP; TwoWireN_L_A++) {
    TwoWire_G_Buffer[TwoWireN_L_SlaveIP][(TwoWire_G_SlaveIPCount * TwoWire_G_BufferSize) + 2 + TwoWireN_L_A][0] = 0;
  }
  return;
}

bool TwoWireO_PinRead() {
  return bitRead(PINB, 0);
}

void TwoWireP_PinWrite(bool TwoWireP_L_DIR) {
  if (TwoWireP_L_DIR) {
    PORTB = PORTB | B00000001;
  }
  else {
    PORTB = PORTB & B11111110;
  }
}

void TwoWireQ_PinSet(bool TwoWireQ_L_DIR) {
  if (TwoWireQ_L_DIR) {
    DDRB = DDRB | B00000001;
  }
  else {
    PORTB = PORTB | B00000001;
    DDRB = DDRB & B11111110;
  }
}

//Master ▲▲▲










////Slave ▼▼▼
//const uint8_t TwoWire_G_SlaveIP = 2;
//const uint8_t TwoWire_G_MasterIP = 0;
//
//uint8_t TwoWire_G_BitBufferRX[5];
//bool TwoWire_G_BitBufferTX[21];
//
//uint32_t TwoWire_G_ConnectionTimer = 0;
//bool TwoWire_G_Connected = false;
//
//bool TwoWire_G_Status[7];
//
//uint8_t TwoWire_G_BufferRX[2];
//uint8_t TwoWire_G_BufferTX[2];
//
//void setup() {
//  TwoWireM_PinSet(0);
//}
//
//void loop() {
//}
//
//bool TwoWire_RTXMessage(bool TwoWireA_Command) {
//    if (TwoWireA_Command) {
//    for (int I = 0; I < 3; I++) {
//      TwoWireA_Run(1);
//      if (TwoWire_G_Status[1])
//        return true;
//      if (!TwoWire_G_Status[1] and TwoWire_G_Status[2])
//        return false;
//    }
//    if (TwoWire_G_Status[(sizeof(TwoWire_G_Status) - 1)])
//      return false;
//  }
//  else {
//    TwoWireA_Run(0);
//    if (TwoWire_G_Status[0])
//      return true;
//  }
//  return false;
//}
//
//void TwoWireA_Run(bool TwoWireA_L_Command) {
//  for (uint8_t TwoWireA_L_A = 0; TwoWireA_L_A < sizeof(TwoWire_G_Status); TwoWireA_L_A++) {
//    TwoWire_G_Status[TwoWireA_L_A] = 0;
//  }
//  if (TwoWire_G_Connected) {
//    if (!TwoWireA_L_Command) {
//      TwoWireJ_CompileMessage(1, 0, 0, false);
//      if (TwoWireC_ParsePacket(1, 0, 0)) {
//        if (bitRead(TwoWire_G_BitBufferRX[2], 0)) {
//          if (bitRead(TwoWire_G_BitBufferRX[2], 1)) {
//            TwoWireF_Send();
//            if (TwoWireD_Read(1, 1)) {
//              if (TwoWireI_CheckIP()) {
//                if (bitRead(TwoWire_G_BitBufferRX[2], 1)) {
//                  TwoWire_G_BufferRX[0] = TwoWire_G_BitBufferRX[3];
//                  TwoWire_G_BufferRX[1] = TwoWire_G_BitBufferRX[4];
//                  TwoWire_G_Status[0] = true;
//                }
//                TwoWire_G_ConnectionTimer = millis();
//              }
//            }
//          }
//        }
//        else {
//          TwoWire_G_Status[3] = true;
//          TwoWire_G_Connected = false;
//        }
//      }
//    }
//    else {
//      TwoWireJ_CompileMessage(2, TwoWire_G_BufferTX[0], TwoWire_G_BufferTX[1], true);
//      if (TwoWireC_ParsePacket(1, 0, 0)) {
//        if (bitRead(TwoWire_G_BitBufferRX[2], 0)) {
//          TwoWireF_Send();
//          if (TwoWireD_Read(1, 0)) {
//            if (TwoWireI_CheckIP()) {
//              if (bitRead(TwoWire_G_BitBufferRX[2], 2)) {
//                TwoWire_G_Status[1] = true;
//              }
//              else TwoWire_G_Status[2] = bitRead(TwoWire_G_BitBufferRX[2], 1);
//              TwoWire_G_ConnectionTimer = millis();
//            }
//          }
//        }
//        else {
//          TwoWire_G_Status[3] = true;
//          TwoWire_G_Connected = false;
//        }
//      }
//    }
//  }
//  TwoWireB_CheckInterval();
//  if (TwoWire_G_Status[3] or TwoWire_G_Status[4] or TwoWire_G_Status[5]) TwoWire_G_Status[6] = true;
//}
//
//void TwoWireB_CheckInterval() {
//  if ((millis() - TwoWire_G_ConnectionTimer) >= 1000 or !TwoWire_G_Connected) {
//    TwoWireJ_CompileMessage(4, 0, 0, false);
//    if (TwoWireC_ParsePacket(1, 0, 0)) {
//      if (bitRead(TwoWire_G_BitBufferRX[2], 0) and !TwoWire_G_Connected) {
//        TwoWire_G_Status[5] = true;
//        return;
//      }
//      TwoWireF_Send();
//      if (TwoWireD_Read(1, 0)) {
//        if (TwoWireI_CheckIP()) {
//          if (bitRead(TwoWire_G_BitBufferRX[2], 0)) {
//            TwoWire_G_Connected = true;
//            TwoWire_G_ConnectionTimer = millis();
//          }
//          else {
//            TwoWire_G_Status[3] = true;
//          TwoWire_G_Connected = false;
//          }
//        }
//      }
//    }
//  }
//}
//
//bool TwoWireC_ParsePacket(bool TwoWireC_L_LengthOne, bool TwoWireC_L_LengthTwo, bool TwoWireC_L_TimerLength) {
//  uint32_t TwoWireC_L_Timer = millis();
//  uint32_t TwoWireC_L_Interval;
//  if(TwoWireC_L_TimerLength) TwoWireC_L_Interval = 100;
//  else TwoWireC_L_Interval = 20;
//  while((millis() - TwoWireC_L_Timer) < TwoWireC_L_Interval){
//    if(TwoWireH_Insert()){
//      if(TwoWireD_Read(TwoWireC_L_LengthOne, TwoWireC_L_LengthTwo)){
//        if(TwoWireI_CheckIP()) return true;
//      }
//      if(TwoWire_G_Status[4]) return false;
//    }
//  }
//  return false;
//}
//
//bool TwoWireD_Read(bool TwoWireD_L_LengthOne, bool TwoWireD_L_LengthTwo) {
//  uint16_t TwoWireD_L_Counter = 0;
//  uint16_t TwoWireD_L_Delay = 300;
//  TwoWireD_L_Delay *= 2.5;
//  while (TwoWireD_L_Counter < TwoWireD_L_Delay){
//    TwoWireD_L_Counter++;
//    if(!TwoWireK_PinRead()){
//      uint8_t TwoWireD_L_ReadHold = TwoWireE_BitRead(TwoWireD_L_LengthOne, TwoWireD_L_LengthTwo);
//      if(TwoWireD_L_ReadHold == 0) return false;
//      else if(TwoWireD_L_ReadHold == 1) return true;
//    }
//  }
//  return false;
//}
//
//bool TwoWireE_BitRead(bool TwoWireE_L_LengthOne, bool TwoWireE_L_LengthTwo) {
//  uint16_t TwoWireE_L_Counter = 0;
//  uint16_t TwoWireE_L_Delay = 20;
//  uint16_t TwoWireE_L_DelayMin = 5;
//  TwoWireE_L_Delay *= 2.5;
//  TwoWireE_L_DelayMin *= 2.5;
//
//  bool TwoWireE_L_TempBitHold = false;
//  uint8_t TwoWireE_L_MaxPacketLength;
//  uint8_t TwoWireE_L_PacketCount = 0;
//  bool TwoWireE_L_RX_TempBuffer[21];
//  bool TwoWireE_L_Done = false;
//  if (TwoWireE_L_LengthOne or TwoWireE_L_LengthTwo) {
//    if (TwoWireE_L_LengthTwo) TwoWireE_L_MaxPacketLength = 20;
//    else TwoWireE_L_MaxPacketLength = 9;
//  }
//  else return 0;
//  while (!TwoWireK_PinRead() and TwoWireE_L_Counter < TwoWireE_L_Delay) TwoWireE_L_Counter++;
//  if (TwoWireE_L_Counter < TwoWireE_L_Delay and TwoWireE_L_Counter > TwoWireE_L_DelayMin) {
//    TwoWireE_L_Delay = 15;
//    while (TwoWireE_L_PacketCount <= TwoWireE_L_MaxPacketLength and !TwoWireE_L_Done) {
//      delayMicroseconds(18);
//      TwoWireE_L_TempBitHold = TwoWireK_PinRead();
//      TwoWireE_L_Counter = 0;
//      while (TwoWireK_PinRead() == TwoWireE_L_TempBitHold and TwoWireE_L_Counter < TwoWireE_L_Delay){
//        TwoWireE_L_Counter++;
//        delayMicroseconds(2);
//      }
//      if (TwoWireE_L_Counter >= TwoWireE_L_Delay) TwoWireE_L_Done = true;
//      else {
//        TwoWireE_L_RX_TempBuffer[TwoWireE_L_PacketCount] = TwoWireE_L_TempBitHold;
//        TwoWireE_L_PacketCount++;
//      }
//    }
//    if ((TwoWireE_L_LengthOne and TwoWireE_L_PacketCount == 9) or (TwoWireE_L_LengthTwo and TwoWireE_L_PacketCount == 20)) {
//      for (uint8_t TwoWireE_L_A = 0; TwoWireE_L_A < 3; TwoWireE_L_A++)
//        bitWrite(TwoWire_G_BitBufferRX[0], TwoWireE_L_A, TwoWireE_L_RX_TempBuffer[TwoWireE_L_A]);
//      for (uint8_t TwoWireE_L_B = 0; TwoWireE_L_B < 3; TwoWireE_L_B++)
//        bitWrite(TwoWire_G_BitBufferRX[1], TwoWireE_L_B, TwoWireE_L_RX_TempBuffer[TwoWireE_L_B + 3]);
//      for (uint8_t TwoWireE_L_C = 0; TwoWireE_L_C < 3; TwoWireE_L_C++)
//        bitWrite(TwoWire_G_BitBufferRX[2], TwoWireE_L_C, TwoWireE_L_RX_TempBuffer[TwoWireE_L_C + 6]);
//      if (TwoWireE_L_PacketCount == 20) {
//        for (uint8_t TwoWireE_L_D = 0; TwoWireE_L_D < 3; TwoWireE_L_D++)
//          bitWrite(TwoWire_G_BitBufferRX[3], TwoWireE_L_D, TwoWireE_L_RX_TempBuffer[TwoWireE_L_D + 9]);
//        for (uint8_t TwoWireE_L_E = 0; TwoWireE_L_E < 8; TwoWireE_L_E++)
//          bitWrite(TwoWire_G_BitBufferRX[4], TwoWireE_L_E, TwoWireE_L_RX_TempBuffer[TwoWireE_L_E + 12]);
//      }
//      return 1;
//    }
//  }
//  else if (!(TwoWireE_L_Counter > TwoWireE_L_DelayMin)) return 2;
//  return 0;
//}
//
//void TwoWireF_Send() {
//  TwoWireG_BitSend();
//}
//
//void TwoWireG_BitSend() {
//  const uint8_t TwoWireG_L_StartBitDelay_us = 20;
//  const uint8_t TwoWireG_L_SpikeBitDelay_us = 10;
//  const uint8_t TwoWireG_L_DataBitDelay_us = 20;
//
//  uint8_t TwoWireG_L_PacketLength;
//
//  if (TwoWire_G_BitBufferTX[20]) TwoWireG_L_PacketLength = 20;
//  else TwoWireG_L_PacketLength = 9;
//
//  TwoWireM_PinSet(1);
//  TwoWireL_PinWrite(0);
//  delayMicroseconds(TwoWireG_L_StartBitDelay_us);
//  TwoWireL_PinWrite(1);
//  for (uint8_t TwoWireG_L_BitCount = 0; TwoWireG_L_BitCount < TwoWireG_L_PacketLength; TwoWireG_L_BitCount++) {
//    delayMicroseconds(TwoWireG_L_SpikeBitDelay_us);
//    TwoWireL_PinWrite(TwoWire_G_BitBufferTX[TwoWireG_L_BitCount]);
//    delayMicroseconds(TwoWireG_L_DataBitDelay_us);
//    TwoWireL_PinWrite(!TwoWire_G_BitBufferTX[TwoWireG_L_BitCount]);
//  }
//  delayMicroseconds(TwoWireG_L_SpikeBitDelay_us);
//  TwoWireL_PinWrite(1);
//  delayMicroseconds(5);
//  TwoWireM_PinSet(0);
//}
//
//bool TwoWireH_Insert() {
//  uint16_t TwoWireH_L_Counter = 0;
//  uint16_t TwoWireH_L_Delay = 60;
//  TwoWireH_L_Delay *= 2.5;
//  while (TwoWireH_L_Counter < TwoWireH_L_Delay and TwoWireK_PinRead()) TwoWireH_L_Counter++;
//  if(TwoWireH_L_Counter >= TwoWireH_L_Delay) return true;
//  else return false;
//}
//
//bool TwoWireI_CheckIP() {
//  if(TwoWire_G_BitBufferRX[0] == TwoWire_G_SlaveIP and TwoWire_G_BitBufferRX[1] == TwoWire_G_MasterIP) return true;
//  else return false;
//}
//
//void TwoWireJ_CompileMessage(uint8_t TwoWireJ_L_COM, uint8_t TwoWireJ_L_MSG_IP, uint8_t TwoWireJ_L_MSG_DATA, bool TwoWireJ_L_PacketLength) {
//  for (uint8_t TwoWireJ_L_A = 0; TwoWireJ_L_A < 3; TwoWireJ_L_A++) {
//    TwoWire_G_BitBufferTX[TwoWireJ_L_A] = bitRead(TwoWire_G_MasterIP, TwoWireJ_L_A);
//  }
//  for (uint8_t TwoWireJ_L_B = 0; TwoWireJ_L_B < 3; TwoWireJ_L_B++) {
//    TwoWire_G_BitBufferTX[TwoWireJ_L_B + 3] = bitRead(TwoWire_G_SlaveIP, TwoWireJ_L_B);
//  }
//  for (uint8_t TwoWireJ_L_C = 0; TwoWireJ_L_C < 3; TwoWireJ_L_C++) {
//    TwoWire_G_BitBufferTX[TwoWireJ_L_C + 6] = bitRead(TwoWireJ_L_COM, TwoWireJ_L_C);
//  }
//  if (TwoWireJ_L_PacketLength) {
//    for (uint8_t TwoWireJ_L_D = 0; TwoWireJ_L_D < 3; TwoWireJ_L_D++) {
//      TwoWire_G_BitBufferTX[TwoWireJ_L_D + 9] = bitRead(TwoWireJ_L_MSG_IP, TwoWireJ_L_D);
//    }
//    for (uint8_t TwoWireJ_L_E = 0; TwoWireJ_L_E < 8; TwoWireJ_L_E++) {
//      TwoWire_G_BitBufferTX[TwoWireJ_L_E + 12] = bitRead(TwoWireJ_L_MSG_DATA, TwoWireJ_L_E);
//    }
//  }
//  TwoWire_G_BitBufferTX[20] = TwoWireJ_L_PacketLength;
//}
//
//bool TwoWireK_PinRead() {
//  return bitRead(PINB, 0);
//}
//
//void TwoWireL_PinWrite(bool TwoWireL_L_DIR) {
//  if (TwoWireL_L_DIR) {
//    PORTB = PORTB | B00000001;
//  }
//  else {
//    PORTB = PORTB & B11111110;
//  }
//}
//
//void TwoWireM_PinSet(bool TwoWireM_L_DIR) {
//  if (TwoWireM_L_DIR) {
//    DDRB = DDRB | B00000001;
//  }
//  else {
//    PORTB = PORTB | B00000001;
//    DDRB = DDRB & B11111110;
//  }
//}
//
////Slave ▲▲▲
