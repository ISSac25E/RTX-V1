
int const MasterIP = 1;
int const SlaveIP[] = {2, 3, 4};
int const SlaveIP_Count = (sizeof(SlaveIP) / 2);
int Buffer[SlaveIP_Count][5][2];
int Recieved[5];

void setup() {
  pinMode(pin, INPUT_PULLUP);
  pinMode(pin, INPUT_PULLUP);
}

void loop() {
  TwoWire_Run();
}
void TwoWire_Run() {
  if(TwoWire_Safe()){
    //TwoWire_Send(SlaveIP[1], MasterIP,'COM1,COM2,COM3',IP,DATA,STATE)
    //or
    //TwoWire_Send(0)
    if(TwoWire_Read()){
      TwoWire_Send(1);
    }
  }
}
boolean TwoWire_Safe() {
  
}
boolean TwoWire_Read() {
  
}
TwoWire_Send() {
  
}
