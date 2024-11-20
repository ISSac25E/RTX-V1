//
////TX
//int X = 0;
//boolean pin = false;
//int Code[][5] = {
//{30, 30, 30, 30, 30},//0
//{100, 30, 30, 30, 30},//1
//{30, 100, 30, 30, 30},//2
//{100, 100, 30, 30, 30},//3
//{30, 30, 100, 30, 30},//4
//{100, 30, 100, 30, 30},//5
//{30, 100, 100, 30, 30},//6
//{100, 100, 100, 30, 30},//7
//{30, 30, 30, 100, 30},//8
//{100, 30, 30, 100, 30},//9
//{30, 100, 30, 100, 30},//10
//{100, 100, 30, 100, 30},//11
//{30, 30, 100, 100, 30},//12
//{100, 30, 100, 100, 30},//13
//{30, 100, 100, 100, 30},//14
//{100, 100, 100, 100, 30},//15
//};
//void setup() {
//  pinMode(10, OUTPUT);
//  digitalWrite(10, LOW);
//}
//
//void loop() {
//  delay(1000);
//  while (X < 6) {
//
// digitalWrite(10, HIGH);
// delayMicroseconds(Code[5][X]);
//
//
//digitalWrite(10, LOW);
//delayMicroseconds(20);
//    X++;
//  }
//  digitalWrite(10, LOW);
//  pin = false;
//  X = 0;
//
//
//
//  delay(1000);
//
//
//
//  while (X < 6) {
//
// digitalWrite(10, HIGH);
// delayMicroseconds(Code[13][X]);
//
//
//digitalWrite(10, LOW);
//delayMicroseconds(20);
//    X++;
//  }
//  digitalWrite(10, LOW);
//  pin = false;
//  X = 0;
//}


//RX
boolean Done = false;
String str = "";
int X = 0;
void setup() {
  Serial.begin(2000000);
  pinMode(10, INPUT);
  delay(500);
}

void loop() {
  if(digitalRead(10) == HIGH){
    Read();
  }
}

void Read(){
  while( X<30){
if(digitalRead(10) == HIGH){
Serial.print("-");
//str += 1;
}
if(digitalRead(10) == LOW){
Serial.print("_");
//str += 0;
}
X++;
}
Serial.println();
//Serial.println(str);
str = "";
X = 0;
}
