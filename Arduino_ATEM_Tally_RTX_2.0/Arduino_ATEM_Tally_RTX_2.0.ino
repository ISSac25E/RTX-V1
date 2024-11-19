//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

const uint8_t TwoWire_SlaveIP = 2;
const uint8_t TwoWire_MasterIP = 0;

uint32_t TwoWire_ConnectionTimer = 0;
uint8_t TwoWire_RX_BitBuffer[5];
bool TwoWire_TX_BitBuffer[21];
bool TwoWire_Connected = false;

bool  TwoWire_Status[7];

uint8_t TwoWire_TX_Buffer[2];
uint8_t TwoWire_RX_Buffer[2];

//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
int X = 0;
long unsigned int Y = 0;

EthernetUDP UDP;

byte ARD_MAC[] = {0x90, 0xA2, 0xDA, 0x00, 0xE8, 0xE9};
IPAddress ARD_IP(192, 168, 86, 177);
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

IPAddress ATEM_V_IP;
uint16_t ATEM_V_LOCAL_PORT;
uint16_t ATEM_V_PacketCounter;
bool ATEM_V_Initialized;
char ATEM_V_PacketBuffer[96];
uint16_t ATEM_V_SessionID;
uint16_t ATEM_V_LastRemotePacketID;
uint8_t ATEM_V_Answer[36];

uint8_t ATEM_V_ProgramInput;
uint8_t ATEM_V_PreviewInput;
uint8_t ATEM_V_TallyInput[8];
bool ATEM_V_TransitionPreview;
uint16_t ATEM_V_TransitionPosition;
uint8_t ATEM_V_KeyerNextTransition;
uint8_t ATEM_V_TransitionStyle;
bool ATEM_V_UpStreamKeyState[4];
bool ATEM_V_DowntreamKeyState[2];
bool ATEM_V_DownStreamKeyTie[2];
uint8_t ATEM_V_FrameCount;
uint8_t ATEM_V_FTB_FrameCount;
uint8_t ATEM_V_AUX_Source[3];
uint8_t ATEM_V_MediaPlayerType[2];
uint8_t ATEM_V_MediaPlayerStill[2];
uint8_t ATEM_V_MediaPlayerClip[2];


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

uint32_t TallyUpdateTimer = 0;

uint8_t Previous_Tally[8];

void setup() {
  TwoWire_PinSet(0, 0);
  TwoWire_PinSet(1, 0);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  ATEM_Setup(IPAddress(192, 168, 86, 68), 56417);
  Ethernet.begin(ARD_MAC, ARD_IP);
  ATEM_Connect_Stage1();
  while (X != 20) {
    X = 0;
    ATEM_Connect_Stage1();
    while (X != 20 and Y < 10000) {
      X = UDP.parsePacket();
      Y++;
    }
    Y = 0;
  }
  ATEM_Connect_Stage2();
  Y = millis();
}

void loop() {
  TwoWire_RTXMessage(0);
  ATEM_RUN();
  TallyUpdate_TwoWire();
  if((millis() - TallyUpdateTimer) >= 500) RTX_TallySend();
  digitalWrite(5, !bitRead(ATEM_V_TallyInput[4], 0));
  digitalWrite(6, !bitRead(ATEM_V_TallyInput[5], 0));
  if (digitalRead(5) == LOW) digitalWrite(2, HIGH);
  else digitalWrite(2, !bitRead(ATEM_V_TallyInput[4], 1));
  if (digitalRead(6) == LOW)   digitalWrite(3, HIGH);
  else digitalWrite(3, !bitRead(ATEM_V_TallyInput[5], 1));
}

void TallyUpdate_TwoWire() {
  bool TallySame = true;
  for (int I = 0; I < 8; I++) {
    if (ATEM_V_TallyInput[I] != Previous_Tally[I]) {
      TallySame = false;
      Previous_Tally[I] = ATEM_V_TallyInput[I];
    }
  }
  if (!TallySame) RTX_TallySend();
}

