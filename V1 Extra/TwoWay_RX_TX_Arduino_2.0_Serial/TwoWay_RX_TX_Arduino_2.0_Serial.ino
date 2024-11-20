int const BufferSize = 4;
int const MasterIP = 1;
int const SlaveIP[] = {2, 3, 4};
int const SlaveIP_Count = (sizeof(SlaveIP) / 2);
int Buffer[SlaveIP_Count][(BufferSize + 1)][2];
int BufferSend[2];
int Recieved[5];

void setup() {
  Serial.begin(9600);
}

void loop() {
  Run();
}

void Run() {
  for (int I = 0; I < SlaveIP_Count; I++) {
    if (Safe()) {
      Send(SlaveIP[I], MasterIP, Command(1, CheckBuffer(I), 0), 0, 0, 0);
      if (Read()) {
        switch (CheckMessage(I)) {
          ////////
          case 1:
            //RX
            if (Safe()) {
              boolean CheckBuffer = BufferRX(I);
              Send(SlaveIP[I], MasterIP, Command(CheckBuffer, 0, 0), BufferSend[0] , BufferSend[1] , CheckBuffer);
            }
            break;
          ////////
          case 2:
            //TX
            if (Safe()) {
              Send(SlaveIP[I], MasterIP, Command(0, BufferStore(I, Recieved[3], Recieved[4]), 0), 0, 0, 0);
            }
            break;
          ////////
          case 3:
            //RTX
            if (Safe()) {
              boolean CheckBuffer = BufferRX(I);
              Send(SlaveIP[I], MasterIP, Command(CheckBuffer, BufferStore(I, Recieved[3], Recieved[4]), 0), BufferSend[0] , BufferSend[1] , CheckBuffer);
            }
            break;
          ////////
          default:
            //ERROR
            break;
        }
      }
    }
  }
}

boolean Safe() {
  return true;
}
boolean Read() {
  boolean DoneRead = false;
  while (!DoneRead) {
    delay(20);
    while (Serial.available()) {
      Serial.read();
    }
    for (int I = 0; I < 5; I++) {
      Recieved[I] = 0;
    }

    Serial.print("EnterMessage? Y or N:");
    while (!Serial.available()) {

    }
    if (Serial.read() == 89) {
      Serial.println(" YES");
    }
    else {
      Serial.println(" NO");
      return false;
    }
    delay(20);
    while (Serial.available()) {
      Serial.read();
    }
    Serial.println("EnterMessage:");
    while (!Serial.available()) {

    }
    delay(30);
    int RecievedCount = 0;
    boolean Done = false;
    int SerialR = 0;
    int SerialRead[3];
    int SerialCount = 0;
    int SerialResult = 0;
    int Multiply[] = {100, 10, 1};
    while (Serial.available() or !Done) {
      SerialR = (Serial.read() - 48);
      if (SerialCount < 3 and SerialR >= 0 and SerialR <= 9) {
        Done = false;
        SerialRead[SerialCount] = SerialR;
        SerialCount++;
      }
      else {
        Done = true;

        for (int I = 0; I < SerialCount; I++) {
          SerialResult += SerialRead[I] * Multiply[(3 - SerialCount) + I];
        }
        Recieved[RecievedCount] = SerialResult;
        if (RecievedCount < 4) {
          RecievedCount++;
        }

        SerialR = 0;
        SerialRead[0] = 0;
        SerialRead[1] = 0;
        SerialRead[2] = 0;
        SerialCount = 0;
        SerialResult = 0;
      }
    }
    for (int I = 0; I < 5; I++) {
      Serial.print(Recieved[I]);
      Serial.print(",");
    }
    Serial.println();
    Serial.print("Y or N: ");
    while (!Serial.available()) {

    }
    if (Serial.read() == 89) {
      Serial.println("YES");
      delay(20);
      while (Serial.available()) {
        Serial.read();
      }
      DoneRead = true;
    }
    else {
      Serial.println("NO");
      delay(20);
      while (Serial.available()) {
        Serial.read();
      }
      DoneRead = false;
    }
  }
  return true;
}

void Send(int A, int B, int C, int D, int E, boolean F) {
  Serial.println("Recieved Message:");
  Serial.print(A);
  Serial.print(",");
  Serial.print(B);
  Serial.print(",");
  Serial.print(C);
  Serial.print(",");
  Serial.print(D);
  Serial.print(",");
  Serial.print(E);
  Serial.print(",");
  Serial.print(F);
  Serial.println();
}

boolean CheckBuffer(int SlaveIP) {
  if (Buffer[SlaveIP][BufferSize][0] == Buffer[SlaveIP][BufferSize][1]) {
    return false;
  }
  else {
    return true;
  }
}

int CheckMessage(int I) {
  if (Recieved[0] == MasterIP and Recieved[1] == SlaveIP[I]) {
    switch (Recieved[2]) {
      case 1:
        return Recieved[2];
        break;

      case 2:
        return Recieved[2];
        break;

      case 3:
        return Recieved[2];
        break;

      default:
        return -1;
    }
  }
  return -1;
}



int Command (boolean X, boolean Y, boolean Z) {
  int Final = X;
  Final += Y << 1;
  Final += Z << 2;
  return Final;
}


boolean BufferRX(int SlaveIP) {
  if (CheckBuffer(SlaveIP)) {
    if (Buffer[SlaveIP][BufferSize][0] < (BufferSize - 1)) {
      Buffer[SlaveIP][BufferSize][0]++;
    }
    else {
      Buffer[SlaveIP][BufferSize][0] = 0;
    }
    BufferSend[0] = Buffer[SlaveIP][Buffer[SlaveIP][BufferSize][0]][0];
    BufferSend[1] = Buffer[SlaveIP][Buffer[SlaveIP][BufferSize][0]][1];
    return true;
  }
  else {
    BufferSend[0] = 0;
    BufferSend[1] = 0;
    return false;
  }
}

boolean BufferStore(int SenderIP, int RecieverIP, int Data) {
  boolean FoundIP = false;
  int SlaveIP_Result = 0;
  for ( int I = 0; I < SlaveIP_Count and FoundIP == false; I++) {
    if (RecieverIP == SlaveIP[I]) {
      FoundIP = true;
      SlaveIP_Result = I;
    }
  }
  if (FoundIP == true) {
    int BufferTest = Buffer[SlaveIP_Result][BufferSize][1];
    if (Buffer[SlaveIP_Result][BufferSize][1] == (BufferSize - 1)) {
      BufferTest = 0;
    }
    else {
      BufferTest++;
    }
    if (BufferTest == Buffer[SlaveIP_Result][BufferSize][0]) {
      return false;
    }
    else {
      Buffer[SlaveIP_Result][BufferSize][1] = BufferTest;
      Buffer[SlaveIP_Result][Buffer[SlaveIP_Result][BufferSize][1]][0] = SlaveIP[SenderIP];
      Buffer[SlaveIP_Result][Buffer[SlaveIP_Result][BufferSize][1]][1] = Data;
      return true;
    }
  }
  else {
    return false;
  }
}
