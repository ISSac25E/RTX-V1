const bool ControllerSide = 0; //0 = Center, 1 = Side

//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

const uint8_t TwoWire_G_SlaveIP = 3 + ControllerSide;
//const uint8_t TwoWire_G_SlaveIP = 3;
const uint8_t TwoWire_G_MasterIP = 0;

uint8_t TwoWire_G_BitBufferRX[5];
bool TwoWire_G_BitBufferTX[21];

uint32_t TwoWire_G_ConnectionTimer = 0;
bool TwoWire_G_Connected = false;

bool TwoWire_G_Status[7];

uint8_t TwoWire_G_BufferRX[2];
uint8_t TwoWire_G_BufferTX[2];

//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

uint8_t ButtonPIN[] {12, 2, 3, 4};
uint32_t ButtonTimer[4];
bool ButtonState[4];
bool CombButtonPressed;
uint32_t CombButtonTimer;
uint32_t ButtonTimeOut;
bool CompMode = false;
bool AutoMode = true;

bool Tally_Connected;
bool Comp_Connected;

uint32_t Comp_Timer = millis();
uint32_t Tally_Timer = millis();
uint32_t Status_LED_Timer = millis();
uint32_t Tally_LED_Timer = millis();

bool Status_LED_State;
uint16_t Status_PWN_Counter = 0;
uint16_t Status_PWM_Delay = 8;
bool Tally_LED_State;
uint16_t Tally_PWN_Counter = 100;
uint16_t Tally_PWM_Delay = 8;

uint8_t TallyPacketCount = 0;
uint8_t TempTallyStatus[8];
bool TallyStatus[2][2];

uint8_t LED_TallyPIN[] = {5, 6}; //{PROG,PREV}
uint8_t LED_StatusPIN[] = {10, 11}; //{ORANGE,GREEN}

void setup() {
  TwoWireM_PinSet(0);
  pinMode(ButtonPIN[0], INPUT_PULLUP);
  pinMode(ButtonPIN[1], INPUT_PULLUP);
  pinMode(ButtonPIN[2], INPUT_PULLUP);
  pinMode(ButtonPIN[3], INPUT_PULLUP);

  pinMode(LED_StatusPIN[0], OUTPUT);
  pinMode(LED_StatusPIN[1], OUTPUT);
  pinMode(LED_TallyPIN[0], OUTPUT);
  pinMode(LED_TallyPIN[1], OUTPUT);
}

void loop() {
  if (TwoWire_RTXMessage(0)) {
    if (TwoWire_G_BufferRX[0] == 1){
      if (bitRead(TwoWire_G_BufferRX[1], 0) and bitRead(TwoWire_G_BufferRX[1], 1) and !bitRead(TwoWire_G_BufferRX[1], 2)) {
        Comp_Timer = millis();
        Comp_Connected = true;
      }
    else Comp_Connected = false;
    }
    if(TwoWire_G_BufferRX[0] == 2){
      Tally_Timer = millis();
      RTX_TallyCheck();
    }
  }
  Check_Connection();
  Button_Execute();
  LED_Run();
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
        TallyStatus[0][0] = bitRead(TempTallyStatus[4], 0);
        TallyStatus[0][1] = bitRead(TempTallyStatus[4], 1);
        TallyStatus[1][0] = bitRead(TempTallyStatus[5], 0);
        TallyStatus[1][1] = bitRead(TempTallyStatus[5], 1);
      }
      else {
        TallyStatus[0][0] = false;
        TallyStatus[0][1] = false;
        TallyStatus[1][0] = false;
        TallyStatus[1][1] = false;
      }
      TallyPacketCount = 0;
    }
  }
  else {
    TallyPacketCount = 0;
  }
}

void Check_Connection() {
  if ((millis() - Comp_Timer) >  1500) Comp_Connected = false;
  if ((millis() - Tally_Timer) > 500) Tally_Connected = false;
}

