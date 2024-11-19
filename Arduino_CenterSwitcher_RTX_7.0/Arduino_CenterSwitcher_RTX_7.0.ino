#include <EEPROM.h>

//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

const uint8_t TwoWire_G_SlaveIP = 3; //3 Center, 4 Side
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

bool TallyRedFadeEN = true;
bool TallyRedLightEN = true;
bool TallyGreenLightEN = true;
bool SwitchEN;

uint8_t GUI_Registers[256];

uint8_t GUI_RX_Buffer[3];
uint8_t GUI_RX_PacketCount;

uint8_t GUI_TX_Buffer[3];
uint8_t GUI_TX_PacketCount;
bool GUI_Sending;

uint8_t ButtonPIN[] {12, 2, 3, 4};
uint32_t ButtonTimer[4];
bool ButtonTimerState[4];
bool ButtonState[4];
bool CombButtonPressed;
uint32_t CombButtonTimer;
uint32_t ButtonTimeOut;

uint8_t Mode = 0;
//bool CompMode = false;
//bool AutoMode = true;

bool Tally_Connected;
bool Comp_Connected;

uint32_t Comp_Timer = millis();
uint32_t Tally_Timer = millis();
uint32_t Status_LED_Timer = millis();
uint32_t Tally_LED_Timer = millis();
uint32_t EEPROM_LED_Timer = millis();

bool Status_LED_State;
uint16_t Status_PWN_Counter = 0;
uint16_t Status_PWM_Delay = 8;
bool Tally_LED_State;
uint16_t Tally_PWN_Counter = 100;
uint16_t Tally_PWM_Delay = 8;
uint8_t EEPROM_LED_STATE;
uint16_t EEPROM_LED_Delay;

uint8_t TallyPacketCount = 0;
uint8_t TallyStatus[8];
bool TallyStatusLED[2];

uint8_t LED_TallyPIN[] = {5, 6}; //{PROG,PREV}
uint8_t LED_StatusPIN[] = {10, 11}; //{ORANGE,GREEN}

bool EEPROM_ERROR;
bool EEPROM_WRITING;

uint8_t GUI_EEPROM_TEMP_DATA_BUFFER[11];
uint8_t EEPROM_WRITE_CYCLE_COUNTER[3];

void setup() {
  EEPROM_ERROR = !GUI_EEPROM_READ();
  RTX_GUI_UPDATE_REGISTER(2);
  RTX_GUI_UPDATE_REGISTER(3);
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
  pinMode(13, OUTPUT);
  if (TwoWire_RTXMessage(0)) {
    if (TwoWire_G_BufferRX[0] == 1) {
      Tally_Timer = millis();
       Comp_Timer = millis();
      RTX_TallyCheck();
    }
    else if (TwoWire_G_BufferRX[0] == 5) RTX_GUI_UPDATE();
  }
  Check_Connection();
  Button_Execute();
  LED_Run();
  RTX_GUI_UPDATE_VALUE(0);
  RTX_GUI_UPDATE_REGISTER(0);
  RTX_GUI_SEND_RUN();
  EEPROM_RUN_WRITE_CYCLE();
}

