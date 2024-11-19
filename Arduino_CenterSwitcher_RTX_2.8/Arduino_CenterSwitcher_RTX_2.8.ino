//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

const uint8_t TwoWire_SlaveIP = 3;
const uint8_t TwoWire_MasterIP = 0;

uint32_t TwoWire_ConnectionTimer = 0;
uint8_t TwoWire_RX_BitBuffer[5];
bool TwoWire_TX_BitBuffer[21];
bool TwoWire_Connected = false;

bool  TwoWire_Status[7];

uint8_t TwoWire_TX_Buffer[2];
uint8_t TwoWire_RX_Buffer[2];

//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

uint32_t ButtonTimer2 = 0;
uint32_t ButtonTimer3 = 0;
uint32_t ButtonTimer4 = 0;

bool ButtonState2 = false;
bool ButtonState3 = false;
bool ButtonState4 = false;
bool ButtonState12 = false;

uint32_t ButtonTimeOut = 0;
uint32_t Comp_Timer = millis();
uint32_t Error_LED_Timer = millis();

bool Error_LED_State = false;

bool Error = true;
bool Mode = false;

uint8_t PWNCounter = 0;
uint8_t PWMDelay = 8;


uint8_t TallyPacketCount = 0;
uint8_t TallyStatus[8];

void setup() {
  TwoWire_PinSet(0, 0);
  TwoWire_PinSet(1, 0);
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
    if (TwoWire_RX_Buffer[0] == 1) {
      if (bitRead(TwoWire_RX_Buffer[1], 0) and bitRead(TwoWire_RX_Buffer[1], 1) and !bitRead(TwoWire_RX_Buffer[1], 2)) {
        Comp_Timer = millis();
        Error = false;
      }
      else
        Error = true;
    }
    if (TwoWire_RX_Buffer[0] == 2) RTX_TallyCheck();

  }
  Companion_Error();
}

void RTX_TallyCheck() {
  uint8_t PacketNumber = 0;
  bitWrite(PacketNumber, 0, bitRead(TwoWire_RX_Buffer[1], 0));
  bitWrite(PacketNumber, 1, bitRead(TwoWire_RX_Buffer[1], 1));
  if (PacketNumber == TallyPacketCount or PacketNumber == 0) {
    if (PacketNumber == 0) {
      for (int I = 0; I < 6; I++) {
        bitWrite(TallyStatus[I], 0, bitRead(TwoWire_RX_Buffer[1], (I + 2)));
      }
      TallyPacketCount = 1;
    }
    if (PacketNumber == 1) {
      for (int I = 0; I < 6; I++) {
        bitWrite(TallyStatus[I], 1, bitRead(TwoWire_RX_Buffer[1], (I + 2)));
      }
      TallyPacketCount = 2;
    }
    if (PacketNumber == 2) {
      for (int I = 0; I < 2; I++) {
        bitWrite(TallyStatus[I+6], 0, bitRead(TwoWire_RX_Buffer[1], (I + 2)));
        bitWrite(TallyStatus[I+6], 1, bitRead(TwoWire_RX_Buffer[1], (I + 4)));
      }
      TallyUpdate();
      TallyPacketCount = 0;
    }
  }
  else {
    TallyPacketCount = 0;
  }
}

void TallyUpdate(){
  if(bitRead(TallyStatus[4], 0)) analogWrite(5, 100);
  else analogWrite(5, 0);
  if(bitRead(TallyStatus[4], 1) and !bitRead(TallyStatus[4], 0)) analogWrite(6, 100);
  else analogWrite(6, 0);
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
          TwoWire_TX_Buffer[0] = 1;
          TwoWire_TX_Buffer[1] = 29;
          TwoWire_RTXMessage(1);
          return;
        }
        else {
          Mode = true;
          TwoWire_TX_Buffer[0] = 1;
          TwoWire_TX_Buffer[1] = 25;
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
        TwoWire_TX_Buffer[0] = 1;
        TwoWire_TX_Buffer[1] = 25;
        TwoWire_RTXMessage(1);
        ButtonTimeOut = millis();
        return;
        //        if ((millis() - ButtonTimeOut) >= 300) {
        //          ButtonTimeOut = millis();
        //          if ((millis() - ButtonTimer3) >= 300) {
        //            TwoWire_TX_Buffer[0] = 1;
        //            TwoWire_TX_Buffer[1] = 28;
        //            TwoWire_RTXMessage(1);
        //            return;
        //          }
        //          else {
        //            TwoWire_TX_Buffer[0] = 1;
        //            TwoWire_TX_Buffer[1] = 27;
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
          TwoWire_TX_Buffer[0] = 1;
          TwoWire_TX_Buffer[1] = 30;
          TwoWire_RTXMessage(1);
          return;
        }
        else {
          Mode = true;
          TwoWire_TX_Buffer[0] = 1;
          TwoWire_TX_Buffer[1] = 26;
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
            TwoWire_TX_Buffer[0] = 1;
            TwoWire_TX_Buffer[1] = 28;
            TwoWire_RTXMessage(1);
            return;
          }
          else {
            TwoWire_TX_Buffer[0] = 1;
            TwoWire_TX_Buffer[1] = 27;
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
      TwoWire_Run(1);
      if (TwoWire_Status[1])
        return true;
      if (!TwoWire_Status[1] and TwoWire_Status[2])
        return false;
    }
    if (TwoWire_Status[(sizeof(TwoWire_Status) - 1)])
      return false;
  }
  else {
    TwoWire_Run(0);
    if (TwoWire_Status[0])
      return true;
  }
  return false;
}