void LED_Run() {
  if (Comp_Connected) {
    if (AutoMode) {
      analogWrite(LED_StatusPIN[0], 0);
      if ((millis() - Status_LED_Timer) > Status_PWM_Delay) {
        Status_LED_Timer = millis();
        Status_PWM_Delay = 8;
        if (Status_LED_State) {
          if (Status_PWN_Counter > 0)
            Status_PWN_Counter -= 5;
          else {
            Status_LED_State = false;
            Status_PWM_Delay = 100;
          }
          analogWrite(LED_StatusPIN[1], Status_PWN_Counter);
        }
        else {
          if (Status_PWN_Counter < 100)
            Status_PWN_Counter += 5;
          else {
            Status_LED_State = true;
            Status_PWM_Delay = 100;
          }
          analogWrite(LED_StatusPIN[1], Status_PWN_Counter);
        }
      }
    }
    else if (CompMode) {
      analogWrite(LED_StatusPIN[0], 100);
      analogWrite(LED_StatusPIN[1], 0);
    }
    else {
      analogWrite(LED_StatusPIN[0], 0);
      analogWrite(LED_StatusPIN[1], 100);
    }
  }
  else {
    analogWrite(LED_StatusPIN[1], 0);
    if ((millis() - Status_LED_Timer) > Status_PWM_Delay) {
      Status_LED_Timer = millis();
      Status_PWM_Delay = 8;
      if (Status_LED_State) {
        if (Status_PWN_Counter > 0)
          Status_PWN_Counter -= 5;
        else {
          Status_LED_State = false;
          Status_PWM_Delay = 100;
        }
        analogWrite(LED_StatusPIN[0], Status_PWN_Counter);
      }
      else {
        if (Status_PWN_Counter < 100)
          Status_PWN_Counter += 5;
        else {
          Status_LED_State = true;
          Status_PWM_Delay = 100;
        }
        analogWrite(LED_StatusPIN[0], Status_PWN_Counter);
      }
    }
  }


  if (Tally_Connected) {
    if (TallyStatus[ControllerSide][0]) {
      analogWrite(LED_TallyPIN[1], 0);
      if ((millis() - Tally_LED_Timer) > Tally_PWM_Delay) {
        Tally_LED_Timer = millis();
        Tally_PWM_Delay = 8;
        if (Tally_LED_State) {
          if (Tally_PWN_Counter > 30)
            Tally_PWN_Counter -= 5;
          else {
            Tally_LED_State = false;
            Tally_PWM_Delay = 100;
          }
          analogWrite(LED_TallyPIN[0], Tally_PWN_Counter);
        }
        else {
          if (Tally_PWN_Counter < 100)
            Tally_PWN_Counter += 5;
          else {
            Tally_LED_State = true;
            Tally_PWM_Delay = 100;
          }
          analogWrite(LED_TallyPIN[0], Tally_PWN_Counter);
        }
      }
    }
    else {
      Tally_LED_State = true;
      Tally_PWN_Counter = 100;
      Tally_PWM_Delay = 0;
      analogWrite(LED_TallyPIN[0], 0);
      if (TallyStatus[ControllerSide][1]) analogWrite(LED_TallyPIN[1], 100);
      else analogWrite(LED_TallyPIN[1], 0);
    }
  }
  else {
    analogWrite(LED_TallyPIN[1], 0);
    if ((millis() - Tally_LED_Timer) > Tally_PWM_Delay) {
      Tally_LED_Timer = millis();
      if (Tally_LED_State) {
        Tally_LED_State = false;
        Tally_PWM_Delay = 600;
        analogWrite(LED_TallyPIN[0], 0);
      }
      else {
        Tally_LED_State = true;
        Tally_PWM_Delay = 50;
        analogWrite(LED_TallyPIN[0], 100);
      }
    }
  }
}