void EEPROM_RUN_WRITE_CYCLE() {
  if (EEPROM_WRITING) {
    digitalWrite(13, HIGH);
    if (EEPROM_WRITE_CYCLE_COUNTER[2] < 4) {
      if (!EEPROM_WRITE_CYCLE_COUNTER[2]) {
        uint32_t TEMP_EEPROM_BUFFER = 0;
        for (uint8_t X = 0; X < 4; X++)  TEMP_EEPROM_BUFFER += EEPROM.read(1023 - X) << (8 * X);
        TEMP_EEPROM_BUFFER++;
        for (uint8_t X = 0; X < 4; X++) GUI_EEPROM_TEMP_DATA_BUFFER[X] = TEMP_EEPROM_BUFFER >> (8 * X);
      }
      EEPROM.write(1023 - EEPROM_WRITE_CYCLE_COUNTER[2], GUI_EEPROM_TEMP_DATA_BUFFER[EEPROM_WRITE_CYCLE_COUNTER[2]]);
      EEPROM_WRITE_CYCLE_COUNTER[2]++;
      if (EEPROM_WRITE_CYCLE_COUNTER[2] == 4) {
        for (uint8_t X = 0; X < 11; X++) GUI_EEPROM_TEMP_DATA_BUFFER[X] = 0;
        EEPROM_WRITE_CYCLE_COUNTER[0] = 0;
        EEPROM_WRITE_CYCLE_COUNTER[1] = 0;
      }
    }
    else {
      if (!EEPROM_WRITE_CYCLE_COUNTER[1]) {
        for (uint8_t X = 0; X < 11; X++) GUI_EEPROM_TEMP_DATA_BUFFER[X] = 0;
        for (uint8_t X = 0; X < 8; X++) GUI_EEPROM_TEMP_DATA_BUFFER[X] = GUI_Registers[X + (EEPROM_WRITE_CYCLE_COUNTER[0] * 8)];

        for (uint8_t Y = 0; Y < 8; Y++) {
          for (uint8_t X = 0; X < 8; X++)
            if (bitRead(GUI_EEPROM_TEMP_DATA_BUFFER[Y], X))
              bitWrite(GUI_EEPROM_TEMP_DATA_BUFFER[8], Y, !bitRead(GUI_EEPROM_TEMP_DATA_BUFFER[8], Y));
        }

        for (uint8_t Y = 0; Y < 8; Y++) {
          for (uint8_t X = 0; X < 8; X++)
            if (bitRead(GUI_EEPROM_TEMP_DATA_BUFFER[X], Y))
              bitWrite(GUI_EEPROM_TEMP_DATA_BUFFER[9], Y, !bitRead(GUI_EEPROM_TEMP_DATA_BUFFER[9], Y));
        }

        uint16_t TEMP_EEPROM_BUFFER = 85;
        for (uint8_t X = 0; X < 10; X++) TEMP_EEPROM_BUFFER += GUI_EEPROM_TEMP_DATA_BUFFER[X];
        GUI_EEPROM_TEMP_DATA_BUFFER[10] = TEMP_EEPROM_BUFFER >> 8;
        GUI_EEPROM_TEMP_DATA_BUFFER[10] += TEMP_EEPROM_BUFFER;
      }

      uint16_t TEMP_EEPROM_ADDRESS;
      if (EEPROM_WRITE_CYCLE_COUNTER[1] < 8) TEMP_EEPROM_ADDRESS = (EEPROM_WRITE_CYCLE_COUNTER[1] + (EEPROM_WRITE_CYCLE_COUNTER[0] * 8));
      else TEMP_EEPROM_ADDRESS = ((EEPROM_WRITE_CYCLE_COUNTER[1] - 8) + (EEPROM_WRITE_CYCLE_COUNTER[0] * 3) + 224);

      EEPROM.write(TEMP_EEPROM_ADDRESS, GUI_EEPROM_TEMP_DATA_BUFFER[EEPROM_WRITE_CYCLE_COUNTER[1]]);

      if (EEPROM_WRITE_CYCLE_COUNTER[1] < 10) EEPROM_WRITE_CYCLE_COUNTER[1]++;
      else {
        EEPROM_WRITE_CYCLE_COUNTER[1] = 0;
        if (EEPROM_WRITE_CYCLE_COUNTER[0] < 27) EEPROM_WRITE_CYCLE_COUNTER[0]++;
        else {
          EEPROM_WRITE_CYCLE_COUNTER[0] = 0;
          EEPROM_WRITE_CYCLE_COUNTER[1] = 0;
          EEPROM_WRITE_CYCLE_COUNTER[2] = 0;
          for (uint8_t X = 0; X < 11; X++) GUI_EEPROM_TEMP_DATA_BUFFER[X] = 0;
          EEPROM_WRITING = false;
          RTX_GUI_UPDATE_REGISTER(4);
        }
      }
    }
  }
  else digitalWrite(13, LOW);
}

