int relay = 7;
void setup() {
pinMode(relay, OUTPUT);

}

void loop() {
  digitalWrite(relay, LOW);   // реле включено
  delay(5000);               
  digitalWrite(relay, HIGH);  // реле выключено
  delay(5000);  
 }