void Button_Execute() {
  if (!digitalRead(ButtonPIN[0]) and !ButtonState[0]) {
    delay(5);
    if (!digitalRead(ButtonPIN[0])) {
      ButtonState[0] = true;
      ButtonTimer[0] = millis();
    }
  }
  if (digitalRead(ButtonPIN[0]) and ButtonState[0]) {
    delay(5);
    if (digitalRead(ButtonPIN[0])) {
      ButtonState[0] = false;
      if ((millis() - ButtonTimer[0]) >= 300) AutoMode = !AutoMode;
      else CompMode = !CompMode;
    }
  }

  if (AutoMode) {
    if ((!digitalRead(ButtonPIN[1]) or !digitalRead(ButtonPIN[2]) or !digitalRead(ButtonPIN[3])) and !CombButtonPressed) {
      delay(5);
      if ((!digitalRead(ButtonPIN[1]) or !digitalRead(ButtonPIN[2]) or !digitalRead(ButtonPIN[3]))) {
        CombButtonPressed = true;
        CombButtonTimer = millis();
        if (Tally_Connected) {
          if ((TallyStatus[0][0] or TallyStatus[1][0]) and !(TallyStatus[0][0] and TallyStatus[1][0])) {
            if (TallyStatus[1][0] and !TallyStatus[0][1]) {
              TwoWire_G_BufferTX[0] = 1;
              TwoWire_G_BufferTX[1] = 30;
              TwoWire_RTXMessage(1);
            }
            else if (TallyStatus[0][0] and !TallyStatus[1][1]) {
              TwoWire_G_BufferTX[0] = 1;
              TwoWire_G_BufferTX[1] = 29;
              TwoWire_RTXMessage(1);
            }
          }
        }
      }
    }
    if (digitalRead(ButtonPIN[1]) and digitalRead(ButtonPIN[2]) and digitalRead(ButtonPIN[3]) and CombButtonPressed) {
      delay(5);
      if (digitalRead(ButtonPIN[1]) and digitalRead(ButtonPIN[2]) and digitalRead(ButtonPIN[3])) {
        CombButtonPressed = false;
        if ((millis() - ButtonTimeOut) >= 300) {
          ButtonTimeOut = millis();
          if (Tally_Connected) {
            if (TallyStatus[0][0] or TallyStatus[1][0]) {
              if ((millis() - CombButtonTimer) >= 300) {
                TwoWire_G_BufferTX[0] = 1;
                TwoWire_G_BufferTX[1] = 28;
                TwoWire_RTXMessage(1);
              }
              else {
                TwoWire_G_BufferTX[0] = 1;
                TwoWire_G_BufferTX[1] = 27;
                TwoWire_RTXMessage(1);
              }
            }
            else {
              if ((millis() - CombButtonTimer) >= 300) {
                TwoWire_G_BufferTX[0] = 1;
                TwoWire_G_BufferTX[1] = 25;
                TwoWire_RTXMessage(1);
              }
              else {
                TwoWire_G_BufferTX[0] = 1;
                TwoWire_G_BufferTX[1] = 26;
                TwoWire_RTXMessage(1);
              }
            }
          }
          else {
            if ((millis() - CombButtonTimer) >= 300) {
              TwoWire_G_BufferTX[0] = 1;
              TwoWire_G_BufferTX[1] = 28;
              TwoWire_RTXMessage(1);
            }
            else {
              TwoWire_G_BufferTX[0] = 1;
              TwoWire_G_BufferTX[1] = 27;
              TwoWire_RTXMessage(1);
            }
          }
        }
      }
    }
  }
  else if (!CompMode) {
    if (!digitalRead(ButtonPIN[1]) and !ButtonState[1]) {
      delay(5);
      if (!digitalRead(ButtonPIN[1])) {
        ButtonState[1] = true;
        ButtonTimer[1] = millis();
      }
    }
    if (digitalRead(ButtonPIN[1]) and ButtonState[1]) {
      delay(5);
      if (digitalRead(ButtonPIN[1])) {
        ButtonState[1] = false;
        if ((millis() - ButtonTimer[1]) >= 200) {
          TwoWire_G_BufferTX[0] = 1;
          TwoWire_G_BufferTX[1] = 29;
          TwoWire_RTXMessage(1);
        }
        else {
          CompMode = true;
          TwoWire_G_BufferTX[0] = 1;
          TwoWire_G_BufferTX[1] = 25;
          TwoWire_RTXMessage(1);
          ButtonTimeOut = millis();
        }
      }
    }
    if (!digitalRead(ButtonPIN[2]) and !ButtonState[2]) {
      delay(5);
      if (!digitalRead(ButtonPIN[2])) {
        ButtonState[2] = true;
        ButtonTimer[2] = millis();
      }
    }
    if (digitalRead(ButtonPIN[2]) and ButtonState[2]) {
      delay(5);
      if (digitalRead(ButtonPIN[2])) {
        ButtonState[2] = false;
        CompMode = true;
        TwoWire_G_BufferTX[0] = 1;
        TwoWire_G_BufferTX[1] = 26;
        TwoWire_RTXMessage(1);
        ButtonTimeOut = millis();
      }
    }
    if (!digitalRead(ButtonPIN[3]) and !ButtonState[3]) {
      delay(5);
      if (!digitalRead(ButtonPIN[3])) {
        ButtonState[3] = true;
        ButtonTimer[3] = millis();
      }
    }
    if (digitalRead(ButtonPIN[3]) and ButtonState[3]) {
      delay(5);
      if (digitalRead(ButtonPIN[3])) {
        ButtonState[3] = false;
        if ((millis() - ButtonTimer[3]) >= 200) {
          TwoWire_G_BufferTX[0] = 1;
          TwoWire_G_BufferTX[1] = 29;
          TwoWire_RTXMessage(1);
        }
        else {
          CompMode = true;
          TwoWire_G_BufferTX[0] = 1;
          TwoWire_G_BufferTX[1] = 25;
          TwoWire_RTXMessage(1);
          ButtonTimeOut = millis();
        }
      }
    }
  }
  else {
    if ((!digitalRead(ButtonPIN[1]) or !digitalRead(ButtonPIN[2]) or !digitalRead(ButtonPIN[3])) and !CombButtonPressed) {
      delay(5);
      if ((!digitalRead(ButtonPIN[1]) or !digitalRead(ButtonPIN[2]) or !digitalRead(ButtonPIN[3]))) {
        CombButtonPressed = true;
        CombButtonTimer = millis();

      }
    }
    if (digitalRead(ButtonPIN[1]) and digitalRead(ButtonPIN[2]) and digitalRead(ButtonPIN[3]) and CombButtonPressed) {
      delay(5);
      if (digitalRead(ButtonPIN[1]) and digitalRead(ButtonPIN[2]) and digitalRead(ButtonPIN[3])) {
        CombButtonPressed = false;
        if ((millis() - ButtonTimeOut) >= 300) {
          ButtonTimeOut = millis();
          if ((millis() - CombButtonTimer) >= 300) {
            TwoWire_G_BufferTX[0] = 1;
            TwoWire_G_BufferTX[1] = 28;
            TwoWire_RTXMessage(1);
          }
          else {
            TwoWire_G_BufferTX[0] = 1;
            TwoWire_G_BufferTX[1] = 27;
            TwoWire_RTXMessage(1);
          }
        }
      }
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