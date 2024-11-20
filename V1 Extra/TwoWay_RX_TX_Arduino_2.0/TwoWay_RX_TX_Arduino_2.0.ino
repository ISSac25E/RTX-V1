//Notes ▼▼▼
Master{
  boolean Safe(int Delay){
   //Checks if CLK and DAT lines are pulled high for at least a set amount of time
   //Optional: Delay declares how long it will keep trying to check CLK and DAT lines for safe transmission
   //Returns HIGH if CLK and DAT line pulled low for longer than set amount of time.
   //Return LOW otherwise
  }
  boolean Read(int Size, int Delay){
    //Read incoming messages through CLK and DAT lines
    //Size declares how many bits(clock pulses) to expect comming in
    //Delay declares how long function will keep checking for incoming messages before terminating operation
    //Stores any succesfully read messages into global array eg. Recieved[]
    //Returns HIGH if message succesfully read and stored in global array.
    //Returns LOW otherwise
  }
  void Write(int IP, int COM, int IP, int DATA, boolean Size){
    //Transmits message through CLK and DAT lines
    //IP declares which target device to send message to
    //COM is command packet in the message
    //IP and DATA is optional when reading from buffer to send(depending on SIZE)
    //Size declares how many bits(clock pulses) are to be sent out
  }
}


Slave{
  
}





//Master ▼▼▼
/*

 
int const BufferSize = 10;
int const MasterIP = 1;
int const SlaveIP[] = {2, 3, 4};
int const SlaveIP_Count = (sizeof(SlaveIP) / 2);
int Buffer[SlaveIP_Count][(BufferSize + 1)][2];
int BufferSend[2];
int Recieved[5];

void setup() {

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
  return false;
}
void Send(int A, int B, int C, int D, int E, int F) {
  
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
     if(RecieverIP == SlaveIP[I]){
      FoundIP = true;
      SlaveIP_Result = I;
     }
  }
  if(FoundIP == true){
  int BufferTest = Buffer[SlaveIP_Result][BufferSize][1];
  if(Buffer[SlaveIP_Result][BufferSize][1] == (BufferSize - 1)){
    BufferTest = 0;
  }
  else{
    BufferTest++;
  }
    if(BufferTest == Buffer[SlaveIP_Result][BufferSize][0]){
    return false;
  }
  else{
    Buffer[SlaveIP_Result][BufferSize][1] = BufferTest;
    Buffer[SlaveIP_Result][Buffer[SlaveIP_Result][BufferSize][1]][0] = SlaveIP[SenderIP];
    Buffer[SlaveIP_Result][Buffer[SlaveIP_Result][BufferSize][1]][1] = Data;
    return true;
  }
  }
  else{
    return false;
  }
}

*///Master ▲▲▲






//Slave ▼▼▼
const int SlaveIP = 2;
const int MasterIP = 1;
int Recieved[5];
boolean Status[3];
boolean 
void setup() {
}

void loop() {
}


boolean TwoWire(int Command, int RecieverIP, int Data){
  switch (Command){
    case 1:
    //RX
    if(Insert()){
      if(Allowed()){
        if(Safe()){
          
        }
      }
    }
    break;

    case 1:
    //TX
    break;

    default:
    return false;
    break;
  }
}


boolean Safe(){
  
}

boolean Insert(){
  //e.g. CLK and DAT line high for at least 100us
}



boolean Allowed(){

}

void Send(int A, int B, int C, int D, int E, int F) {
  
}

boolean Read() {
  return false;
}


int Command (boolean X, boolean Y, boolean Z) {
  int Final = X;
  Final += Y << 1;
  Final += Z << 2;
  return Final;
}
//Slave ▲▲▲
