//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

const uint8_t TwoWire_G_SlaveIP = 2;
const uint8_t TwoWire_G_MasterIP = 0;

uint8_t TwoWire_G_BitBufferRX[5];
bool TwoWire_G_BitBufferTX[21];

uint32_t TwoWire_G_ConnectionTimer = 0;
bool TwoWire_G_Connected = false;

bool TwoWire_G_Status[7];

uint8_t TwoWire_G_BufferRX[2];
uint8_t TwoWire_G_BufferTX[2];

//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

uint8_t ATEM_PacketCount = 0;

bool ATEM_Connected = false;

uint32_t ATEM_ConnectionTimer = millis();

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

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
  TwoWireM_PinSet(0);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  ATEM_Setup(IPAddress(192, 168, 86, 68), 56417);
  Ethernet.begin(ARD_MAC, ARD_IP);
  ATEM_Connected = ATEM_Connect();
}

void loop() {
  TwoWire_RTXMessage(0);
  if(!ATEM_Connected) ATEM_Connected = ATEM_Connect();
  if(ATEM_Connected) ATEM_RUN();
  if((millis() - ATEM_ConnectionTimer) > 3000){
    ATEM_Connected = ATEM_CheckConnection();
    ATEM_ConnectionTimer = millis();
  }
  TallyUpdate_TwoWire();
  if((millis() - TallyUpdateTimer) >= 200) RTX_TallySend();
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
  TwoWire_G_BufferTX[0] = 3;
  TwoWire_G_BufferTX[1] = 0;
  for (int I = 0; I < 6; I++) {
    bitWrite(TwoWire_G_BufferTX[1], (I + 2), bitRead(ATEM_V_TallyInput[I],0));
  }
  TwoWire_RTXMessage(1);
  TwoWire_G_BufferTX[1] = 1;
  for (int I = 0; I < 6; I++) {
    bitWrite(TwoWire_G_BufferTX[1], (I + 2), bitRead(ATEM_V_TallyInput[I],1));
  }
  TwoWire_RTXMessage(1);
  TwoWire_G_BufferTX[1] = 2;
  for (int I = 0; I < 2; I++) {
    bitWrite(TwoWire_G_BufferTX[1], (I + 2), bitRead(ATEM_V_TallyInput[I+6],0));
  }
  for (int I = 0; I < 2; I++) {
    bitWrite(TwoWire_G_BufferTX[1], (I + 4), bitRead(ATEM_V_TallyInput[I+6],1));
  }
   bitWrite(TwoWire_G_BufferTX[1], 7, ATEM_Connected);
  TwoWire_RTXMessage(1);

  
  TwoWire_G_BufferTX[0] = 4;
  TwoWire_G_BufferTX[1] = 0;
  for (int I = 0; I < 6; I++) {
    bitWrite(TwoWire_G_BufferTX[1], (I + 2), bitRead(ATEM_V_TallyInput[I],0));
  }
  TwoWire_RTXMessage(1);
  TwoWire_G_BufferTX[1] = 1;
  for (int I = 0; I < 6; I++) {
    bitWrite(TwoWire_G_BufferTX[1], (I + 2), bitRead(ATEM_V_TallyInput[I],1));
  }
  TwoWire_RTXMessage(1);
  TwoWire_G_BufferTX[1] = 2;
  for (int I = 0; I < 2; I++) {
    bitWrite(TwoWire_G_BufferTX[1], (I + 2), bitRead(ATEM_V_TallyInput[I+6],0));
  }
  for (int I = 0; I < 2; I++) {
    bitWrite(TwoWire_G_BufferTX[1], (I + 4), bitRead(ATEM_V_TallyInput[I+6],1));
  }
  bitWrite(TwoWire_G_BufferTX[1], 7, ATEM_Connected);
  TwoWire_RTXMessage(1);
  TallyUpdateTimer = millis();
}

bool ATEM_CheckConnection(){
  if(ATEM_PacketCount > 0){
    ATEM_PacketCount = 0;
    return true;
  }
  else{
    ATEM_PacketCount = 0;
    return false;
  }
}

bool ATEM_Connect(){
  long unsigned int Y = 0;
  int X = 0;
  ATEM_Connect_Stage1();
    while (X != 20 and Y < 10000) {
      X = UDP.parsePacket();
      Y++;
    }
    ATEM_ConnectionTimer = millis();
    if(X == 20){
      ATEM_Connect_Stage2();
      return true;
    }
    else return false;
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
        ATEM_PacketCount++;
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
//    for (int I = 0; I < 3; I++) {
      TwoWireA_Run(1);
      if (TwoWire_G_Status[1]) return true;
      if (!TwoWire_G_Status[1] and TwoWire_G_Status[2])
        return false;
//    }
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
  while((millis() - TwoWireC_L_Timer) < TwoWireC_L_Interval){
    if(TwoWireH_Insert()){
      if(TwoWireD_Read(TwoWireC_L_LengthOne, TwoWireC_L_LengthTwo)){
        if(TwoWireI_CheckIP()) return true;
      }
      if(TwoWire_G_Status[4]) return false;
    }
  }
  return false;
}

bool TwoWireD_Read(bool TwoWireD_L_LengthOne, bool TwoWireD_L_LengthTwo) {
  uint16_t TwoWireD_L_Counter = 0;
  uint16_t TwoWireD_L_Delay = 200;
  
  TwoWireD_L_Delay *= 2.5;
  while (TwoWireD_L_Counter < TwoWireD_L_Delay){
    TwoWireD_L_Counter++;
    if(!TwoWireK_PinRead()){
      uint8_t TwoWireD_L_ReadHold = TwoWireE_BitRead(TwoWireD_L_LengthOne, TwoWireD_L_LengthTwo);
      if(TwoWireD_L_ReadHold == 0) return false;
      else if(TwoWireD_L_ReadHold == 1) return true;
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
      while (TwoWireK_PinRead() == TwoWireE_L_TempBitHold and TwoWireE_L_Counter < TwoWireE_L_Delay){
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
  while (TwoWireH_L_Counter < TwoWireH_L_Delay and TwoWireK_PinRead()){
    TwoWireH_L_Counter++;
    delayMicroseconds(2);
  }
  if(TwoWireH_L_Counter >= TwoWireH_L_Delay) return true;
  else return false;
}

bool TwoWireI_CheckIP() {
  if(TwoWire_G_BitBufferRX[0] == TwoWire_G_SlaveIP and TwoWire_G_BitBufferRX[1] == TwoWire_G_MasterIP) return true;
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
