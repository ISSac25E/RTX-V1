//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

const uint8_t TwoWire_G_SlaveIP = 4;
const uint8_t TwoWire_G_MasterIP = 0;

uint8_t TwoWire_G_BitBufferRX[5];
bool TwoWire_G_BitBufferTX[21];

uint32_t TwoWire_G_ConnectionTimer = 0;
bool TwoWire_G_Connected = false;

bool TwoWire_G_Status[7];

uint8_t TwoWire_G_BufferRX[2];
uint8_t TwoWire_G_BufferTX[2];

//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

bool Tally_Connected = false;

uint32_t ButtonTimer2 = 0;
uint32_t ButtonTimer3 = 0;
uint32_t ButtonTimer4 = 0;

bool ButtonState2 = false;
bool ButtonState3 = false;
bool ButtonState4 = false;
bool ButtonState12 = false;

uint32_t ButtonTimeOut = 0;
uint32_t Comp_Timer = millis();
uint32_t Tally_Timer = millis();
uint32_t Error_LED_Timer = millis();
uint32_t Tally_LED_Timer = millis();

bool Error_LED_State = false;
bool Tally_Error_LED_State = false;

bool Error = true;
bool Mode = false;

uint8_t PWNCounter = 0;
uint8_t PWMDelay = 8;

uint16_t Tally_PWNCounter = 100;
uint16_t Tally_PWMDelay = 8;

uint8_t TallyPacketCount = 0;
uint8_t TempTallyStatus[8];

bool TallyStatus[2];