void TwoWire_Run(bool TwoWire_Command) {
  for (uint8_t I = 0; I < sizeof(TwoWire_Status); I++) {
    TwoWire_Status[I] = false;
  }
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
                  TwoWire_Status[0] = true;
                }
                TwoWire_ConnectionTimer = micros();
              }
            }
          }
        }
        else {
          TwoWire_Status[2] = true;
          TwoWire_Connected = false;
        }
      }
    }
    if (TwoWire_Command) {
      TwoWire_CompileMessage(2, TwoWire_TX_Buffer[0], TwoWire_TX_Buffer[1], HIGH);
      if (TwoWire_ParsePacket(1, 0, 0)) {
        if (bitRead(TwoWire_RX_BitBuffer[2], 0)) {
          TwoWire_Send();
          if (TwoWire_Read(1, 0)) {
            if (TwoWire_CheckIP()) {
              if (bitRead(TwoWire_RX_BitBuffer[2], 2)) {
                TwoWire_Status[1] = true;
              }
              else {
                TwoWire_Status[2] = bitRead(TwoWire_RX_BitBuffer[2], 1);
              }
              TwoWire_ConnectionTimer = micros();
            }
          }
        }
        else {
          TwoWire_Status[3] = true;
          TwoWire_Connected = false;
        }
      }
    }
  }
  TwoWire_Check_Interval();
  if (TwoWire_Status[3] or TwoWire_Status[4] or TwoWire_Status[5]) {
    TwoWire_Status[(sizeof(TwoWire_Status) - 1)] = true;
  }
}


void TwoWire_Check_Interval() {
  if (((micros() - TwoWire_ConnectionTimer) > 1000000) or !TwoWire_Connected) {
    TwoWire_CompileMessage(4, 0, 0, false);
    if (TwoWire_ParsePacket(1, 0, 1)) {
      if (bitRead(TwoWire_RX_BitBuffer[2], 0) and !TwoWire_Connected) {
        TwoWire_Status[5] = true;
        return;
      }
      TwoWire_Send();
      if (TwoWire_Read(1, 0)) {
        if (TwoWire_CheckIP()) {
          if (bitRead(TwoWire_RX_BitBuffer[2], 0)) {
            TwoWire_Connected = true;
            TwoWire_ConnectionTimer = micros();
          }
          else {
            TwoWire_Status[3] = true;
            TwoWire_Connected = false;
          }
        }
      }
      else {
        TwoWire_Connected = false;
        TwoWire_Status[3] = true;
      }
    }
  }
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
      if (TwoWire_Status[4])
        return false;
    }
  }
  return false;
}

bool TwoWire_Insert() {
  uint32_t TwoWire_Timer = micros();
  while ((micros() - TwoWire_Timer) < 60) {
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
  TwoWire_Status[4] = true;
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
    for (uint8_t X = 0; X < 3; X++) {
      bitWrite(TwoWire_RX_BitBuffer[0], X , TwoWire_RX_TempBuffer[X]);
    }
    for (uint8_t X = 0; X < 3; X++) {
      bitWrite(TwoWire_RX_BitBuffer[1], X , TwoWire_RX_TempBuffer[X + 3]);
    }
    for (uint8_t X = 0; X < 3; X++) {
      bitWrite(TwoWire_RX_BitBuffer[2], X , TwoWire_RX_TempBuffer[X + 6]);
    }
    if (TwoWire_PacketCount == 20) {
      for (uint8_t X = 0; X < 3; X++) {
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
    return bitRead(PINB, 0);
  }
  else {
    return bitRead(PIND, 7);
  }
}

void TwoWire_PinWrite(bool TwoWire_PIN, bool TwoWire_DIR) {
  if (TwoWire_PIN) {
    if (TwoWire_DIR) {
      PORTB = PORTB | B00000001;
    }
    else {
      PORTB = PORTB & B11111110;
    }
  }
  else {
    if (TwoWire_DIR) {
      PORTD = PORTD | B10000000;
    }
    else {
      PORTD = PORTD & B01111111;
    }
  }
}

void TwoWire_PinSet(bool TwoWire_PIN, bool TwoWire_DIR) {
  if (TwoWire_PIN) {
    if (TwoWire_DIR) {
      DDRB = DDRB | B00000001;
    }
    else {
      PORTB = PORTB | B00000001;
      DDRB = DDRB & B11111110;
    }
  }
  else {
    if (TwoWire_DIR) {
      DDRD = DDRD | B10000000;
    }
    else {
      PORTD = PORTD | B10000000;
      DDRD = DDRD & B01111111;
    }
  }
}
//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲
