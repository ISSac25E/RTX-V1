//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

const uint8_t TwoWire_G_SlaveIP = 1;
const uint8_t TwoWire_G_MasterIP = 0;

uint8_t TwoWire_G_BitBufferRX[5];
bool TwoWire_G_BitBufferTX[21];

uint32_t TwoWire_G_ConnectionTimer = 0;
bool TwoWire_G_Connected = false;

bool TwoWire_G_Status[7];

uint8_t TwoWire_G_BufferRX[2];
uint8_t TwoWire_G_BufferTX[2];

//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

#include <SPI.h>
#include <UIPEthernet.h>
#include <UIPUdp.h>

byte ARD_MAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ARD_IP(192, 168, 86, 176);
IPAddress COMP_IP(192, 168, 86, 138);

unsigned int ARD_LCP = 8888;
unsigned int UDP_LCP = 51235;
unsigned int TCP_LCP = 51234;

unsigned long int TCP_WRITE_COUNT = 0;
unsigned long int TCP_READ_COUNT = 0;

unsigned long int SendInterval = 0;

unsigned long int BUTTON4_COUNT = 0;
unsigned long int BUTTON4_COUNTER = 0;

unsigned long int BUTTON3_COUNTER = 0;
unsigned long int BUTTON5_COUNTER = 0;

unsigned long int ConnectionTimer = 0;


uint8_t PACKET_COUNT = 0;

boolean DISCONNECTED_STAGE1 = false;
boolean CONNECTED = false;

boolean ConnectionSleep = false;

const char Companion_BankDown5[] = "bank-down 5 ";
const char Companion_BankUp5[] = "bank-up 5 ";


EthernetUDP UDP;
UIPClient UIP;

void setup() {
  TwoWireM_PinSet(0);


  Ethernet.begin(ARD_MAC, ARD_IP);
  UDP.begin(ARD_LCP);
}

void loop() {
  TwoWire_SendInterval();
  TwoWire_Execute();
  if (!ConnectionSleep) {
    if (CONNECTED)
      CheckInterval();
    else
      Reconnect();
  }
}

void TwoWire_Execute() {
  if (TwoWire_RTXMessage(0)) {
    switch (TwoWire_G_BufferRX[0]) {
      case 3:
        switch (TwoWire_G_BufferRX[1]) {
            break;
          case 25:
            if (CONNECTED) {
              for (int I = 0; I < 2; I++) {
                UDP.beginPacket(COMP_IP, UDP_LCP);
                UDP.write(Companion_BankDown5);
                UDP.write("25");
                UDP.endPacket();
              }
              UDP.beginPacket(COMP_IP, UDP_LCP);
              UDP.write(Companion_BankUp5);
              UDP.write("25");
              UDP.endPacket();
              ConnectionTimer = millis();
              TCP_WRITE_COUNT = millis();
            }
            break;

          case 26:
            if (CONNECTED) {
              for (int I = 0; I < 2; I++) {
                UDP.beginPacket(COMP_IP, UDP_LCP);
                UDP.write(Companion_BankDown5);
                UDP.write("26");
                UDP.endPacket();
              }
              UDP.beginPacket(COMP_IP, UDP_LCP);
              UDP.write(Companion_BankUp5);
              UDP.write("26");
              UDP.endPacket();
              ConnectionTimer = millis();
              TCP_WRITE_COUNT = millis();
            }
            break;
          case 27:
            if (CONNECTED) {
              for (int I = 0; I < 2; I++) {
                UDP.beginPacket(COMP_IP, UDP_LCP);
                UDP.write(Companion_BankDown5);
                UDP.write("27");
                UDP.endPacket();
              }
              UDP.beginPacket(COMP_IP, UDP_LCP);
              UDP.write(Companion_BankUp5);
              UDP.write("27");
              UDP.endPacket();
              ConnectionTimer = millis();
              TCP_WRITE_COUNT = millis();
            }
            break;
          case 28:
            if (CONNECTED) {
              for (int I = 0; I < 2; I++) {
                UDP.beginPacket(COMP_IP, UDP_LCP);
                UDP.write(Companion_BankDown5);
                UDP.write("28");
                UDP.endPacket();
              }
              UDP.beginPacket(COMP_IP, UDP_LCP);
              UDP.write(Companion_BankUp5);
              UDP.write("28");
              UDP.endPacket();
              ConnectionTimer = millis();
              TCP_WRITE_COUNT = millis();
            }
            break;
          case 29:
            if (CONNECTED) {
              for (int I = 0; I < 2; I++) {
                UDP.beginPacket(COMP_IP, UDP_LCP);
                UDP.write(Companion_BankDown5);
                UDP.write("29");
                UDP.endPacket();
              }
              UDP.beginPacket(COMP_IP, UDP_LCP);
              UDP.write(Companion_BankUp5);
              UDP.write("29");
              UDP.endPacket();
              ConnectionTimer = millis();
              TCP_WRITE_COUNT = millis();
            }
            break;

          case 30:
            if (CONNECTED) {
              for (int I = 0; I < 2; I++) {
                UDP.beginPacket(COMP_IP, UDP_LCP);
                UDP.write(Companion_BankDown5);
                UDP.write("30");
                UDP.endPacket();
              }
              UDP.beginPacket(COMP_IP, UDP_LCP);
              UDP.write(Companion_BankUp5);
              UDP.write("30");
              UDP.endPacket();
              ConnectionTimer = millis();
              TCP_WRITE_COUNT = millis();
            }
            break;
          case 31:
            if (CONNECTED) {
              for (int I = 0; I < 2; I++) {
                UDP.beginPacket(COMP_IP, UDP_LCP);
                UDP.write(Companion_BankDown5);
                UDP.write("31");
                UDP.endPacket();
              }
              UDP.beginPacket(COMP_IP, UDP_LCP);
              UDP.write(Companion_BankUp5);
              UDP.write("31");
              UDP.endPacket();
              ConnectionTimer = millis();
              TCP_WRITE_COUNT = millis();
            }
            break;
          case 32:
            if (CONNECTED) {
              for (int I = 0; I < 2; I++) {
                UDP.beginPacket(COMP_IP, UDP_LCP);
                UDP.write(Companion_BankDown5);
                UDP.write("32");
                UDP.endPacket();
              }
              UDP.beginPacket(COMP_IP, UDP_LCP);
              UDP.write(Companion_BankUp5);
              UDP.write("32");
              UDP.endPacket();
              ConnectionTimer = millis();
              TCP_WRITE_COUNT = millis();
            }
            break;
        }
        break;
      case 4:
        break;
    }
  }
}