void setup() {
  TwoWireM_PinSet(0);
  pinMode(13, OUTPUT);

  pinMode(12, INPUT_PULLUP);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop() {
  Button_Execute();
  if (TwoWire_RTXMessage(0)) {
    if (TwoWire_G_BufferRX[0] == 1) {
      if (bitRead(TwoWire_G_BufferRX[1], 0) and bitRead(TwoWire_G_BufferRX[1], 1) and !bitRead(TwoWire_G_BufferRX[1], 2)) {
        Comp_Timer = millis();
        Error = false;
      }
      else
        Error = true;
    }
    if (TwoWire_G_BufferRX[0] == 2) {
      Tally_Timer = millis();
      RTX_TallyCheck();
    }

  }
  Companion_Error();
  Tally_Error();
  TallyUpdate();
}

void RTX_TallyCheck() {
  uint8_t PacketNumber = 0;
  bitWrite(PacketNumber, 0, bitRead(TwoWire_G_BufferRX[1], 0));
  bitWrite(PacketNumber, 1, bitRead(TwoWire_G_BufferRX[1], 1));
  if (PacketNumber == TallyPacketCount or PacketNumber == 0) {
    if (PacketNumber == 0) {
      for (int I = 0; I < 6; I++) {
        bitWrite(TempTallyStatus[I], 0, bitRead(TwoWire_G_BufferRX[1], (I + 2)));
      }
      TallyPacketCount = 1;
    }
    if (PacketNumber == 1) {
      for (int I = 0; I < 6; I++) {
        bitWrite(TempTallyStatus[I], 1, bitRead(TwoWire_G_BufferRX[1], (I + 2)));
      }
      TallyPacketCount = 2;
    }
    if (PacketNumber == 2) {
      for (int I = 0; I < 2; I++) {
        bitWrite(TempTallyStatus[I + 6], 0, bitRead(TwoWire_G_BufferRX[1], (I + 2)));
        bitWrite(TempTallyStatus[I + 6], 1, bitRead(TwoWire_G_BufferRX[1], (I + 4)));
      }
      Tally_Connected = bitRead(TwoWire_G_BufferRX[1], 7);
      if (Tally_Connected) {
        TallyStatus[0] = bitRead(TempTallyStatus[5], 0);
        TallyStatus[1] = bitRead(TempTallyStatus[5], 1);
      }
      else {
        TallyStatus[0] = false;
        TallyStatus[1] = false;
      }
      TallyPacketCount = 0;
    }
  }
  else {
    TallyPacketCount = 0;
  }
}

void TallyUpdate() {
  if (Tally_Connected) {
    if (TallyStatus[0]){
//      analogWrite(5, 100);
      analogWrite(6, 0);

      
    if ((millis() - Tally_LED_Timer) > Tally_PWMDelay) {
      Tally_LED_Timer = millis();
      Tally_PWMDelay = 8;
      if (Tally_Error_LED_State) {
        if (Tally_PWNCounter > 30)
          Tally_PWNCounter -= 5;
        else {
          Tally_Error_LED_State = false;
          Tally_PWMDelay = 100;
        }
        analogWrite(5, Tally_PWNCounter);
      }
      else {
        if (Tally_PWNCounter < 100)
          Tally_PWNCounter += 5;
        else {
          Tally_Error_LED_State = true;
          Tally_PWMDelay = 100;
        }
        analogWrite(5, Tally_PWNCounter);
      }
    }

    
    }
    else {
      Tally_Error_LED_State = true;
      Tally_PWNCounter = 100;
      Tally_PWMDelay = 0;
      analogWrite(5, 0);
      if (TallyStatus[1] and !TallyStatus[0]) analogWrite(6, 100);
      else analogWrite(6, 0);
    }
  }
}

void Tally_Error() {
  if (!Tally_Connected) {
    analogWrite(6, 0);
    if((millis() - Tally_LED_Timer) > Tally_PWMDelay) {
      Tally_LED_Timer = millis();
      if(Tally_Error_LED_State) {
        analogWrite(5,0);
         Tally_PWMDelay = 600;
         Tally_Error_LED_State = false;
      }
      else {
        analogWrite(5,100);
         Tally_PWMDelay = 50;
         Tally_Error_LED_State = true;
      }
    }
  }
  else{
    if ((millis() - Tally_Timer) > 500) Tally_Connected = false;
//   Tally_LED_Timer = millis();
//    Tally_Error_LED_State = true;
//      Tally_PWNCounter = 100;
//      Tally_PWMDelay = 0;
  }
}

void Companion_Error() {
  if (((millis() - Comp_Timer) > 1500) or Error) {
    Error = true;
    analogWrite(11, 0);
    if ((millis() - Error_LED_Timer) > PWMDelay) {
      Error_LED_Timer = millis();
      PWMDelay = 8;
      if (Error_LED_State) {
        if (PWNCounter > 0)
          PWNCounter -= 5;
        else {
          Error_LED_State = false;
          PWMDelay = 100;
        }
        analogWrite(10, PWNCounter);
      }
      else {
        if (PWNCounter < 100)
          PWNCounter += 5;
        else {
          Error_LED_State = true;
          PWMDelay = 100;
        }
        analogWrite(10, PWNCounter);
      }
    }
  }
  else {
    if (Mode) {
      analogWrite(11, 0);
      analogWrite(10, 100);
    }
    else {
      analogWrite(10, 0);
      analogWrite(11, 100);
    }
  }
}

void Button_Execute() {
  if (!Mode) {
    if (!digitalRead(2) and !ButtonState2) {
      delay(5);
      if (!digitalRead(2)) {
        ButtonTimer2 = millis();
        ButtonState2 = true;
      }
    }
    if (digitalRead(2) and ButtonState2) {
      delay(5);
      if (digitalRead(2)) {
        ButtonState2 = false;
        if ((millis() - ButtonTimer2) >= 200) {
          TwoWire_G_BufferTX[0] = 1;
          TwoWire_G_BufferTX[1] = 29;
          TwoWire_RTXMessage(1);
          return;
        }
        else {
          Mode = true;
          TwoWire_G_BufferTX[0] = 1;
          TwoWire_G_BufferTX[1] = 25;
          TwoWire_RTXMessage(1);
          ButtonTimeOut = millis();
          return;
        }
      }
    }

    if (!digitalRead(3) and !ButtonState3) {
      delay(5);
      if (!digitalRead(3)) {
        ButtonTimer3 = millis();
        ButtonState3 = true;
      }
    }
    if (digitalRead(3) and ButtonState3) {
      delay(5);
      if (digitalRead(3)) {
        ButtonState3 = false;
        Mode = true;
        TwoWire_G_BufferTX[0] = 1;
        TwoWire_G_BufferTX[1] = 26;
        TwoWire_RTXMessage(1);
        ButtonTimeOut = millis();
        return;
        //        if ((millis() - ButtonTimeOut) >= 300) {
        //          ButtonTimeOut = millis();
        //          if ((millis() - ButtonTimer3) >= 300) {
        //            TwoWire_G_BufferTX[0] = 1;
        //            TwoWire_G_BufferTX[1] = 28;
        //            TwoWire_RTXMessage(1);
        //            return;
        //          }
        //          else {
        //            TwoWire_G_BufferTX[0] = 1;
        //            TwoWire_G_BufferTX[1] = 27;
        //            TwoWire_RTXMessage(1);
        //            return;
        //          }
        //        }
      }
    }

    if (!digitalRead(4) and !ButtonState4) {
      delay(5);
      if (!digitalRead(4)) {
        ButtonTimer4 = millis();
        ButtonState4 = true;
      }
    }
    if (digitalRead(4) and ButtonState4) {
      delay(5);
      if (digitalRead(4)) {
        ButtonState4 = false;
        if ((millis() - ButtonTimer4) >= 200) {
          TwoWire_G_BufferTX[0] = 1;
          TwoWire_G_BufferTX[1] = 30;
          TwoWire_RTXMessage(1);
          return;
        }
        else {
          Mode = true;
          TwoWire_G_BufferTX[0] = 1;
          TwoWire_G_BufferTX[1] = 26;
          TwoWire_RTXMessage(1);
          ButtonTimeOut = millis();
          return;
        }
      }
    }
  }
  else {
    if ((!digitalRead(2) or !digitalRead(3) or !digitalRead(4)) and !ButtonState3) {
      delay(5);
      if ((!digitalRead(2) or !digitalRead(3) or !digitalRead(4))) {
        ButtonTimer3 = millis();
        ButtonState3 = true;
      }
    }
    if (digitalRead(2) and digitalRead(3) and digitalRead(4) and ButtonState3) {
      delay(5);
      if (digitalRead(2) and digitalRead(3) and digitalRead(4)) {
        ButtonState3 = false;
        if ((millis() - ButtonTimeOut) >= 300) {
          ButtonTimeOut = millis();
          if ((millis() - ButtonTimer3) >= 300) {
            TwoWire_G_BufferTX[0] = 1;
            TwoWire_G_BufferTX[1] = 28;
            TwoWire_RTXMessage(1);
            return;
          }
          else {
            TwoWire_G_BufferTX[0] = 1;
            TwoWire_G_BufferTX[1] = 27;
            TwoWire_RTXMessage(1);
            return;
          }
        }
      }
    }
  }

  if (!digitalRead(12) and !ButtonState12) {
    delay(5);
    if (!digitalRead(12)) {
      if (!Error) {
        //      if(Mode)
        //      Mode = false;
        //      else
        //      Mode = true;
        Mode = !Mode;
        if (Mode) {
          //        ButtonTimer2 = millis();
          //        ButtonTimer4 = millis();
          analogWrite(11, 0);
          analogWrite(10, 100);
        }
        else {
          analogWrite(10, 0);
          analogWrite(11, 100);
        }
      }
      ButtonState12 = true;
    }
  }
  if (digitalRead(12) and ButtonState12) {
    delay(5);
    if (digitalRead(12)) {
      ButtonState12 = false;
    }
  }
}



//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

bool TwoWire_RTXMessage(bool TwoWireA_Command) {
  if (TwoWireA_Command) {
    for (int I = 0; I < 3; I++) {
      TwoWireA_Run(1);
      if (TwoWire_G_Status[1])
        return true;
      if (!TwoWire_G_Status[1] and TwoWire_G_Status[2])
        return false;
    }
    if (TwoWire_G_Status[(sizeof(TwoWire_G_Status) - 1)])
      return false;
  }
  else {
    TwoWireA_Run(0);
    if (TwoWire_G_Status[0])
      return true;
  }
  return false;
}

void TwoWireA_Run(bool TwoWireA_L_Command) {
  for (uint8_t TwoWireA_L_A = 0; TwoWireA_L_A < sizeof(TwoWire_G_Status); TwoWireA_L_A++) {
    TwoWire_G_Status[TwoWireA_L_A] = 0;
  }
  if (TwoWire_G_Connected) {
    if (!TwoWireA_L_Command) {
      TwoWireJ_CompileMessage(1, 0, 0, false);
      if (TwoWireC_ParsePacket(1, 0, 0)) {
        if (bitRead(TwoWire_G_BitBufferRX[2], 0)) {
          if (bitRead(TwoWire_G_BitBufferRX[2], 1)) {
            TwoWireF_Send();
            if (TwoWireD_Read(1, 1)) {
              if (TwoWireI_CheckIP()) {
                if (bitRead(TwoWire_G_BitBufferRX[2], 1)) {
                  TwoWire_G_BufferRX[0] = TwoWire_G_BitBufferRX[3];
                  TwoWire_G_BufferRX[1] = TwoWire_G_BitBufferRX[4];
                  TwoWire_G_Status[0] = true;
                }
                TwoWire_G_ConnectionTimer = millis();
              }
            }
          }
        }
        else {
          TwoWire_G_Status[3] = true;
          TwoWire_G_Connected = false;
        }
      }
    }
    else {
      TwoWireJ_CompileMessage(2, TwoWire_G_BufferTX[0], TwoWire_G_BufferTX[1], true);
      if (TwoWireC_ParsePacket(1, 0, 0)) {
        if (bitRead(TwoWire_G_BitBufferRX[2], 0)) {
          TwoWireF_Send();
          if (TwoWireD_Read(1, 0)) {
            if (TwoWireI_CheckIP()) {
              if (bitRead(TwoWire_G_BitBufferRX[2], 2)) {
                TwoWire_G_Status[1] = true;
              }
              else TwoWire_G_Status[2] = bitRead(TwoWire_G_BitBufferRX[2], 1);
              TwoWire_G_ConnectionTimer = millis();
            }
          }
        }
        else {
          TwoWire_G_Status[3] = true;
          TwoWire_G_Connected = false;
        }
      }
    }
  }
  TwoWireB_CheckInterval();
  if (TwoWire_G_Status[3] or TwoWire_G_Status[4] or TwoWire_G_Status[5]) TwoWire_G_Status[6] = true;
}

void TwoWireB_CheckInterval() {
  if ((millis() - TwoWire_G_ConnectionTimer) >= 1000 or !TwoWire_G_Connected) {
    TwoWireJ_CompileMessage(4, 0, 0, false);
    if (TwoWireC_ParsePacket(1, 0, 0)) {
      if (bitRead(TwoWire_G_BitBufferRX[2], 0) and !TwoWire_G_Connected) {
        TwoWire_G_Status[5] = true;
        return;
      }
      TwoWireF_Send();
      if (TwoWireD_Read(1, 0)) {
        if (TwoWireI_CheckIP()) {
          if (bitRead(TwoWire_G_BitBufferRX[2], 0)) {
            TwoWire_G_Connected = true;
            TwoWire_G_ConnectionTimer = millis();
          }
          else {
            TwoWire_G_Status[3] = true;
            TwoWire_G_Connected = false;
          }
        }
      }
    }
  }
}

bool TwoWireC_ParsePacket(bool TwoWireC_L_LengthOne, bool TwoWireC_L_LengthTwo, bool TwoWireC_L_TimerLength) {
  uint32_t TwoWireC_L_Timer = millis();
  uint32_t TwoWireC_L_Interval = 20;
  //  if(TwoWireC_L_TimerLength) TwoWireC_L_Interval = 100;
  //  else TwoWireC_L_Interval = 20;
  while ((millis() - TwoWireC_L_Timer) < TwoWireC_L_Interval) {
    if (TwoWireH_Insert()) {
      if (TwoWireD_Read(TwoWireC_L_LengthOne, TwoWireC_L_LengthTwo)) {
        if (TwoWireI_CheckIP()) return true;
      }
      if (TwoWire_G_Status[4]) return false;
    }
  }
  return false;
}

bool TwoWireD_Read(bool TwoWireD_L_LengthOne, bool TwoWireD_L_LengthTwo) {
  uint16_t TwoWireD_L_Counter = 0;
  uint16_t TwoWireD_L_Delay = 200;

  TwoWireD_L_Delay *= 2.5;
  while (TwoWireD_L_Counter < TwoWireD_L_Delay) {
    TwoWireD_L_Counter++;
    if (!TwoWireK_PinRead()) {
      uint8_t TwoWireD_L_ReadHold = TwoWireE_BitRead(TwoWireD_L_LengthOne, TwoWireD_L_LengthTwo);
      if (TwoWireD_L_ReadHold == 0) return false;
      else if (TwoWireD_L_ReadHold == 1) return true;
    }
  }
  TwoWire_G_Status[4] = true;
  return false;
}

bool TwoWireE_BitRead(bool TwoWireE_L_LengthOne, bool TwoWireE_L_LengthTwo) {
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
  while (!TwoWireK_PinRead() and TwoWireE_L_Counter < TwoWireE_L_Delay) TwoWireE_L_Counter++;
  if (TwoWireE_L_Counter < TwoWireE_L_Delay and TwoWireE_L_Counter > TwoWireE_L_DelayMin) {
    TwoWireE_L_Delay = 15;
    while (TwoWireE_L_PacketCount <= TwoWireE_L_MaxPacketLength and !TwoWireE_L_Done) {
      delayMicroseconds(18);
      TwoWireE_L_TempBitHold = TwoWireK_PinRead();
      TwoWireE_L_Counter = 0;
      while (TwoWireK_PinRead() == TwoWireE_L_TempBitHold and TwoWireE_L_Counter < TwoWireE_L_Delay) {
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
        bitWrite(TwoWire_G_BitBufferRX[0], TwoWireE_L_A, TwoWireE_L_RX_TempBuffer[TwoWireE_L_A]);
      for (uint8_t TwoWireE_L_B = 0; TwoWireE_L_B < 3; TwoWireE_L_B++)
        bitWrite(TwoWire_G_BitBufferRX[1], TwoWireE_L_B, TwoWireE_L_RX_TempBuffer[TwoWireE_L_B + 3]);
      for (uint8_t TwoWireE_L_C = 0; TwoWireE_L_C < 3; TwoWireE_L_C++)
        bitWrite(TwoWire_G_BitBufferRX[2], TwoWireE_L_C, TwoWireE_L_RX_TempBuffer[TwoWireE_L_C + 6]);
      if (TwoWireE_L_PacketCount == 20) {
        for (uint8_t TwoWireE_L_D = 0; TwoWireE_L_D < 3; TwoWireE_L_D++)
          bitWrite(TwoWire_G_BitBufferRX[3], TwoWireE_L_D, TwoWireE_L_RX_TempBuffer[TwoWireE_L_D + 9]);
        for (uint8_t TwoWireE_L_E = 0; TwoWireE_L_E < 8; TwoWireE_L_E++)
          bitWrite(TwoWire_G_BitBufferRX[4], TwoWireE_L_E, TwoWireE_L_RX_TempBuffer[TwoWireE_L_E + 12]);
      }
      return 1;
    }
  }
  else if (!(TwoWireE_L_Counter > TwoWireE_L_DelayMin)) return 2;
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

  if (TwoWire_G_BitBufferTX[20]) TwoWireG_L_PacketLength = 20;
  else TwoWireG_L_PacketLength = 9;

  TwoWireM_PinSet(1);
  TwoWireL_PinWrite(0);
  delayMicroseconds(TwoWireG_L_StartBitDelay_us);
  TwoWireL_PinWrite(1);
  for (uint8_t TwoWireG_L_BitCount = 0; TwoWireG_L_BitCount < TwoWireG_L_PacketLength; TwoWireG_L_BitCount++) {
    delayMicroseconds(TwoWireG_L_SpikeBitDelay_us);
    TwoWireL_PinWrite(TwoWire_G_BitBufferTX[TwoWireG_L_BitCount]);
    delayMicroseconds(TwoWireG_L_DataBitDelay_us);
    TwoWireL_PinWrite(!TwoWire_G_BitBufferTX[TwoWireG_L_BitCount]);
  }
  delayMicroseconds(TwoWireG_L_SpikeBitDelay_us);
  TwoWireL_PinWrite(1);
  delayMicroseconds(5);
  TwoWireM_PinSet(0);
}

bool TwoWireH_Insert() {
  uint16_t TwoWireH_L_Counter = 0;
  uint16_t TwoWireH_L_Delay = 60;
  while (TwoWireH_L_Counter < TwoWireH_L_Delay and TwoWireK_PinRead()) {
    TwoWireH_L_Counter++;
    delayMicroseconds(2);
  }
  if (TwoWireH_L_Counter >= TwoWireH_L_Delay) return true;
  else return false;
}

bool TwoWireI_CheckIP() {
  if (TwoWire_G_BitBufferRX[0] == TwoWire_G_SlaveIP and TwoWire_G_BitBufferRX[1] == TwoWire_G_MasterIP) return true;
  else return false;
}

void TwoWireJ_CompileMessage(uint8_t TwoWireJ_L_COM, uint8_t TwoWireJ_L_MSG_IP, uint8_t TwoWireJ_L_MSG_DATA, bool TwoWireJ_L_PacketLength) {
  for (uint8_t TwoWireJ_L_A = 0; TwoWireJ_L_A < 3; TwoWireJ_L_A++) {
    TwoWire_G_BitBufferTX[TwoWireJ_L_A] = bitRead(TwoWire_G_MasterIP, TwoWireJ_L_A);
  }
  for (uint8_t TwoWireJ_L_B = 0; TwoWireJ_L_B < 3; TwoWireJ_L_B++) {
    TwoWire_G_BitBufferTX[TwoWireJ_L_B + 3] = bitRead(TwoWire_G_SlaveIP, TwoWireJ_L_B);
  }
  for (uint8_t TwoWireJ_L_C = 0; TwoWireJ_L_C < 3; TwoWireJ_L_C++) {
    TwoWire_G_BitBufferTX[TwoWireJ_L_C + 6] = bitRead(TwoWireJ_L_COM, TwoWireJ_L_C);
  }
  if (TwoWireJ_L_PacketLength) {
    for (uint8_t TwoWireJ_L_D = 0; TwoWireJ_L_D < 3; TwoWireJ_L_D++) {
      TwoWire_G_BitBufferTX[TwoWireJ_L_D + 9] = bitRead(TwoWireJ_L_MSG_IP, TwoWireJ_L_D);
    }
    for (uint8_t TwoWireJ_L_E = 0; TwoWireJ_L_E < 8; TwoWireJ_L_E++) {
      TwoWire_G_BitBufferTX[TwoWireJ_L_E + 12] = bitRead(TwoWireJ_L_MSG_DATA, TwoWireJ_L_E);
    }
  }
  TwoWire_G_BitBufferTX[20] = TwoWireJ_L_PacketLength;
}

bool TwoWireK_PinRead() {
  return bitRead(PINB, 0);
}

void TwoWireL_PinWrite(bool TwoWireL_L_DIR) {
  if (TwoWireL_L_DIR) {
    PORTB = PORTB | B00000001;
  }
  else {
    PORTB = PORTB & B11111110;
  }
}

void TwoWireM_PinSet(bool TwoWireM_L_DIR) {
  if (TwoWireM_L_DIR) {
    DDRB = DDRB | B00000001;
  }
  else {
    PORTB = PORTB | B00000001;
    DDRB = DDRB & B11111110;
  }
}
//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲
