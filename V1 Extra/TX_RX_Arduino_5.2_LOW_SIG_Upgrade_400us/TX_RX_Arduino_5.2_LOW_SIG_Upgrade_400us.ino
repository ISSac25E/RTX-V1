//TX 5.0 LOW
const int SDAT = 10;
const int SCLT = 11;
long int X = 0;
void setup() {
  pinMode(SDAT, INPUT_PULLUP);
  pinMode(SCLT, INPUT_PULLUP);

}

void loop() {
  delayMicroseconds(200);
Send(X, 8);
X++;
}

void Send(const int Message, const int PacketLength){
  int PacketCount = 0;
  unsigned long int MicrosCount = 0;
  const int Interval = 20;
  boolean Packet[PacketLength];
  boolean Done = false;
  boolean PulseState = HIGH;
  boolean PulseEnable = false;
      for(int I = 0; I < PacketLength; I++){
 Packet[I] = bitRead(Message, I);
}
pinMode(SDAT, OUTPUT);
  pinMode(SCLT, OUTPUT);
  while(!Done){
    if((micros() - MicrosCount) >= Interval){
      if(PulseEnable){
        if(PulseState){
          TwoWire_PinWrite(1, LOW);
          PulseState = LOW;
        }
        else{
         TwoWire_PinWrite(1, HIGH);
          PulseState = HIGH;
        }
      }
      if(PulseState){
        if(PacketCount < PacketLength){
          TwoWire_PinWrite(0, Packet[PacketCount]);
          PacketCount++;
        }
        else{
          Done = true;
        }
        if(!PulseEnable){
          PulseEnable = true;
        }
      }
      MicrosCount = micros();
    }
  }
  pinMode(SDAT, INPUT_PULLUP);
  pinMode(SCLT, INPUT_PULLUP);
}
void TwoWire_PinWrite(bool PIN, bool DIR) {
  if (PIN) {
    if (DIR) {
      PORTB = PORTB | B00001000;
    }
    else {
      PORTB = PORTB & B11110111;
    }
  }
  else {
    if (DIR) {
      PORTB = PORTB | B00000100;
    }
    else {
      PORTB = PORTB & B11111011;
    }
  }
}









//RX 5.0 LOW
int X = 0;
void setup() {
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  Serial.begin(2000000);
  delay(500);
}

void loop() {
  if (!bitRead(PINB,3)) {
    X = CheckData(8);
    if(X == -1){
    Serial.println("ERROR");
    }
    else{
      Serial.println(X);
    }
  }
}
int CheckData(int CheckData_Y) {
  byte CheckData_Result = B00000000;
  boolean CheckData_Done = false;
  unsigned long int CheckData_M1 = 0;
  int CheckData_Z = 0;
  CheckData_Done = false;
  for (int CheckData_X = 0; CheckData_X < (CheckData_Y+1) and CheckData_Done == false; CheckData_X++) {
    if (bitRead(PINB,2) == HIGH) {
      CheckData_Result += true << CheckData_X;
    }
    else {
      CheckData_Result += false << CheckData_X;
    }
    CheckData_M1 = micros();
    CheckData_M1 += 60;
    while (!bitRead(PINB,3) and CheckData_Done == false) {
      if (micros() > CheckData_M1) {
        CheckData_Done = true;
      }
    }
    while (bitRead(PINB,3) and CheckData_Done == false) {
      if (micros() > CheckData_M1) {
        CheckData_Done = true;
      }
    }
    CheckData_Z++;
  }
  if(CheckData_Z < CheckData_Y or CheckData_Z > CheckData_Y){
    return -1;

  }
  else{
  return CheckData_Result;
  }
}
