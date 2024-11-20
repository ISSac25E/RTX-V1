//PORTB = PORTB | B00000100;        10/SDAT HIGH
//PORTB = PORTB & B11111011;        10/SDAT LOW
//
//PORTB = PORTB | B00001000;        11/SCLT HIGH
//PORTB = PORTB & B11110111;        11/SCLT LOW
//
//
//
//
//PORTD = PORTD | B00001000;        3/SDAT HIGH
//PORTD = PORTD & B11110111;        3/SDAT LOW
//
//PORTD = PORTD | B00100000;        5/SCLT HIGH
//PORTD = PORTD & B11011111;        5/SCLT LOW
//
//
//
//
//
//PORTB = PORTB | B00100000;
//PORTB = PORTB & B11011111;








////TX 6.0 LOW
//#define NOP __asm__ __volatile__ ("nop\n\t")
//const int SDAT = 3;
//const int SCLT = 5;
//long int X = 0;
//void setup() {
//  pinMode(SDAT, OUTPUT);
//  pinMode(SCLT, OUTPUT);
////  pinMode(12, INPUT_PULLUP);
////  pinMode(SDAT, INPUT_PULLUP);
////  pinMode(SCLT, INPUT_PULLUP);
//  pinMode(13,OUTPUT);
//}
//
//void loop() {
//delayMicroseconds(200);
//   Send(X, 8, 6);
//   X++;
//}
//
//void Send(const int Message, const int PacketLength, const int Interval){
//  PORTB = PORTB | B00100000;
//  int PacketCount = 0;
//  unsigned long int MicrosCount = 0;
//
//  boolean Packet[PacketLength];
//  boolean Done = false;
//  boolean PulseState = HIGH;
//  boolean PulseEnable = false;
//      for(int I = 0; I < PacketLength; I++){
// Packet[I] = bitRead(Message, I);
//}
////pinMode(SDAT, OUTPUT);
////  pinMode(SCLT, OUTPUT);
//  while(!Done){
//    //if((micros() - MicrosCount) >= Interval){
//      //MicrosCount = micros();
//      if(PulseEnable){
//        if(PulseState){
//          PORTD = PORTD & B11011111;
//          PulseState = LOW;
//        }
//        else{
//          PORTD = PORTD | B00100000;
//          PulseState = HIGH;
//        }
//      }
//      else{
//        NOP;
//        NOP;
//        NOP;
//        NOP;
//        NOP;
//      }
//      if(PulseState){
//        if(PacketCount < PacketLength){
//        if(Packet[PacketCount]){
//          PORTD = PORTD | B00001000;
//        }
//        else{
//          PORTD = PORTD & B11110111;
//        }
//          PacketCount++;
//        }
//        else{
//          Done = true;
//        }
//        if(!PulseEnable){
//          PulseEnable = true;
//        }
//
//
////      }
//    }
//    else{
//        NOP;
//        NOP;
//        NOP;
//        NOP;
//        NOP;
//        NOP;
//        NOP;
//        NOP;
//        NOP;
//        NOP;
//    }
//    for(int X = 0; X <= 20; X++){
//        NOP;
//        }
//  }
//  PORTD = PORTD | B00001000;
//  PORTD = PORTD | B00100000;
////  pinMode(SDAT, INPUT_PULLUP);
////  pinMode(SCLT, INPUT_PULLUP);
//  PORTB = PORTB & B11011111;
//}




//bitRead(PINB,2);   10
//bitRead(PINB,3);   11


//bitRead(PIND,3);   3   SDA
//bitRead(PIND,5);   5   SCL




//RX 6.0 LOW
#define NOP __asm__ __volatile__ ("nop\n\t")
int X = 0;
int Y = 0;

unsigned long int Z = 0;
unsigned long int Count = 0;
unsigned long int TempCount = 0;
unsigned long int Temp = 0;


unsigned long int Seconds = 0;
unsigned long int Minutes = 0;
unsigned long int Hours = 0;
unsigned long int Counter = 0;
void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  Serial.begin(2000000);
  delay(500);
  Count = millis();
}

void loop() {
  if (!bitRead(PIND, 2)) {
    Serial.print("Total: ");
    Serial.println(Z);
    Seconds = 0;
    Minutes = 0;
    Hours = 0;
    Counter = (millis() - Count);

    while (Counter >= 1000) {
      Seconds++;
      Counter -= 1000;
    }
    while (Seconds >= 60) {
      Minutes++;
      Seconds -= 60;
    }
    while (Minutes >= 60) {
      Hours++;
      Minutes -= 60;
    }
    Serial.print(Hours);
    Serial.print(":");
    Serial.print(Minutes);
    Serial.print(":");
    Serial.print(Seconds);
    Serial.print(".");
    Serial.println(Counter);
    while (!bitRead(PIND, 2));
  }
  if (!bitRead(PIND, 5)) {
    X = CheckData(8);
    if (X == -1) {
      Serial.println("ERROR");
    }
    else {
      Serial.println(X);
      if (X != Y) {
        while (Serial.available()) {
          Serial.read();
        }
        Serial.print("ERROR: ");
        Serial.print(Y);
        Serial.println(" :<<<<<<");
        Y = X;
        Serial.print("Total: ");
    Serial.println(Z);
    Seconds = 0;
    Minutes = 0;
    Hours = 0;
    Counter = (millis() - Count);

    while (Counter >= 1000) {
      Seconds++;
      Counter -= 1000;
    }
    while (Seconds >= 60) {
      Minutes++;
      Seconds -= 60;
    }
    while (Minutes >= 60) {
      Hours++;
      Minutes -= 60;
    }
    Serial.print(Hours);
    Serial.print(":");
    Serial.print(Minutes);
    Serial.print(":");
    Serial.print(Seconds);
    Serial.print(".");
    Serial.println(Counter);
        while (Serial.available() == 0) {

        }
        while (Serial.available()) {
          Serial.read();
        }
        while (!bitRead(PIND, 5)) {
        }
        X = CheckData(8);
        Y = X;
        Y++;
      }
      else {

        Z++;
        if (Y < 255) {
          Y++;
        }
        else {
          Y = 0;
        }
      }
    }

  }
}
int CheckData(int CheckData_Y) {
  byte CheckData_Result = B0;
  boolean CheckData_Done = false;
  unsigned long int CheckData_M1 = 0;
  int CheckData_Z = 0;
  long unsigned NOPCOUNT = 0;
  CheckData_Done = false;
  //  CheckData_M1 = micros();
  //    CheckData_M1 += 150;
  for (int CheckData_X = 0; CheckData_X < CheckData_Y and !CheckData_Done; CheckData_X++) {
    //    if(!bitRead(PINB,3)){
    CheckData_Result += bitRead(PIND, 3) << CheckData_X;
    //    CheckData_M1 = micros();
    //    CheckData_M1 += 150;
    NOPCOUNT = 0;
    while (!bitRead(PIND, 5) and !CheckData_Done) {
      //      NOP;
      NOPCOUNT++;
      if (NOPCOUNT > 30) {
        CheckData_Done = true;
      }
    }
    NOPCOUNT = 0;
    while (bitRead(PIND, 5) and !CheckData_Done) {
      //      NOP;
      NOPCOUNT++;
      if (NOPCOUNT > 30) {
        CheckData_Done = true;
      }
    }

    CheckData_Z++;

  }
  if (CheckData_Z == CheckData_Y) {
    return CheckData_Result;
  }
  else {
    //    Serial.print("ERROR ");
    //    Serial.print(CheckData_Z);
    //    Serial.print(" - ");
    //    return CheckData_Result;
    return -1;
  }
}