void TwoWire_SendInterval() {
  if ((millis() - SendInterval) > 500) {
    bitWrite(TwoWire_G_BufferTX[1], 0, 1);
    bitWrite(TwoWire_G_BufferTX[1], 1, CONNECTED);
    bitWrite(TwoWire_G_BufferTX[1], 2, ConnectionSleep);
    TwoWire_G_BufferTX[0] = 3;
    TwoWire_RTXMessage(1);
    TwoWire_G_BufferTX[0] = 4;
    TwoWire_RTXMessage(1);
    SendInterval = millis();
  }
}

void CheckInterval() {
  if ((millis() - ConnectionTimer) > 3600000) {
    UIPSleep();
  }
  while (UIP.available()) {
    UIP.read();
    PACKET_COUNT++;
  }
  if ((millis() - TCP_WRITE_COUNT) >= 300) {
    UIP.println("bank-up 5 8");
    TCP_WRITE_COUNT = millis();
  }
  if ((millis() - TCP_READ_COUNT) >= 600) {
    if (PACKET_COUNT == 0 and !DISCONNECTED_STAGE1)
      DISCONNECTED_STAGE1 = true;
    else {
      if (PACKET_COUNT == 0 and DISCONNECTED_STAGE1) {
        CONNECTED = false;
        bitWrite(TwoWire_G_BufferTX[1], 0, 1);
        bitWrite(TwoWire_G_BufferTX[1], 1, CONNECTED);
        bitWrite(TwoWire_G_BufferTX[1], 2, ConnectionSleep);
        TwoWire_G_BufferTX[0] = 3;
        TwoWire_RTXMessage(1);
        TwoWire_G_BufferTX[0] = 4;
        TwoWire_RTXMessage(1);
        SendInterval = millis();
      }
      DISCONNECTED_STAGE1 = false;
    }
    PACKET_COUNT = 0;
    TCP_READ_COUNT = millis();
    TCP_WRITE_COUNT = (millis() - 300);
  }
}

void Reconnect() {
  UDP.stop();
  if (UIP.connect(COMP_IP, TCP_LCP)) {
    UDP.begin(ARD_LCP);
    CONNECTED = true;
    ConnectionTimer = millis();
    TCP_READ_COUNT = millis();
  }
}

void UIPSleep() {
  UIP.stop();
  ConnectionSleep = true;
  CONNECTED = false;
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
  uint32_t TwoWireC_L_Interval;
  if(TwoWireC_L_TimerLength) TwoWireC_L_Interval = 100;
  else TwoWireC_L_Interval = 20;
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
  uint16_t TwoWireD_L_Delay = 300;
  TwoWireD_L_Delay *= 2.5;
  while (TwoWireD_L_Counter < TwoWireD_L_Delay){
    TwoWireD_L_Counter++;
    if(!TwoWireK_PinRead()){
      uint8_t TwoWireD_L_ReadHold = TwoWireE_BitRead(TwoWireD_L_LengthOne, TwoWireD_L_LengthTwo);
      if(TwoWireD_L_ReadHold == 0) return false;
      else if(TwoWireD_L_ReadHold == 1) return true;
    }
  }
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
  TwoWireH_L_Delay *= 2.5;
  while (TwoWireH_L_Counter < TwoWireH_L_Delay and TwoWireK_PinRead()) TwoWireH_L_Counter++;
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
