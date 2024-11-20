//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

const uint8_t TwoWire_SlaveIP = 1;
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
  TwoWire_PinSet(0, 0);
  TwoWire_PinSet(1, 0);


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
    switch (TwoWire_RX_Buffer[0]) {
      case 3:
        switch (TwoWire_RX_Buffer[1]) {
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
    bitWrite(TwoWire_TX_Buffer[1], 0, 1);
    bitWrite(TwoWire_TX_Buffer[1], 1, CONNECTED);
    bitWrite(TwoWire_TX_Buffer[1], 2, ConnectionSleep);
    TwoWire_TX_Buffer[0] = 3;
    TwoWire_RTXMessage(1);
    TwoWire_TX_Buffer[0] = 4;
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
        bitWrite(TwoWire_TX_Buffer[1], 0, 1);
        bitWrite(TwoWire_TX_Buffer[1], 1, CONNECTED);
        bitWrite(TwoWire_TX_Buffer[1], 2, ConnectionSleep);
        TwoWire_TX_Buffer[0] = 3;
        TwoWire_RTXMessage(1);
        TwoWire_TX_Buffer[0] = 4;
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
