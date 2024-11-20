int X = 0;
int O = 0;
int I = 0;
void setup() {
  Serial.begin(115200);
  pinMode(10, INPUT);

}

void loop() {
  if (digitalRead(10) == HIGH) {
    while (X < 80) {
      X++;
      while (digitalRead(10) == HIGH) {
        I++;
      }
      if (I > 2) {
        Serial.print("1");
      }
      else{
        Serial.print("0");
      }
      I = 0;
        while (digitalRead(10) == LOW) {
          I++;
        }
        if (I > 2) {
        Serial.print("1");
      }
      else{
        Serial.print("0");
      }
      I = 0;

      }
      X = 0;
      Serial.println();
    }

  }
