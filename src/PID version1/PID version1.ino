#include <OneWire.h>
#include <DallasTemperature.h>
unsigned int t;

#define ONE_WIRE_BUS 13
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int TRIAC_PIN =12;
int output=0;
int adc=0;
int Kp= 2;
int Ki= 0;
int Kd =1;
#define T 0.01
float  a, b, c,err, LastOutput;
int temp;
unsigned int   E1 =0, E2 = 0,ghnd;
void ICACHE_RAM_ATTR TriacControl()
{
//  QuetLed();
  delayMicroseconds((1000-output)*10); 
  digitalWrite(TRIAC_PIN,HIGH);
  delayMicroseconds(100);
  digitalWrite(TRIAC_PIN,LOW);
}
float pid(int limit, float temp) {
   int output_;
   err = abs(limit - temp);
   
   a = 2*T*Kp + Ki*T*T + 2*Kd;
   b = Ki*T*T - 4*Kd - 2*T*Kp;
   c = 2*Kd;
   
   output_ = (a*err + b*E1 + c*E2 + 2*T*LastOutput)/(2*T);
   LastOutput = output_;
   E2 = E1; 
   E1 = err;
   
   if(output_ > 255) 
      output_ = 255;
   if(output_ < 0) 
      output_ = 0;
   return output_;
}
void setup() {
  // put your setup code here, to run once:

  Serial.begin(19200);
      sensors.begin(); // Bắt đầu đọc cảm biến
  pinMode(TRIAC_PIN,OUTPUT);
  digitalPinToInterrupt(14);
  attachInterrupt(14, TriacControl, RISING);

//  adc=1000;
}

void loop() {
  String data_ ,mode_,val;
  int moc;
  int data;
   if(Serial.available() > 0)
  {
    val = Serial.readStringUntil('\n');
    for (int i = 0; i < val.length(); i++) {
    if (val.charAt(i) == '=') {
        moc = i; //Tìm vị trí của dấu ""
      }
    }
   mode_=val;
   data_=val;
   mode_.remove(moc);
   data_.remove(0,moc+1);
   data=data_.toInt();
  }
     if(mode_ == "P")
   {
    Kp = data;
   }
   if(mode_ == "I")
   {
    Ki = data;
   }
   if(mode_ == "D")
   {
    Kd = data;
   }
   if(mode_ == "S")
   {
    ghnd = data;
   }
  sensors.requestTemperatures();
  temp= sensors.getTempCByIndex(0);
  output=map(pid(ghnd,temp),0,255,100,900);
  Serial.println(output);
  Serial.print("temp"); Serial.println(temp);
  Serial.print("              set"); Serial.println(ghnd);
  Serial.print("              PID "); Serial.println(pid(ghnd,temp));
  Serial.print("                            P "); Serial.print(Kp); Serial.print("  I "); Serial.print(Ki); Serial.print("  D "); Serial.print(Kd); Serial.println("   ");
}

