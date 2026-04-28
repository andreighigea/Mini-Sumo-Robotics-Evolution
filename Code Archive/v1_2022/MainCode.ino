#include<QTRSensors.h>
#include<IRLib.h>
QTRSensors qtr;
IRdecode My_Decoder;
IRrecv My_Reciever(2);
const uint8_t SensorCount = 2;
uint16_t sensorValues[SensorCount];
char out[20];
long strlint;
String hex = "";
String tempHex = "";
int state = 0;
bool start=true;
const int motorPin1=3;
const int motorPin2=5;
const int motorPin3=6;
const int motorPin4=9;
const int sensorDreapta=A0;
const int sensorStanga=A1;
const int sensorFataDreapta=A2;
const int sensorFataStanga=A3;
const int sensorSpate=4;
const int lineD=A7;
const int lineS=A6;
int value=0;
int lRight=0;
int lLeft=0;
bool dFR=false;
bool dFL=false;
bool dR=false;
bool dL=false;
bool sP=false;
void setup(){
  pinMode(motorPin1,OUTPUT);
  pinMode(motorPin2,OUTPUT);
  pinMode(motorPin3,OUTPUT);
  pinMode(motorPin4,OUTPUT);
  pinMode(sensorSpate, INPUT);
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){A7, A6}, SensorCount);
  Serial.begin(9600);
  My_Reciever.enableIRIn(); //start recieving
}
void loop(){
      
  qtr.read(sensorValues);
  lRight=analogRead(A7);
  lLeft=analogRead(A6);
  
  Serial.print(lRight);
  Serial.print("\n");
  if (My_Reciever.GetResults(&My_Decoder)){
    My_Decoder.decode();
    strlint = atol(String(My_Decoder.value).c_str());
    tempHex = String(strlint, HEX);
    sprintf(out, "%08X", strlint);
    if (hex == ""){
      hex = tempHex;
    }
    if (tempHex == hex){
      state += 1;
      if (state > 2){
        state = 1;
      }
    }
    My_Reciever.resume(); // restart reciever
  }
  if(state == 2){
    digitalWrite(13, HIGH);
    if(lLeft<370 || lRight<230){
      if (lLeft<370 && lRight >= 230){
        motor(255,0,0,255);
        delay (300);
        motor(255,0,255,0);
        delay (175);
      }
      else if (lRight < 230 && lLeft >= 370){
        motor(255,0,0,255);
        delay (300);
        motor(0,255,0,255);
        delay (175);
      }
      else{
        motor(255,0,0,255);
        delay (250);
        motor(255,0,255,0);
        delay (150);
      }
    }
    else{
      dFR=analogRead(sensorFataDreapta)>300;
      dFL=analogRead(sensorFataStanga)>300;
      dR=analogRead(sensorDreapta)>350;
      dL=analogRead(sensorStanga)>350;
      
      if (dFR && dFL){
        motor(0,255,255,0);
      }
      else if (dL && !dFL){
        motor(0,200,0,200);
        delay(175);
      }
      else if (dR && !dFR){
        motor(200,0,200,0);
        delay(175);
      }
      else if (dR && dFR){
        motor(0,75,0,200);
        delay(35);
      }
      else if (dL && dFL){
        motor(200,0,75,0);
        delay (35);
      }
      else if (dFR){
        motor(125,0,125,0);
        delay (45);
      }
      else if (dFL){
        motor(0,125,0,125);
        delay(45);
      }
      else{
        if (start){
          start = false;
          motor(0,255,255,0);
          delay(150);
          motor(0,0,0,0);
          delay(150);
          motor(0,200,200,0);
        }
        else{
          motor(0,255,255,0);
        }
      }
    }                                                                                                                                    
  }
  else if (state == 1){
    motor(0,0,0,0);
    digitalWrite(13, HIGH);
    delay(250);
    digitalWrite(13, LOW);
    delay(250);
  }
  else{
    motor(0,0,0,0);
    digitalWrite(13, LOW);
  }
}
void motor(int a, int b, int c, int d){
  analogWrite(motorPin1, a);
  analogWrite(motorPin2, b);
  analogWrite(motorPin3, c);
  analogWrite(motorPin4, d);
}