void RTX_TallySend() {
  TwoWire_TX_Buffer[0] = 3;
  TwoWire_TX_Buffer[1] = 0;
  for (int I = 0; I < 6; I++) {
    bitWrite(TwoWire_TX_Buffer[1], (I + 2), bitRead(ATEM_V_TallyInput[I],0));
  }
  TwoWire_RTXMessage(1);
  TwoWire_TX_Buffer[1] = 1;
  for (int I = 0; I < 6; I++) {
    bitWrite(TwoWire_TX_Buffer[1], (I + 2), bitRead(ATEM_V_TallyInput[I],1));
  }
  TwoWire_RTXMessage(1);
  TwoWire_TX_Buffer[1] = 2;
  for (int I = 0; I < 2; I++) {
    bitWrite(TwoWire_TX_Buffer[1], (I + 2), bitRead(ATEM_V_TallyInput[I+6],0));
  }
  for (int I = 0; I < 2; I++) {
    bitWrite(TwoWire_TX_Buffer[1], (I + 4), bitRead(ATEM_V_TallyInput[I+6],1));
  }
  TwoWire_RTXMessage(1);

  
  TwoWire_TX_Buffer[0] = 4;
  TwoWire_TX_Buffer[1] = 0;
  for (int I = 0; I < 6; I++) {
    bitWrite(TwoWire_TX_Buffer[1], (I + 2), bitRead(ATEM_V_TallyInput[I],0));
  }
  TwoWire_RTXMessage(1);
  TwoWire_TX_Buffer[1] = 1;
  for (int I = 0; I < 6; I++) {
    bitWrite(TwoWire_TX_Buffer[1], (I + 2), bitRead(ATEM_V_TallyInput[I],1));
  }
  TwoWire_RTXMessage(1);
  TwoWire_TX_Buffer[1] = 2;
  for (int I = 0; I < 2; I++) {
    bitWrite(TwoWire_TX_Buffer[1], (I + 2), bitRead(ATEM_V_TallyInput[I+6],0));
  }
  for (int I = 0; I < 2; I++) {
    bitWrite(TwoWire_TX_Buffer[1], (I + 4), bitRead(ATEM_V_TallyInput[I+6],1));
  }
  TwoWire_RTXMessage(1);
  TallyUpdateTimer = millis();
}

void ATEM_Setup(IPAddress IP, uint16_t LocalPort) {
  ATEM_V_IP = IP;
  ATEM_V_LOCAL_PORT = LocalPort;
}

void ATEM_Connect_Stage1() {
  ATEM_V_PacketCounter = 1;
  ATEM_V_Initialized = false;
  UDP.begin(ATEM_V_LOCAL_PORT);
  byte ATEM_T_ConnectByte[] = {
    0x10, 0x14, 0x53, 0xAB, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3A,
    0x00, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00
  };
  UDP.beginPacket(ATEM_V_IP,  9910);
  UDP.write(ATEM_T_ConnectByte, 20);
  UDP.endPacket();
}

void ATEM_Connect_Stage2() {
  while (!UDP.available()) {}
  UDP.read(ATEM_V_PacketBuffer, 20);
  ATEM_V_SessionID = ATEM_V_PacketBuffer[15];
  byte ATEM_T_ConnectByte2[] = {
    0x80, 0x0c, 0x53, 0xab, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x03, 0x00, 0x00
  };
  UDP.beginPacket(ATEM_V_IP,  9910);
  UDP.write(ATEM_T_ConnectByte2, 12);
  UDP.endPacket();
}

void ATEM_RUN() {
  uint16_t ATEM_T_PacketSize = UDP.parsePacket();
  if (UDP.available() && ATEM_T_PacketSize != 0)   {
    UDP.read(ATEM_V_PacketBuffer, 12);
    uint16_t ATEM_T_PacketLength = word(ATEM_V_PacketBuffer[0] & B00000111, ATEM_V_PacketBuffer[1]);
    ATEM_V_LastRemotePacketID = word(ATEM_V_PacketBuffer[10], ATEM_V_PacketBuffer[11]);
    uint8_t ATEM_T_Command = ATEM_V_PacketBuffer[0] & B11111000;
    bool ATEM_T_CommandACK = ATEM_T_Command & B00001000 ? true : false;
    if (ATEM_T_PacketSize == ATEM_T_PacketLength) {
      if (!ATEM_V_Initialized && ATEM_T_PacketSize == 12) {
        ATEM_V_Initialized = true;
      }
      if (ATEM_T_PacketLength > 12) {
        ATEM_ParsePacket(ATEM_T_PacketLength);
      }
      if (ATEM_T_CommandACK) {
        ATEM_SendAnswerPacket(ATEM_V_LastRemotePacketID);
      }
    }
    else {
      while (UDP.available()) {
        UDP.read(ATEM_V_PacketBuffer, 96);
      }
    }
  }
}

