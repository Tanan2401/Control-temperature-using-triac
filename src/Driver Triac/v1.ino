#define TRIAC_PIN 5
int output;
void TriacControl()
{
  delayMicroseconds((1000-output)*10); 
  digitalWrite(TRIAC_PIN,HIGH);
  delayMicroseconds(100);
  digitalWrite(TRIAC_PIN,LOW);
}
void setup() {
  // put your setup code here, to run once:
  pinMode(TRIAC_PIN,OUTPUT);
  attachInterrupt(0, TriacControl, RISING);
  output= 900;
}

void loop() {
    output= 900;

}


