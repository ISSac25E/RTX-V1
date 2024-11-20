//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

const uint8_t TwoWire_G_SlaveIP = 5;
const uint8_t TwoWire_G_MasterIP = 0;

uint8_t TwoWire_G_BitBufferRX[5];
bool TwoWire_G_BitBufferTX[21];

uint32_t TwoWire_G_ConnectionTimer = 0;
bool TwoWire_G_Connected = false;

bool TwoWire_G_Status[7];

uint8_t TwoWire_G_BufferRX[2];
uint8_t TwoWire_G_BufferTX[2];

//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲

uint8_t SelectedIP;
bool IP_Enter = true;
bool Command_Select = false;

bool ReadMode;
bool WriteMode;
bool DumpMode;
bool ExportBinary;

void setup() {
  TwoWireM_PinSet(0);
  Serial.begin(115200);
  Serial.println(F("ENTER SLAVEIP"));
}

void loop() {
  CheckSerial_Run();
  TwoWire_RTXMessage(0);
}

void CheckSerial_Run() {
  if (IP_Enter) {
    if (Serial.available()) {
      int SerialCheckTemp = SerialParse();
      if (SerialParse() == -1) {
        if (SerialCheckTemp >= 0 and SerialCheckTemp <= 7 and SerialCheckTemp != TwoWire_G_SlaveIP and SerialCheckTemp != TwoWire_G_MasterIP) {
          SelectedIP = SerialCheckTemp;
          Serial.print(F("SELECTED SLAVEIP: "));
          Serial.println(SelectedIP);
          Serial.println();
          IP_Enter = false;
          Command_Select = true;
          Serial.println(F("SELECT ACTION(R,W,D)"));
        }
      }
      while (Serial.available()) Serial.read();
    }
  }
  else if (Command_Select) {
    if (Serial.available()) {
      int SerialCheckTemp = SerialParse();
      if (bitRead(SerialCheckTemp, 15)) {
        bitWrite(SerialCheckTemp, 15, 0);
        if (SerialCheckTemp == 'R') {
          Serial.println(F("READ(0-255):"));
          ReadMode = true;
          Command_Select = false;
        }
        else if (SerialCheckTemp == 'W') {
          Serial.println(F("WRITE(0-255,0-255):"));
          WriteMode = true;
          Command_Select = false;
        }
        else if (SerialCheckTemp == 'D') {
          SerialCheckTemp = SerialParse();
          if (bitRead(SerialCheckTemp, 15) and SerialCheckTemp != -1) {
            if (SerialParse() == -1) {
              bitWrite(SerialCheckTemp, 15, 0);
              if (SerialCheckTemp == 'B') {
                ExportBinary = true;
                DumpMode = true;
                Command_Select = false;
              }
            }
          }
          else if (SerialCheckTemp == -1) {
            DumpMode = true;
            Command_Select = false;
            ExportBinary = false;
          }
        }
        else if (SerialCheckTemp == 'C') {
          Serial.println(F("CANCEL"));
          Serial.println();
          Serial.println(F("ENTER SLAVEIP"));
          Command_Select = false;
          IP_Enter = true;
        }
      }
      while (Serial.available()) Serial.read();
    }
  }
  else if (ReadMode) {
    if (Serial.available()) {
      int SerialCheckTemp = SerialParse();
      if (SerialParse() == -1) {
        if (bitRead(SerialCheckTemp, 15)) {
          bitWrite(SerialCheckTemp, 15, 0);
          if (SerialCheckTemp == 'C') {
            Serial.println(F("CANCEL"));
            Serial.println();
            Serial.println(F("SELECT ACTION(R,W,D)"));
            Command_Select = true;
            ReadMode = false;
          }
        }
        else if (SerialCheckTemp >= 0 and SerialCheckTemp <= 255) {
          uint8_t RTX_BufferSendTemp[2] = {0, 1};
          for (uint8_t X = 0; X < 4; X++) bitWrite(RTX_BufferSendTemp[0], X + 4, bitRead(SerialCheckTemp, X));
          for (uint8_t X = 0; X < 4; X++) bitWrite(RTX_BufferSendTemp[1], X + 2, bitRead(SerialCheckTemp, X + 4));
          TwoWire_G_BufferTX[0] = SelectedIP;
          for (uint8_t X = 0; X < 2; X++) {
            TwoWire_G_BufferTX[1] = RTX_BufferSendTemp[X];
            if (!TwoWire_RTXMessage(1)) {
              Serial.println(F("ERROR: CAN'T SEND"));
              Serial.println();
              Serial.println(F("READ(0-255):"));
              while (Serial.available()) Serial.read();
              return;
            }
          }
          uint32_t RTX_TimeOutTimer = millis();
          uint8_t PacketCount = 0;
          uint8_t RTX_BufferRecieveTemp[3] = {0, 0, 0};
          bool Recieved = false;
          while ((millis() - RTX_TimeOutTimer) < 1000 and !Recieved) {
            if (TwoWire_RTXMessage(0)) {
              if (TwoWire_G_BufferRX[0] == SelectedIP) {
                uint8_t PacketNumber = 0;
                bitWrite(PacketNumber, 0, bitRead(TwoWire_G_BufferRX[1], 0));
                bitWrite(PacketNumber, 1, bitRead(TwoWire_G_BufferRX[1], 1));
                if (PacketNumber == PacketCount) {
                  if (PacketNumber == 0) {
                    for (uint8_t X = 0; X < 2; X++) bitWrite(RTX_BufferRecieveTemp[0], X, bitRead(TwoWire_G_BufferRX[1], X + 2));
                    for (uint8_t X = 0; X < 4; X++) bitWrite(RTX_BufferRecieveTemp[1], X, bitRead(TwoWire_G_BufferRX[1], X + 4));
                  }
                  if (PacketNumber == 1) {
                    for (uint8_t X = 0; X < 4; X++) bitWrite(RTX_BufferRecieveTemp[1], X + 4, bitRead(TwoWire_G_BufferRX[1], X + 2));
                    for (uint8_t X = 0; X < 2; X++) bitWrite(RTX_BufferRecieveTemp[2], X, bitRead(TwoWire_G_BufferRX[1], X + 6));
                  }
                  if (PacketNumber == 2) {
                    for (uint8_t X = 0; X < 6; X++) bitWrite(RTX_BufferRecieveTemp[2], X + 2, bitRead(TwoWire_G_BufferRX[1], X + 2));
                    if (!RTX_BufferRecieveTemp[0]) {
                      Recieved = true;
                      Serial.print(F("READ: "));
                      Serial.print(RTX_BufferRecieveTemp[1]);
                      Serial.print(F(", "));
                      Serial.println(RTX_BufferRecieveTemp[2]);
                      Serial.println();
                      Serial.println(F("READ(0-255):"));
                    }
                    else {
                      Serial.println(F("ERROR: SYNCING"));
                      Serial.println();
                      Serial.println(F("READ(0-255):"));
                      while (Serial.available()) Serial.read();
                      return;
                    }
                  }
                  PacketCount++;
                }
                else {
                  Serial.println(F("ERROR: SYNCING"));
                  Serial.println();
                  Serial.println(F("READ(0-255):"));
                  while (Serial.available()) Serial.read();
                  return;
                }
              }
            }
          }
          if (!Recieved) {
            Serial.println(F("ERROR: TIMEOUT"));
            Serial.println();
            Serial.println(F("READ(0-255):"));
            while (Serial.available()) Serial.read();
            return;
          }
        }
      }
      while (Serial.available()) Serial.read();
    }
  }
  else if (WriteMode) {
    if (Serial.available()) {
      int SerialCheckTemp[2];
      SerialCheckTemp[0] = SerialParse();
      if (SerialCheckTemp[0] != -1) {
        if (bitRead(SerialCheckTemp[0], 15)) {
          bitWrite(SerialCheckTemp[0], 15, 0);
          if (SerialParse() == -1) {
            if (SerialCheckTemp[0] == 'C') {
              Serial.println(F("CANCEL"));
              Serial.println();
              Serial.println(F("SELECT ACTION(R,W,D)"));
              Command_Select = true;
              WriteMode = false;
            }
          }
        }
        else {
          SerialCheckTemp[1] = SerialParse();
          if (!bitRead(SerialCheckTemp[1], 15)) {
            if (SerialParse() == -1) {
              if (SerialCheckTemp[0] >= 0 and SerialCheckTemp[0] <= 255 and SerialCheckTemp[1] >= 0 and SerialCheckTemp[1] <= 255) {
                uint8_t RTX_BufferSendTemp[3] = {4, 1, 2};
                for (uint8_t X = 0; X < 4; X++) bitWrite(RTX_BufferSendTemp[0], X + 4, bitRead(SerialCheckTemp[0], X));
                for (uint8_t X = 0; X < 4; X++) bitWrite(RTX_BufferSendTemp[1], X + 2, bitRead(SerialCheckTemp[0], X + 4));
                for (uint8_t X = 0; X < 2; X++) bitWrite(RTX_BufferSendTemp[1], X + 6, bitRead(SerialCheckTemp[1], X));
                for (uint8_t X = 0; X < 6; X++) bitWrite(RTX_BufferSendTemp[2], X + 2, bitRead(SerialCheckTemp[1], X + 2));
                TwoWire_G_BufferTX[0] = SelectedIP;
                for (uint8_t X = 0; X < 3; X++) {
                  TwoWire_G_BufferTX[1] = RTX_BufferSendTemp[X];
                  if (!TwoWire_RTXMessage(1)) {
                    Serial.println(F("ERROR: CAN'T SEND"));
                    Serial.println();
                    Serial.println(F("WRITE(0-255,0-255):"));
                    while (Serial.available()) Serial.read();
                    return;
                  }
                }
                uint32_t RTX_TimeOutTimer = millis();
                uint8_t PacketCount = 0;
                uint8_t RTX_BufferRecieveTemp[3] = {0, 0, 0};
                bool Recieved = false;
                while ((millis() - RTX_TimeOutTimer) < 1000 and !Recieved) {
                  if (TwoWire_RTXMessage(0)) {
                    if (TwoWire_G_BufferRX[0] == SelectedIP) {
                      uint8_t PacketNumber = 0;
                      bitWrite(PacketNumber, 0, bitRead(TwoWire_G_BufferRX[1], 0));
                      bitWrite(PacketNumber, 1, bitRead(TwoWire_G_BufferRX[1], 1));
                      if (PacketNumber == PacketCount) {
                        if (PacketNumber == 0) {
                          for (uint8_t X = 0; X < 2; X++) bitWrite(RTX_BufferRecieveTemp[0], X, bitRead(TwoWire_G_BufferRX[1], X + 2));
                          for (uint8_t X = 0; X < 4; X++) bitWrite(RTX_BufferRecieveTemp[1], X, bitRead(TwoWire_G_BufferRX[1], X + 4));
                        }
                        if (PacketNumber == 1) {
                          for (uint8_t X = 0; X < 4; X++) bitWrite(RTX_BufferRecieveTemp[1], X + 4, bitRead(TwoWire_G_BufferRX[1], X + 2));
                          for (uint8_t X = 0; X < 2; X++) bitWrite(RTX_BufferRecieveTemp[2], X, bitRead(TwoWire_G_BufferRX[1], X + 6));
                        }
                        if (PacketNumber == 2) {
                          for (uint8_t X = 0; X < 6; X++) bitWrite(RTX_BufferRecieveTemp[2], X + 2, bitRead(TwoWire_G_BufferRX[1], X + 2));
                          if (RTX_BufferRecieveTemp[0]) {
                            Recieved = true;
                            Serial.print(F("WRITE: "));
                            Serial.print(RTX_BufferRecieveTemp[1]);
                            Serial.print(F(", "));
                            Serial.println(RTX_BufferRecieveTemp[2]);
                            Serial.println();
                            Serial.println(F("WRITE(0-255,0-255):"));
                          }
                          else {
                            Serial.println(F("ERROR: SYNCING"));
                            Serial.println();
                            Serial.println(F("WRITE(0-255,0-255):"));
                            while (Serial.available()) Serial.read();
                            return;
                          }
                        }
                        PacketCount++;
                      }
                      else {
                        Serial.println(F("ERROR: SYNCING"));
                        Serial.println();
                        Serial.println(F("WRITE(0-255,0-255):"));
                        while (Serial.available()) Serial.read();
                        return;
                      }
                    }
                  }
                }
                if (!Recieved) {
                  Serial.println(F("ERROR: TIMEOUT"));
                  Serial.println();
                  Serial.println(F("WRITE(0-255,0-255):"));
                  while (Serial.available()) Serial.read();
                  return;
                }
              }
            }
          }
        }
      }
      while (Serial.available()) Serial.read();
    }
  }
  else if (DumpMode) {
    Serial.print(F("DATA DUMP"));
    if(ExportBinary)  Serial.print(F("(BIN)"));
    Serial.println(F(":"));
    TwoWire_G_BufferTX[0] = SelectedIP;
    TwoWire_G_BufferTX[1] = 0;
    if (!TwoWire_RTXMessage(1)) {
      Serial.println(F("ERROR: CAN'T SEND"));
      Serial.println();
      Serial.println(F("SELECT ACTION(R,W,D)"));
      Command_Select = true;
      DumpMode = false;
      while (Serial.available()) Serial.read();
      return;
    }
    Serial.println(F("DOWNLOADING, PLEASE WAIT:"));
    Serial.print(F("0%"));
    for (uint8_t X = 0; X < 92; X++) Serial.print(F(" "));
    Serial.println(F("100%"));
    bool DataDumpDone = false;
    uint8_t RegisterCounter = 0;
    uint8_t TempRegHold[256];
    uint8_t SerialLoadCounter = 0;
    for (uint16_t X = 0; X < 256; X++) TempRegHold[X] = 0;
    while (!DataDumpDone) {
      uint8_t RTX_BufferSendTemp[2] = {0, 1};
      for (uint8_t X = 0; X < 4; X++) bitWrite(RTX_BufferSendTemp[0], X + 4, bitRead(RegisterCounter, X));
      for (uint8_t X = 0; X < 4; X++) bitWrite(RTX_BufferSendTemp[1], X + 2, bitRead(RegisterCounter, X + 4));
      TwoWire_G_BufferTX[0] = SelectedIP;
      for (uint8_t X = 0; X < 2; X++) {
        TwoWire_G_BufferTX[1] = RTX_BufferSendTemp[X];
        if (!TwoWire_RTXMessage(1)) {
          Serial.println();
          Serial.println(F("ERROR: CAN'T SEND"));
          Serial.println();
          Serial.println(F("SELECT ACTION(R,W,D)"));
          Command_Select = true;
          DumpMode = false;
          while (Serial.available()) Serial.read();
          return;
        }
      }
      uint32_t RTX_TimeOutTimer = millis();
      uint8_t PacketCount = 0;
      uint8_t RTX_BufferRecieveTemp[3] = {0, 0, 0};
      bool Recieved = false;
      while ((millis() - RTX_TimeOutTimer) < 1000 and !Recieved) {
        if (Serial.available()) {
          int SerialCheckTemp = SerialParse();
          if (SerialParse() == -1) {
            if (bitRead(SerialCheckTemp, 15)) {
              bitWrite(SerialCheckTemp, 15, 0);
              if (SerialCheckTemp == 'C') {
                Serial.println();
                Serial.println(F("CANCELLED"));
                Serial.println();
                Serial.println(F("SELECT ACTION(R,W,D)"));
                Command_Select = true;
                DumpMode = false;
                return;
              }
            }
          }
          while (Serial.available()) Serial.read();
        }
        if (TwoWire_RTXMessage(0)) {
          if (TwoWire_G_BufferRX[0] == SelectedIP) {
            uint8_t PacketNumber = 0;
            bitWrite(PacketNumber, 0, bitRead(TwoWire_G_BufferRX[1], 0));
            bitWrite(PacketNumber, 1, bitRead(TwoWire_G_BufferRX[1], 1));
            if (PacketNumber == PacketCount) {
              if (PacketNumber == 0) {
                for (uint8_t X = 0; X < 2; X++) bitWrite(RTX_BufferRecieveTemp[0], X, bitRead(TwoWire_G_BufferRX[1], X + 2));
                for (uint8_t X = 0; X < 4; X++) bitWrite(RTX_BufferRecieveTemp[1], X, bitRead(TwoWire_G_BufferRX[1], X + 4));
              }
              if (PacketNumber == 1) {
                for (uint8_t X = 0; X < 4; X++) bitWrite(RTX_BufferRecieveTemp[1], X + 4, bitRead(TwoWire_G_BufferRX[1], X + 2));
                for (uint8_t X = 0; X < 2; X++) bitWrite(RTX_BufferRecieveTemp[2], X, bitRead(TwoWire_G_BufferRX[1], X + 6));
              }
              if (PacketNumber == 2) {
                for (uint8_t X = 0; X < 6; X++) bitWrite(RTX_BufferRecieveTemp[2], X + 2, bitRead(TwoWire_G_BufferRX[1], X + 2));
                if (!RTX_BufferRecieveTemp[0]) {
                  if (RTX_BufferRecieveTemp[1] == RegisterCounter) {
                    TempRegHold[RegisterCounter] = RTX_BufferRecieveTemp[2];
                    Recieved = true;
                    if (RegisterCounter == 255) DataDumpDone = true;
                    else RegisterCounter++;
                    while (SerialLoadCounter < map(RegisterCounter, 0, 255, 0, 98)) {
                      Serial.print(F("-"));
                      SerialLoadCounter++;
                    }
                  }
                }
                else {
                  Serial.println();
                  Serial.println(F("ERROR: SYNCING"));
                  Serial.println();
                  Serial.println(F("SELECT ACTION(R,W,D)"));
                  Command_Select = true;
                  DumpMode = false;
                  while (Serial.available()) Serial.read();
                  return;
                }
              }
              PacketCount++;
            }
            else {
              Serial.println();
              Serial.println(F("ERROR: SYNCING"));
              Serial.println();
              Serial.println(F("SELECT ACTION(R,W,D)"));
              Command_Select = true;
              DumpMode = false;
              while (Serial.available()) Serial.read();
              return;
            }
          }
        }
      }
      if (!Recieved) {
        Serial.println();
        Serial.println(F("ERROR: TIMEOUT"));
        Serial.println();
        Serial.println(F("SELECT ACTION(R,W,D)"));
        Command_Select = true;
        DumpMode = false;
        while (Serial.available()) Serial.read();
        return;
      }
    }
    while (SerialLoadCounter < 98) {
      Serial.print(F("-"));
      SerialLoadCounter++;
    }
    Serial.println();
    uint16_t X = 0;
    while (X < 256) {
      Serial.print(X);
      Serial.print("-");
      Serial.print(X + 7);
      Serial.print(":");
      Serial.print("\t");
      if (X < 104) Serial.print("\t");
      for (uint8_t Y = 0; Y < 8; Y++) {
        if (ExportBinary) {
          for (int8_t Z = 7; Z > -1; Z--) {
            Serial.print(bitRead(TempRegHold[X], Z));
          }
          Serial.print(F("("));
          Serial.print(TempRegHold[X]);
          Serial.print(F(")"));
        }
        else Serial.print(TempRegHold[X]);
        Serial.print("\t");
        X++;
      }
      Serial.println();
    }
    DumpMode = false;
    Command_Select = true;
    Serial.println();
    Serial.println(F("SELECT ACTION(R,W,D)"));
  }
}

