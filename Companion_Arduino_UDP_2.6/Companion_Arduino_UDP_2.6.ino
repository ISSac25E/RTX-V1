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
unsigned long int BUTTON4_COUNT = 0;
unsigned long int BUTTON4_COUNTER = 0;

unsigned long int BUTTON3_COUNTER = 0;
unsigned long int BUTTON5_COUNTER = 0;

unsigned long int ConnectionTimer = 0;

int PACKET_COUNT = 0;

boolean CONNECTED_STAGE1 = true;
boolean CONNECTED = true;
boolean BUTTON3 = false;
boolean BUTTON4 = false;
boolean BUTTON5 = false;

boolean ButtonState = false;
boolean ConnectionSleep = false;

EthernetUDP UDP;
UIPClient UIP;

void setup() {
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  Serial.begin(2000000);
  Ethernet.begin(ARD_MAC, ARD_IP);
  UDP.begin(ARD_LCP);
  Serial.println("Connecting");
  while (!UIP.connect(COMP_IP, TCP_LCP)) {
    delay(100);
  }
  Serial.println("Connected");
  TCP_READ_COUNT = millis();
  TCP_READ_COUNT += 600;
}

void loop() {
  CheckButton();
  CheckInterval();
  if(ConnectionSleep){
    UIPSleep();
  }
  if (!CONNECTED) {
    Reconnect();
  }
}

void CheckButton() {
  if (!digitalRead(3) and !BUTTON3) {
    delay(10);
    if (!digitalRead(3)) {
      ButtonState = true;
      BUTTON3 = true;
      while(!digitalRead(3) and BUTTON3_COUNTER < 200){
        BUTTON3_COUNTER += 10;
        delay(10);
      }
      if(BUTTON3_COUNTER >= 200){
        for (int I = 0; I < 2; I++) {
        UDP.beginPacket(COMP_IP, UDP_LCP);
        UDP.write("bank-down 5 17");
        UDP.endPacket();
      }
      for (int I = 0; I < 1; I++) {
        UDP.beginPacket(COMP_IP, UDP_LCP);
        UDP.write("bank-up 5 17");
        UDP.endPacket();
      }
      }
      else{
      for (int I = 0; I < 2; I++) {
        UDP.beginPacket(COMP_IP, UDP_LCP);
        UDP.write("bank-down 5 25");
        UDP.endPacket();
      }
      for (int I = 0; I < 1; I++) {
        UDP.beginPacket(COMP_IP, UDP_LCP);
        UDP.write("bank-up 5 25");
        UDP.endPacket();
      }
      }
      BUTTON3_COUNTER = 0;
      Serial.println("5 25");
    }
  }
  if (!digitalRead(5) and !BUTTON5) {
    delay(10);
    if (!digitalRead(5)) {
      ButtonState = true;
      BUTTON5 = true;
      while(!digitalRead(5) and BUTTON5_COUNTER < 200){
        BUTTON5_COUNTER += 10;
        delay(10);
      }
      if(BUTTON5_COUNTER >= 200){
        for (int I = 0; I < 2; I++) {
        UDP.beginPacket(COMP_IP, UDP_LCP);
        UDP.write("bank-down 5 18");
        UDP.endPacket();
      }
      for (int I = 0; I < 1; I++) {
        UDP.beginPacket(COMP_IP, UDP_LCP);
        UDP.write("bank-up 5 18");
        UDP.endPacket();
      }
      }
      else{
      for (int I = 0; I < 2; I++) {
        UDP.beginPacket(COMP_IP, UDP_LCP);
        UDP.write("bank-down 5 26");
        UDP.endPacket();
      }
      for (int I = 0; I < 1; I++) {
        UDP.beginPacket(COMP_IP, UDP_LCP);
        UDP.write("bank-up 5 26");
        UDP.endPacket();
      }
      }
      BUTTON5_COUNTER = 0;
      Serial.println("5 26");
//      for (int I = 0; I < 2; I++) {
//        UDP.beginPacket(COMP_IP, UDP_LCP);
//        UDP.write("bank-down 5 26");
//        UDP.endPacket();
//      }
//      for (int I = 0; I < 1; I++) {
//        UDP.beginPacket(COMP_IP, UDP_LCP);
//        UDP.write("bank-up 5 26");
//        UDP.endPacket();
//      }
//      Serial.println("5 26");
    }
  }
  if (!digitalRead(4) and !BUTTON4) {
    delay(10);
    if (digitalRead(4) == LOW) {
      ButtonState = true;
      BUTTON4 = true;
      while (digitalRead(4) == LOW) {
        BUTTON4_COUNTER += 10;
        delay(10);
      }
      if (BUTTON4_COUNTER <= 300) {
        for (int I = 0; I < 2; I++) {
          UDP.beginPacket(COMP_IP, UDP_LCP);
          UDP.write("bank-down 5 27");
          UDP.endPacket();
        }
        for (int I = 0; I < 1; I++) {
          UDP.beginPacket(COMP_IP, UDP_LCP);
          UDP.write("bank-up 5 27");
          UDP.endPacket();
        }
        Serial.println("5 27");
      }
      else {
        for (int I = 0; I < 2; I++) {
          UDP.beginPacket(COMP_IP, UDP_LCP);
          UDP.write("bank-down 5 28");
          UDP.endPacket();
        }
        for (int I = 0; I < 1; I++) {
          UDP.beginPacket(COMP_IP, UDP_LCP);
          UDP.write("bank-up 5 28");
          UDP.endPacket();
        }
        Serial.println("5 28");
      }
      BUTTON4_COUNTER = 0;
      BUTTON4_COUNT = millis();
      BUTTON4_COUNT += 300;
    }
  }
  if (digitalRead(3) and BUTTON3) {
    BUTTON3 = false;
  }
  if (digitalRead(5) and BUTTON5) {
    BUTTON5 = false;
  }
  if (millis() >= BUTTON4_COUNT and BUTTON4) {
    BUTTON4 = false;
  }
}