void ATEM_ParsePacket(uint16_t ATEM_T_PacketLength2) {
  bool ATEM_ParsePacket_Done = false;
  uint8_t ATEM_T_Index;
  uint16_t ATEM_T_IndexPointer = 12;
  while (ATEM_T_IndexPointer < ATEM_T_PacketLength2 and !ATEM_ParsePacket_Done) {
    UDP.read(ATEM_V_PacketBuffer, 2);
    uint16_t ATEM_T_CommandLength = word(0, ATEM_V_PacketBuffer[1]);
    if (ATEM_T_CommandLength > 2 && ATEM_T_CommandLength <= 96) {
      UDP.read(ATEM_V_PacketBuffer, ATEM_T_CommandLength - 2);
      char ATEM_T_CommandString[] = {
        ATEM_V_PacketBuffer[-2 + 4],
        ATEM_V_PacketBuffer[-2 + 5],
        ATEM_V_PacketBuffer[-2 + 6],
        ATEM_V_PacketBuffer[-2 + 7], '\0'
      };
      if (strcmp( ATEM_T_CommandString, "PrgI") == 0) {
        ATEM_V_ProgramInput = ATEM_V_PacketBuffer[-2 + 8 + 1];
      }
      if (strcmp( ATEM_T_CommandString, "PrvI") == 0) {
        ATEM_V_PreviewInput = ATEM_V_PacketBuffer[-2 + 8 + 1];
      }
      if (strcmp( ATEM_T_CommandString, "TlIn") == 0) {
        ATEM_V_TallyInput[0] = ATEM_V_PacketBuffer[-2 + 8 + 2];
        ATEM_V_TallyInput[1] = ATEM_V_PacketBuffer[-2 + 8 + 3];
        ATEM_V_TallyInput[2] = ATEM_V_PacketBuffer[-2 + 8 + 4];
        ATEM_V_TallyInput[3] = ATEM_V_PacketBuffer[-2 + 8 + 5];
        ATEM_V_TallyInput[4] = ATEM_V_PacketBuffer[-2 + 8 + 6];
        ATEM_V_TallyInput[5] = ATEM_V_PacketBuffer[-2 + 8 + 7];
        ATEM_V_TallyInput[6] = ATEM_V_PacketBuffer[-2 + 8 + 8];
        ATEM_V_TallyInput[7] = ATEM_V_PacketBuffer[-2 + 8 + 9];
      }
      if (strcmp( ATEM_T_CommandString, "Time") == 0) {}
      if (strcmp( ATEM_T_CommandString, "TrPr") == 0) {
        ATEM_V_TransitionPreview = ATEM_V_PacketBuffer[-2 + 8 + 1] > 0 ? true : false;
      }
      if (strcmp( ATEM_T_CommandString, "TrPs") == 0) {
        ATEM_V_FrameCount = ATEM_V_PacketBuffer[-2 + 8 + 2];
        ATEM_V_TransitionPosition = ATEM_V_PacketBuffer[-2 + 8 + 4] * 256 + ATEM_V_PacketBuffer[-2 + 8 + 5];
      }
      if (strcmp( ATEM_T_CommandString, "TrSS") == 0) {
        ATEM_V_KeyerNextTransition = ATEM_V_PacketBuffer[-2 + 8 + 2] & B11111;
        ATEM_V_TransitionStyle = ATEM_V_PacketBuffer[-2 + 8 + 1];
      }
      if (strcmp( ATEM_T_CommandString, "FtbS") == 0) {
        ATEM_V_FTB_FrameCount = ATEM_V_PacketBuffer[-2 + 8 + 2];
      }
      if (strcmp( ATEM_T_CommandString, "DskS") == 0) {
        ATEM_T_Index = ATEM_V_PacketBuffer[-2 + 8 + 0];
        if (ATEM_T_Index >= 0 && ATEM_T_Index <= 1) {
          ATEM_V_DowntreamKeyState[ATEM_T_Index] = ATEM_V_PacketBuffer[-2 + 8 + 1] > 0 ? true : false;
        }
      }
      if (strcmp( ATEM_T_CommandString, "DskP") == 0) {
        ATEM_T_Index = ATEM_V_PacketBuffer[-2 + 8 + 0];
        if (ATEM_T_Index >= 0 && ATEM_T_Index <= 1) {
          ATEM_V_DownStreamKeyTie[ATEM_T_Index] = ATEM_V_PacketBuffer[-2 + 8 + 1] > 0 ? true : false;
        }
      }
      if (strcmp( ATEM_T_CommandString, "KeOn") == 0) {
        ATEM_T_Index = ATEM_V_PacketBuffer[-2 + 8 + 1];
        if (ATEM_T_Index >= 0 && ATEM_T_Index <= 3) {
          ATEM_V_UpStreamKeyState[ATEM_T_Index] = ATEM_V_PacketBuffer[-2 + 8 + 2] > 0 ? true : false;
        }
      }
      if (strcmp( ATEM_T_CommandString, "ColV") == 0) {}
      if (strcmp( ATEM_T_CommandString, "MPCE") == 0) {
        ATEM_T_Index = ATEM_V_PacketBuffer[-2 + 8 + 0];
        if (ATEM_T_Index >= 0 && ATEM_T_Index <= 1) {
          ATEM_V_MediaPlayerType[ATEM_T_Index] = ATEM_V_PacketBuffer[-2 + 8 + 1];
          ATEM_V_MediaPlayerStill[ATEM_T_Index] = ATEM_V_PacketBuffer[-2 + 8 + 2];
          ATEM_V_MediaPlayerClip[ATEM_T_Index] = ATEM_V_PacketBuffer[-2 + 8 + 3];
        }
      }
      if (strcmp( ATEM_T_CommandString, "AuxS") == 0) {
        ATEM_T_Index = ATEM_V_PacketBuffer[-2 + 8 + 1];
        if (ATEM_T_Index >= 0 && ATEM_T_Index <= 2) {
          ATEM_V_AUX_Source[ATEM_T_Index] = ATEM_V_PacketBuffer[-2 + 8 + 1];
        }
      }
      if (ATEM_V_Initialized) {}
      ATEM_T_IndexPointer += ATEM_T_CommandLength;
    }
    else {
      ATEM_ParsePacket_Done = true;
      while (UDP.available()) {
        UDP.read(ATEM_V_PacketBuffer, 96);
      }
    }
  }
}