bool GUI_EEPROM_READ() {
  EEPROM_WRITE_CYCLE_COUNTER[0] = 0;
  EEPROM_WRITE_CYCLE_COUNTER[1] = 0;
  EEPROM_WRITE_CYCLE_COUNTER[2] = 0;
  for (uint8_t X = 0; X < 11; X++) GUI_EEPROM_TEMP_DATA_BUFFER[X] = 0;
  for (EEPROM_WRITE_CYCLE_COUNTER[0] = 0; EEPROM_WRITE_CYCLE_COUNTER[0] < 28; EEPROM_WRITE_CYCLE_COUNTER[0]++) {
    for (EEPROM_WRITE_CYCLE_COUNTER[1] = 0; EEPROM_WRITE_CYCLE_COUNTER[1] < 8; EEPROM_WRITE_CYCLE_COUNTER[1]++) GUI_EEPROM_TEMP_DATA_BUFFER[EEPROM_WRITE_CYCLE_COUNTER[1]] = EEPROM.read(EEPROM_WRITE_CYCLE_COUNTER[1] + (EEPROM_WRITE_CYCLE_COUNTER[0] * 8));
    for (EEPROM_WRITE_CYCLE_COUNTER[1] = 0; EEPROM_WRITE_CYCLE_COUNTER[1] < 3; EEPROM_WRITE_CYCLE_COUNTER[1]++) GUI_EEPROM_TEMP_DATA_BUFFER[EEPROM_WRITE_CYCLE_COUNTER[1] + 8] = EEPROM.read(EEPROM_WRITE_CYCLE_COUNTER[1] + (EEPROM_WRITE_CYCLE_COUNTER[0] * 3) + 224);
    uint8_t Temp_V_Parity_Test = 0;
    uint8_t Temp_H_Parity_Test = 0;
    for (uint8_t Y = 0; Y < 8; Y++)
      for (uint8_t X = 0; X < 8; X++) if (bitRead(GUI_EEPROM_TEMP_DATA_BUFFER[Y], X)) bitWrite(Temp_V_Parity_Test, Y, ! bitRead(Temp_V_Parity_Test, Y));
    for (uint8_t Y = 0; Y < 8; Y++)
      for (uint8_t X = 0; X < 8; X++) if (bitRead(GUI_EEPROM_TEMP_DATA_BUFFER[X], Y)) bitWrite(Temp_H_Parity_Test, Y, ! bitRead(Temp_H_Parity_Test, Y));

    uint16_t Temp_Check_Sum2 = 85;
    for (uint8_t X = 0; X < 10; X++) Temp_Check_Sum2 += GUI_EEPROM_TEMP_DATA_BUFFER[X];
    uint8_t Temp_Check_Sum = Temp_Check_Sum2;
    Temp_Check_Sum += Temp_Check_Sum2 >> 8;

    if (Temp_V_Parity_Test != GUI_EEPROM_TEMP_DATA_BUFFER[8] or Temp_H_Parity_Test != GUI_EEPROM_TEMP_DATA_BUFFER[9] or Temp_Check_Sum != GUI_EEPROM_TEMP_DATA_BUFFER[10]) {
      EEPROM_WRITE_CYCLE_COUNTER[0] = 0;
      EEPROM_WRITE_CYCLE_COUNTER[1] = 0;
      EEPROM_WRITE_CYCLE_COUNTER[2] = 0;
      for (uint8_t X = 0; X < 11; X++) GUI_EEPROM_TEMP_DATA_BUFFER[X] = 0;
      RTX_GUI_UPDATE_REGISTER(5);
      return false;
    }

    for (uint8_t X = 0; X < 8; X++) GUI_Registers[X + (EEPROM_WRITE_CYCLE_COUNTER[0] * 8)] = GUI_EEPROM_TEMP_DATA_BUFFER[X];
  }
  EEPROM_WRITE_CYCLE_COUNTER[0] = 0;
  EEPROM_WRITE_CYCLE_COUNTER[1] = 0;
  EEPROM_WRITE_CYCLE_COUNTER[2] = 0;
  for (uint8_t X = 0; X < 11; X++) GUI_EEPROM_TEMP_DATA_BUFFER[X] = 0;
  RTX_GUI_UPDATE_REGISTER(5);
  return true;
}