void CheckInterval() {
  if(ButtonState){
    ButtonState = false;
    ConnectionTimer = millis();
  }
  if((millis() - ConnectionTimer) > 3600000){
    ConnectionSleep = true;
  }
  while (UIP.available() and !ConnectionSleep) {
    UIP.read();
    PACKET_COUNT++;
  }
  if (millis() >= TCP_WRITE_COUNT and !ConnectionSleep) {
    UIP.println("bank-up 5 8");
    TCP_WRITE_COUNT = millis();
    TCP_WRITE_COUNT += 300;
  }
  if (millis() >= TCP_READ_COUNT and !ConnectionSleep) {
    if (PACKET_COUNT == 0 and CONNECTED_STAGE1 == false) {
      CONNECTED = false;
      Serial.println("Disconnected");
    }
    else {
      CONNECTED_STAGE1 = true;
    }
    if (PACKET_COUNT == 0 and CONNECTED_STAGE1 == true) {
      CONNECTED_STAGE1 = false;
      Serial.println("Disconnected STAGE1");
    }
    else {
      Serial.println("Connected");
    }
    PACKET_COUNT = 0;
    TCP_READ_COUNT = millis();
    TCP_READ_COUNT += 600;
    TCP_WRITE_COUNT = millis();
  }
}

void Reconnect(){
    Serial.println("Reconnecting");
    UIP.stop();
    while (!UIP.connect(COMP_IP, TCP_LCP)) {
      delay(100);
    }
    Serial.println("Reconnected");
    CONNECTED = true;
    CONNECTED_STAGE1 = true;
    TCP_READ_COUNT = millis();
    TCP_READ_COUNT += 600;
}

void UIPSleep(){
  Serial.println("Connection Sleep");
  UIP.stop();
  while(digitalRead(3) and digitalRead(4) and digitalRead(5)){
  while(digitalRead(3) and digitalRead(4) and digitalRead(5)){
    delay(100);
  }
  delay(30);
  }
  while(!digitalRead(3) or !digitalRead(4) or !digitalRead(5)){
    delay(20);
  }
  ConnectionSleep = false;
  ConnectionTimer = millis();
  Reconnect();
}
