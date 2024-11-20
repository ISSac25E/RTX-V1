unsigned long int AA = 0;
unsigned long int AB = 0;
unsigned long int AC = 0;
 unsigned long int mill = 0;
boolean A = false;
boolean B = false;
boolean C = false;
void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);  
  pinMode(4, INPUT_PULLUP);
}

void loop() {
mill = millis();
 if(digitalRead(2) == LOW and A == false){
  AA = mill;
  A = true;
  Serial.println("2 Pressed");
 }
 if(digitalRead(2) == HIGH and A == true){
    AA = mill -= AA;
    A = false;
  Serial.print("2 Released ");
  Serial.println(AA);
 }
mill = millis();
 
  if(digitalRead(3) == LOW and B == false){
    AB = mill;
    B = true;
  Serial.println("3 Pressed");
 }
 if(digitalRead(3) == HIGH and B == true){
    AB = mill -= AB;
    B = false;
  Serial.print("3 Released ");
  Serial.println(AB);
 }
mill = millis();
 
  if(digitalRead(4) == LOW and C == false){
    AC = mill;
    C = true;
  Serial.println("4 Pressed");
 }
 if(digitalRead(4) == HIGH and C == true){
  AC = mill -= AC;
    C = false;
  Serial.print("4 Released ");
  Serial.println(AC);
 }
 delay(10);
}