int16_t SerialParse() {
  delay(5);
  if (Serial.available() > 1) {
    int Result = 0;
    int TempSerialHold = Serial.read();
    if (TempSerialHold >= 48 and TempSerialHold <= 57) {
      int TempCharBuffer[4];
      uint8_t CharCount = 0;
      while (Serial.available() and TempSerialHold >= 48 and TempSerialHold <= 57) {
        if (CharCount < 4) {
          TempCharBuffer[CharCount] = TempSerialHold - 48;
          CharCount++;
        }
        TempSerialHold = Serial.read();
      }
      uint8_t TempCount = 0;
      while (CharCount > 0) {
        switch (TempCount) {
          case 0:
            Result = TempCharBuffer[CharCount - 1];
            break;
          case 1:
            Result += TempCharBuffer[CharCount - 1] * 10;
            break;
          case 2:
            Result += TempCharBuffer[CharCount - 1] * 100;
            break;
          case 3:
            Result += TempCharBuffer[CharCount - 1] * 1000;
            break;
          case 4:
            Result += TempCharBuffer[CharCount - 1] * 10000;
            break;
        }
        CharCount--;
        TempCount++;
      }
      if (Serial.available() == 1) Serial.read();
      return Result;
    }
    else {
      Result = TempSerialHold;
      bitWrite(Result, 15, 1);
      if (Serial.available() == 1) Serial.read();
      return Result;
    }
  }
  else {
    if (Serial.available()) Serial.read();
    return -1;
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
