int X = 0;

int Y = 0;
int Z = 0;
int I = 0;
int K = 0;
int A = 0;
int B = 0;
int C = 0;
void setup(){
  pinMode(9, OUTPUT);
  pinMode(12, INPUT_PULLUP);
  Serial.begin(31250);
  delay(220);
  Serial.write(0x90);
  Serial.write(0x7E);
  Serial.write(0x45);
  delay(200);
  Serial.write(0x90);
  Serial.write(0x00);
  Serial.write(0x45);
}
void loop(){
  while(digitalRead(12) == LOW & X <= 2000){
    if(B == 0){
      for(A = A; A < 255; A += 1){
    analogWrite(9, A);
    delay(10);
    X += 10;
    if(digitalRead(12) == HIGH || X >= 2000){
      return;
  }
  }
    B = 1;
  }
  if(B == 1){
  for(A = A; A > 70; A -= 1){
    analogWrite(9, A);
    delay(10);
    X += 10;
    if(digitalRead(12) == HIGH || X >= 2000){
      return;
  }
  }
    B = 0;
  }

}
if(X < 5){
  if(B == 0){
  for(A = A; A < 255; A += 1){
    analogWrite(9, A);
    delay(10);
    if(digitalRead(12) == LOW){
      return;
  }
  }
  B = 1;
  }
  if(B == 1){
  for(A = A; A > 70; A -= 1){
    analogWrite(9, A);
    delay(10);
    if(digitalRead(12) == LOW){
      return;
  }
  }
  B = 0;
  }
}

if(X > 2000){
  if(I == 0){
    I = 1;
    C = 1;
    if(Y == 0){
      K = 1;
    }
    if(Y == 1){
      K = 1;
    }
    if(Y == 2){
      K = 1;
    }
    if(Y == 3){
      K = 1;
    }
    if(Y == 4){
      K = 0;
    }
    if(Y == 5){
      K = 0;
    }
    if(Y == 6){
      K = 0;
    }
    if(Y == 7){
      K = 0;
    }
  }
    else{
      I = 0;
      C = 1;
    }
}
if(I == 0){
if(X > 400 & X < 2000){
  if(Z == 1){
  Z = 0;  
  }
  else{
    Z = 1;
  }
 }
}
if(I == 1){
if(X > 5 & X < 2000){
  if(Z == 1){
  Z = 0;  
  }
  else{
    Z = 1;
  }
 }
}
if(X > 5 & X < 400){
    if(I == 0){
    if (Y == 7) {
      Y = 0;
    } else {
      Y += 1;
    }
  }
}
if(X > 5){ 
if(I == 0){  
 if(Z == 0){

  if(Y == 0){ 
  Serial.write(0x90);
  Serial.write(0x00);
  Serial.write(0x45);
  }
  if(Y == 1){
  Serial.write(0x90);
  Serial.write(0x01);
  Serial.write(0x45);
  }
  if(Y == 2){ 
  Serial.write(0x90);
  Serial.write(0x02);
  Serial.write(0x45);
  }
  if(Y == 3){ 
  Serial.write(0x90);
  Serial.write(0x03);
  Serial.write(0x45);
  }
  if(Y == 4){ 
  Serial.write(0x90);
  Serial.write(0x04);
  Serial.write(0x45);
  }
  if(Y == 5){ 
  Serial.write(0x90);
  Serial.write(0x05);
  Serial.write(0x45);
  }
  if(Y == 6){ 
  Serial.write(0x90);
  Serial.write(0x06);
  Serial.write(0x45);
  }
  if(Y == 7){ 
  Serial.write(0x90);
  Serial.write(0x07);
  Serial.write(0x45);
  }
X = 0;
      if(C == 1){
    digitalWrite(9, LOW);
    delay(200);
    digitalWrite(9, HIGH);
    delay(200);
    digitalWrite(9, LOW);
    delay(200);
    digitalWrite(9, HIGH);
    C = 0;
  }
    while(digitalRead(12) == LOW){
    if(B == 0){
            for(A = A; A < 255; A += 1){
    analogWrite(9, A);
    delay(10);
    if(digitalRead(12) == HIGH){
      return;
  }
  }
  B = 1;
  }
    if(B == 1){
  for(A = A; A > 70; A -= 1){
    analogWrite(9, A);
    delay(10);
    if(digitalRead(12) == HIGH){
      return;
  }
  }
  B = 0;
    }
    }
 }
else{
 if(Z == 1){
  if(Y == 0){
  Serial.write(0x90);
  Serial.write(0x17);
  Serial.write(0x45);
  }
  if(Y == 1){
  Serial.write(0x90);
  Serial.write(0x10);
  Serial.write(0x45);
  }
  if(Y == 2){
  Serial.write(0x90);
  Serial.write(0x11);
  Serial.write(0x45);
  }
  if(Y == 3){ 
  Serial.write(0x90);
  Serial.write(0x12);
  Serial.write(0x45);
  }
  if(Y == 4){ 
  Serial.write(0x90);
  Serial.write(0x13);
  Serial.write(0x45);
  }
  if(Y == 5){ 
  Serial.write(0x90);
  Serial.write(0x14);
  Serial.write(0x45);
  }
  if(Y == 6){ 
  Serial.write(0x90);
  Serial.write(0x15);
  Serial.write(0x45);
  }
  if(Y == 7){ 
  Serial.write(0x90);
  Serial.write(0x16);
  Serial.write(0x45);
  }
 }
  X = 0;
  if(C == 1){
    digitalWrite(9, LOW);
    delay(200);
    digitalWrite(9, HIGH);
    delay(200);
    digitalWrite(9, LOW);
    delay(200);
    digitalWrite(9, HIGH);
    C = 0;
  }
    while(digitalRead(12) == LOW){
    if(B == 0){
            for(A = A; A < 255; A += 1){
    analogWrite(9, A);
    delay(10);
    if(digitalRead(12) == HIGH){
      return;
  }
  }
  B = 1;
  }
    if(B == 1){
  for(A = A; A > 70; A -= 1){
    analogWrite(9, A);
    delay(10);
    if(digitalRead(12) == HIGH){
      return;
  }
  }
  B = 0;
    }
    }
  }
}
else{
  if(X > 5){
   if(K == 0){
    if(Z == 1){
  Serial.write(0x90);
  Serial.write(0x21);
  Serial.write(0x45);
    }
    if(Z == 0){
  Serial.write(0x90);
  Serial.write(0x22);
  Serial.write(0x45);
    }
    X = 0;      
    if(C == 1){
    digitalWrite(9, LOW);
    delay(200);
    digitalWrite(9, HIGH);
    delay(200);
    digitalWrite(9, LOW);
    delay(200);
    digitalWrite(9, HIGH);
    C = 0;
  }
   }
 if(K == 1){
    if(Z == 1){
  Serial.write(0x90);
  Serial.write(0x25);
  Serial.write(0x45);
    }
    if(Z == 0){
  Serial.write(0x90);
  Serial.write(0x26);
  Serial.write(0x45);
    }
    X = 0;
      if(C == 1){
    digitalWrite(9, LOW);
    delay(200);
    digitalWrite(9, HIGH);
    delay(200);
    digitalWrite(9, LOW);
    delay(200);
    digitalWrite(9, HIGH);
    C = 0;
  }
   } 
    while(digitalRead(12) == LOW){
    if(B == 0){
            for(A = A; A < 255; A += 1){
    analogWrite(9, A);
    delay(10);
    if(digitalRead(12) == HIGH){
      return;
  }
  }
  B = 1;
  }
    if(B == 1){
  for(A = A; A > 70; A -= 1){
    analogWrite(9, A);
    delay(10);
    if(digitalRead(12) == HIGH){
      return;
  }
  }
  B = 0;
    }
    }
}
}
}
}