void RTX_GUI_UPDATE() {
  uint8_t PacketNumber = 0;
  bitWrite(PacketNumber, 0, bitRead(TwoWire_G_BufferRX[1], 0));
  bitWrite(PacketNumber, 1, bitRead(TwoWire_G_BufferRX[1], 1));
  if (PacketNumber == GUI_RX_PacketCount or PacketNumber == 0) {
    if (PacketNumber == 0) {
      for (uint8_t X = 0; X < 2; X++) bitWrite(GUI_RX_Buffer[0], X, bitRead(TwoWire_G_BufferRX[1], X + 2));
      for (uint8_t X = 0; X < 4; X++) bitWrite(GUI_RX_Buffer[1], X, bitRead(TwoWire_G_BufferRX[1], X + 4));
      GUI_RX_PacketCount = 0;
    }
    if (PacketNumber == 1) {
      for (uint8_t X = 0; X < 4; X++) bitWrite(GUI_RX_Buffer[1], X + 4, bitRead(TwoWire_G_BufferRX[1], X + 2));
      if (GUI_RX_Buffer[0]) {
        for (uint8_t X = 0; X < 2; X++) bitWrite(GUI_RX_Buffer[2], X, bitRead(TwoWire_G_BufferRX[1], X + 6));
      }
      else {
        GUI_RX_PacketCount = 0;
        GUI_Compile();
        return;
      }
    }
    if (PacketNumber == 2) {
      for (uint8_t X = 0; X < 6; X++) bitWrite(GUI_RX_Buffer[2], X + 2, bitRead(TwoWire_G_BufferRX[1], X + 2));
      GUI_RX_PacketCount = 0;
      GUI_Compile();
      return;
    }
    GUI_RX_PacketCount++;
  }
}

void RTX_GUI_UPDATE_VALUE(uint8_t UPDATE_COMMAND) {
  switch (UPDATE_COMMAND) {
    case 0:
      Mode = GUI_Registers[217] & B00000011;
      TallyRedLightEN = bitRead(GUI_Registers[216], 0);
      TallyGreenLightEN = bitRead(GUI_Registers[216], 1);
      TallyRedFadeEN = bitRead(GUI_Registers[216], 2);
      SwitchEN = bitRead(GUI_Registers[218], 0);
      if (bitRead(GUI_Registers[251], 0)) {
        EEPROM_ERROR = !GUI_EEPROM_READ();
        RTX_GUI_UPDATE_REGISTER(2);
        RTX_GUI_UPDATE_REGISTER(3);
      }
      EEPROM_ERROR = bitRead(GUI_Registers[251], 2);
      if (!EEPROM_WRITING) EEPROM_WRITING = bitRead(GUI_Registers[251], 1);
      RTX_GUI_UPDATE_REGISTER(4);
      bool TempTallyCheckDone = false;
      for (uint8_t X = 0; X < 8 and !TempTallyCheckDone; X++)
        if (bitRead(GUI_Registers[222], X))
          if (bitRead(TallyStatus[X], 0)) {
            TallyStatusLED[0] = true;
            TempTallyCheckDone = true;
          }
      if (!TempTallyCheckDone) TallyStatusLED[0] = false;
      TempTallyCheckDone = false;
      for (uint8_t X = 0; X < 8 and !TempTallyCheckDone; X++)
        if (bitRead(GUI_Registers[223], X))
          if (bitRead(TallyStatus[X], 1)) {
            TallyStatusLED[1] = true;
            TempTallyCheckDone = true;
          }
      if (!TempTallyCheckDone) TallyStatusLED[1] = false;
      break;
    case 1:
      break;
  }
}

void RTX_GUI_UPDATE_REGISTER(uint8_t UPDATE_COMMAND) {
  switch (UPDATE_COMMAND) {
    case 0:
      GUI_Registers[248] = (millis() / 60000);
      bitWrite(GUI_Registers[240], 0, Comp_Connected);
      bitWrite(GUI_Registers[240], 1, Tally_Connected);
      break;
    case 1:
      bitWrite(GUI_Registers[217], 0, bitRead(Mode, 0));
      bitWrite(GUI_Registers[217], 1, bitRead(Mode, 1));
      break;
    case 2:
      bitWrite(GUI_Registers[251], 0, 0);
      break;
    case 3:
      bitWrite(GUI_Registers[251], 2, EEPROM_ERROR);
      break;
    case 4:
      bitWrite(GUI_Registers[251], 1, EEPROM_WRITING);
      break;
    case 5:
      for (uint8_t X = 0; X < 4; X++) GUI_Registers[252 + X] = EEPROM.read(1020 + X);
      break;
    case 6:
      break;
    case 7:
      break;
    case 8:
      break;
    case 9:
      break;
  }
}