void ATEM_SendAnswerPacket(uint16_t ATEM_T_RemotePacketID) {
  memset(ATEM_V_Answer, 0, 12);
  ATEM_V_Answer[2] = 0x80;
  ATEM_V_Answer[3] = ATEM_V_SessionID;
  ATEM_V_Answer[4] = ATEM_T_RemotePacketID / 256;
  ATEM_V_Answer[5] = ATEM_T_RemotePacketID % 256;
  ATEM_V_Answer[9] = 0x41;
  uint16_t ATEM_T_PacketLength3 = 10 + 2;
  ATEM_V_Answer[0] = ATEM_T_PacketLength3 / 256;
  ATEM_V_Answer[1] = ATEM_T_PacketLength3 % 256;
  ATEM_V_Answer[0] |= B10000000;
  UDP.beginPacket(ATEM_V_IP,  9910);
  UDP.write(ATEM_V_Answer, ATEM_T_PacketLength3);
  UDP.endPacket();
}

void ATEM_SendCommandPacket(char ATEM_T_Command[4], uint8_t ATEM_T_CommandBytes[16], uint8_t ATEM_T_CommandByteLength) {
  if (ATEM_T_CommandByteLength <= 16) {
    memset(ATEM_V_Answer, 0, 36);
    ATEM_V_Answer[2] = 0x80;
    ATEM_V_Answer[3] = ATEM_V_SessionID;
    ATEM_V_Answer[10] = ATEM_V_PacketCounter / 256;
    ATEM_V_Answer[11] = ATEM_V_PacketCounter % 256;
    int X = 0;
    for (X = 0; X < 4; X++) {
      ATEM_V_Answer[12 + 4 + X] = ATEM_T_Command[X];
    }
    for (X = 0; X < ATEM_T_CommandByteLength; X++) {
      ATEM_V_Answer[12 + 4 + 4 + X] = ATEM_T_CommandBytes[X];
    }
    ATEM_V_Answer[12] = (4 + 4 + ATEM_T_CommandByteLength) / 256;
    ATEM_V_Answer[12 + 1] = (4 + 4 + ATEM_T_CommandByteLength) % 256;
    uint16_t ATEM_T_PacketLength4 = 10 + 2 + (4 + 4 + ATEM_T_CommandByteLength);
    ATEM_V_Answer[0] = ATEM_T_PacketLength4 / 256;
    ATEM_V_Answer[1] = ATEM_T_PacketLength4 % 256;
    ATEM_V_Answer[0] |= B00001000;
    UDP.beginPacket(ATEM_V_IP,  9910);
    UDP.write(ATEM_V_Answer, ATEM_T_PacketLength4);
    UDP.endPacket();
    ATEM_V_PacketCounter++;
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
  while ((micros() - TwoWire_Timer) < 30) {
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