void RTX_GUI_SEND_RUN() {
  if (GUI_Sending) {
    if (GUI_TX_PacketCount == 0) {
      TwoWire_G_BufferTX[0] = 5;
      TwoWire_G_BufferTX[1] = GUI_TX_Buffer[0];
      TwoWire_RTXMessage(1);
    }
    if (GUI_TX_PacketCount == 1) {
      TwoWire_G_BufferTX[0] = 5;
      TwoWire_G_BufferTX[1] = GUI_TX_Buffer[1];
      TwoWire_RTXMessage(1);
    }
    if (GUI_TX_PacketCount == 2) {
      TwoWire_G_BufferTX[0] = 5;
      TwoWire_G_BufferTX[1] = GUI_TX_Buffer[2];
      TwoWire_RTXMessage(1);
      GUI_Sending = false;
    }
    GUI_TX_PacketCount++;
  }
  else GUI_TX_PacketCount = 0;
}

void GUI_Compile() {
  for (uint8_t X = 0; X < 3; X++) GUI_TX_Buffer[X] = 0;
  if (GUI_RX_Buffer[0]) GUI_Registers[GUI_RX_Buffer[1]] = GUI_RX_Buffer[2];
  bitWrite(GUI_TX_Buffer[0], 2, GUI_RX_Buffer[0]);
  for (uint8_t X = 0; X < 4; X++) bitWrite(GUI_TX_Buffer[0], X + 4, bitRead(GUI_RX_Buffer[1], X));
  GUI_TX_Buffer[1] = 1;
  for (uint8_t X = 0; X < 4; X++) bitWrite(GUI_TX_Buffer[1], X + 2, bitRead(GUI_RX_Buffer[1], X + 4));
  for (uint8_t X = 0; X < 2; X++) bitWrite(GUI_TX_Buffer[1], X + 6, bitRead(GUI_Registers[GUI_RX_Buffer[1]], X));
  GUI_TX_Buffer[2] = 2;
  for (uint8_t X = 0; X < 6; X++) bitWrite(GUI_TX_Buffer[2], X + 2, bitRead(GUI_Registers[GUI_RX_Buffer[1]], X + 2));
  GUI_Sending = true;
}

void RTX_TallyCheck() {
  uint8_t PacketNumber = 0;
  bitWrite(PacketNumber, 0, bitRead(TwoWire_G_BufferRX[1], 0));
  bitWrite(PacketNumber, 1, bitRead(TwoWire_G_BufferRX[1], 1));
  if (PacketNumber == TallyPacketCount or PacketNumber == 0) {
    if (PacketNumber == 0) {
      for (int I = 0; I < 6; I++) {
        bitWrite(TallyStatus[I], 0, bitRead(TwoWire_G_BufferRX[1], (I + 2)));
      }
      TallyPacketCount = 1;
    }
    if (PacketNumber == 1) {
      for (int I = 0; I < 6; I++) {
        bitWrite(TallyStatus[I], 1, bitRead(TwoWire_G_BufferRX[1], (I + 2)));
      }
      TallyPacketCount = 2;
    }
    if (PacketNumber == 2) {
      for (int I = 0; I < 2; I++) {
        bitWrite(TallyStatus[I + 6], 0, bitRead(TwoWire_G_BufferRX[1], (I + 2)));
        bitWrite(TallyStatus[I + 6], 1, bitRead(TwoWire_G_BufferRX[1], (I + 4)));
      }
      Comp_Connected = bitRead(TwoWire_G_BufferRX[1], 6);
      Tally_Connected = bitRead(TwoWire_G_BufferRX[1], 7);
      if (!Tally_Connected) for (uint8_t X = 0; X < 8; X++) TallyStatus[X] = 0;
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
  if (!EEPROM_ERROR) {
    if (Comp_Connected) {
      switch (Mode & B00000011) {
        case 0:
          analogWrite(LED_StatusPIN[1], 100);
          analogWrite(LED_StatusPIN[0], 0);
          break;
        case 1:
          analogWrite(LED_StatusPIN[0], 100);
          analogWrite(LED_StatusPIN[1], 0);
          break;
        case 2:
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
          break;
        case 3:
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
          break;
      }
    }
    else {
      analogWrite(LED_StatusPIN[1], 0);
      if ((millis() - Status_LED_Timer) > Status_PWM_Delay) {
        Status_LED_Timer = millis();
        if (Status_LED_State) {
          Status_LED_State = false;
          Status_PWM_Delay = 600;
          analogWrite(LED_StatusPIN[0], 0);
        }
        else {
          Status_LED_State = true;
          Status_PWM_Delay = 50;
          analogWrite(LED_StatusPIN[0], 100);
        }
      }
    }


    if (Tally_Connected) {
      if (TallyStatusLED[0]) {
        analogWrite(LED_TallyPIN[1], 0);
        if (TallyRedLightEN) {
          if (TallyRedFadeEN) {
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
          else analogWrite(LED_TallyPIN[0], 100);
        }
        else analogWrite(LED_TallyPIN[0], 0);
      }
      else {
        Tally_LED_State = true;
        Tally_PWN_Counter = 100;
        Tally_PWM_Delay = 0;
        analogWrite(LED_TallyPIN[0], 0);
        if (TallyStatusLED[1] and TallyGreenLightEN) analogWrite(LED_TallyPIN[1], 100);
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
  else {
    if ((millis() - EEPROM_LED_Timer) > EEPROM_LED_Delay) {
      EEPROM_LED_Timer = millis();
      switch (EEPROM_LED_STATE) {
        case 0:
          analogWrite(LED_TallyPIN[0], 0);
          analogWrite(LED_TallyPIN[1], 0);
          analogWrite(LED_StatusPIN[0], 0);
          analogWrite(LED_StatusPIN[1], 0);
          EEPROM_LED_Delay = 400;
          break;
        case 1:
          analogWrite(LED_TallyPIN[0], 100);
          analogWrite(LED_TallyPIN[1], 0);
          analogWrite(LED_StatusPIN[0], 100);
          analogWrite(LED_StatusPIN[1], 0);
          EEPROM_LED_Delay = 130;
          break;
        case 2:
          analogWrite(LED_TallyPIN[0], 0);
          analogWrite(LED_TallyPIN[1], 100);
          analogWrite(LED_StatusPIN[0], 0);
          analogWrite(LED_StatusPIN[1], 100);
          EEPROM_LED_Delay = 130;
          break;
      }
      if (EEPROM_LED_STATE < 2) EEPROM_LED_STATE++;
      else EEPROM_LED_STATE = 0;
    }
  }
}

void Button_Execute() {
  if (!EEPROM_ERROR) {
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
        ButtonTimerState[0] = false;
        if (!((millis() - ButtonTimer[0]) >= 300)) bitWrite(Mode, 0, !bitRead(Mode, 0));
        RTX_GUI_UPDATE_REGISTER(1);
      }
    }
    if (ButtonState[0] and !ButtonTimerState[0] and (millis() - ButtonTimer[0]) >= 300) {
      ButtonTimerState[0] = true;
      bitWrite(Mode, 1, !bitRead(Mode, 1));
      RTX_GUI_UPDATE_REGISTER(1);
    }

    Mode = Mode & B00000011;

    for (uint8_t X = 0; X < 3; X++) {
      if (CheckButton(GUI_Registers[(Mode * 24) + (X * 8)]) and !ButtonState[X + 1]) {
        delay(5);
        if (CheckButton(GUI_Registers[(Mode * 24) + (X * 8)])) {
          ButtonState[X + 1] = true;
          ButtonTimer[X + 1] = millis();
          RunAction(GUI_Registers[(Mode * 24) + (X * 8) + 1]);
        }
      }
      if (!CheckButton(GUI_Registers[(Mode * 24) + (X * 8)]) and ButtonState[X + 1]) {
        delay(5);
        if (!CheckButton(GUI_Registers[(Mode * 24) + (X * 8)])) {
          ButtonState[X + 1] = false;
          ButtonTimerState[X + 1] = false;
          if ((millis() - ButtonTimer[X + 1]) >= (GUI_Registers[(Mode * 24) + (X * 8) + 7] + (GUI_Registers[(Mode * 24) + (X * 8) + 6] << 8)))
            RunAction(GUI_Registers[(Mode * 24) + (X * 8) + 4]);
          else
            RunAction(GUI_Registers[(Mode * 24) + (X * 8) + 3]);
        }
      }
      if (ButtonState[X + 1] and !ButtonTimerState[X + 1] and (millis() - ButtonTimer[X + 1]) >= (GUI_Registers[(Mode * 24) + (X * 8) + 7] + (GUI_Registers[(Mode * 24) + (X * 8) + 6] << 8))) {
        ButtonTimerState[X + 1] = true;
        RunAction(GUI_Registers[(Mode * 24) + (X * 8) + 2]);
      }
    }
  }
}

bool CheckButton(uint8_t ButtonCheckCommand) {
  for (uint8_t X = 0; X < 3; X++)
    if (bitRead(ButtonCheckCommand, X) and !digitalRead(ButtonPIN[X + 1])) return true;
  return false;
}

void RunAction(uint8_t ActionCommand) {
  if (bitRead(ActionCommand, 7) and SwitchEN) {
    if (bitRead(ActionCommand, 6)) {
      if ((millis() - ButtonTimeOut) >= 300) {
        ButtonTimeOut = millis();
        TwoWire_G_BufferTX[0] = 1;
        TwoWire_G_BufferTX[1] = (ActionCommand & B00011111);
        TwoWire_RTXMessage(1);
      }
    }
    else {
      switch (ActionCommand & B00011111) {
        case 0:
          if (Tally_Connected) {
            if ((bitRead(TallyStatus[4], 0) or bitRead(TallyStatus[5], 0)) and !(bitRead(TallyStatus[4], 0) and bitRead(TallyStatus[5], 0))) {
              if (bitRead(TallyStatus[5], 0) and !bitRead(TallyStatus[4], 1)) {
                TwoWire_G_BufferTX[0] = 1;
                TwoWire_G_BufferTX[1] = 30;
                TwoWire_RTXMessage(1);
              }
              else if (bitRead(TallyStatus[4], 0) and !bitRead(TallyStatus[5], 1)) {
                TwoWire_G_BufferTX[0] = 1;
                TwoWire_G_BufferTX[1] = 29;
                TwoWire_RTXMessage(1);
              }
            }
          }
          break;
        case 1:
          if ((millis() - ButtonTimeOut) >= 300) {
            ButtonTimeOut = millis();
            if (Tally_Connected) {
              if (bitRead(TallyStatus[4], 0) or bitRead(TallyStatus[5], 0)) {
                TwoWire_G_BufferTX[0] = 1;
                TwoWire_G_BufferTX[1] = 27;
                TwoWire_RTXMessage(1);
              }
              else {
                TwoWire_G_BufferTX[0] = 1;
                TwoWire_G_BufferTX[1] = 26;
                TwoWire_RTXMessage(1);
              }
            }
            else {
              TwoWire_G_BufferTX[0] = 1;
              TwoWire_G_BufferTX[1] = 27;
              TwoWire_RTXMessage(1);
            }
          }
          break;
        case 2:
          if ((millis() - ButtonTimeOut) >= 300) {
            ButtonTimeOut = millis();
            if (Tally_Connected) {
              if (bitRead(TallyStatus[4], 0) or bitRead(TallyStatus[5], 0)) {
                TwoWire_G_BufferTX[0] = 1;
                TwoWire_G_BufferTX[1] = 28;
                TwoWire_RTXMessage(1);
              }
              else {
                TwoWire_G_BufferTX[0] = 1;
                TwoWire_G_BufferTX[1] = 25;
                TwoWire_RTXMessage(1);
              }
            }
            else {
              TwoWire_G_BufferTX[0] = 1;
              TwoWire_G_BufferTX[1] = 28;
              TwoWire_RTXMessage(1);
            }
          }
          break;

        case 3:
          break;
        case 4:
          break;
        case 5:
          break;
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
