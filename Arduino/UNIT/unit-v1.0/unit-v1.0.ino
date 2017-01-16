#include <Wire.h>
#include <JY901.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads(0x48);

char head[15] = "AT+CIPSEND=186";

char data[186];

bool isInit = false;
//char data1[60];
//char data2[60];
//char data3[66];

void setup() 
{
   Serial.begin(115200);
   JY901.StartIIC();
   ads.begin();
   for(int i=0;i<186;++i)
      data[i]='b';
//   for(int i=0;i<60;++i){
//      data1[i] = 's';
//      data2[i] =  't';
//   }
//
//    for(int i=0;i<66;++i){
//      data3[i] = 'r';
//   }

   delay(2000);
   sendRST();
} 

void loop() 
{
   unsigned long start = millis();
   writeTime(start,55);
   jy901();
   
   short adc0,adc1,adc2,adc3;  // we read from the ADC, we have a sixteen bit integer as a result
   adc0 = ads.readADC_SingleEnded(0);
   adc1 = ads.readADC_SingleEnded(1);
   writeTime(start,56);
   jy901();
   
   adc2 = ads.readADC_SingleEnded(2);
   adc3 = ads.readADC_SingleEnded(3);
   
   writeTime(start,57);
   jy901();
   short value0 = analogRead( A0 );
   short value1 = analogRead( A1 );
   short value2 = analogRead( A2 );
   short value3 = analogRead( A3 );
   short value6 = analogRead( A6 );
   short value7 = analogRead( A7 );
   

   Serial.print(head);
   Serial.println();

   writeTime(start,58);
   jy901();  

   writeTime(start,59);
   jy901();
   int2char(start,60);
   Serial.write(data,186);
   Serial.println();
//   Serial.print("cost");
//   Serial.println(millis()-start);
    
}

void writeTime(unsigned long start,int curr){
   int t = (millis()-start);
   short2char(t,curr);
}


void dealShortData(int curr){
    int i = curr*3;
    if(data[i]==0&&data[i+1]==0){
      data[i]= (char)1;
      data[i+1] = (char)1;
      data[i+2] = (char)1;
    }
    else if(data[i] ==0 && data[i+1] != 0){
      if(data[i+1]==(char)1){
        data[i] = (char)2;
        data[i+2] = (char)2;
      }
      else{
        data[i] = (char)1;
        data[i+2] = (char)1;       
      }
    }
    else if(data[i] !=0 && data[i+1] == 0){
      if(data[i]==(char)1){
        data[i+1] = (char)2;
        data[i+2] = (char)2;
      }
      else{
        data[i+1] = (char)1;
        data[i+2] = (char)1;       
      }
    }
    else if(data[i]!=0 && data[i+1]!=0){
      if(data[i]!=1 && data[i+1]!=1)
        data[i+2] = 1;
      else if(data[i]!=2 && data[i+1]!=2)
        data[i+2] = 2;
      else if(data[i]!=3 && data[i+1]!=3)
        data[i+2] = 3;
    }
}


void sendRST(){
    char a0[3] = "AT";
    char a1[7] = "AT+RST";
    char a2[12] = "AT+CWMODE=1";
    //char a3[36] = "AT+CWJAP=\"Ippclub\",\"coseastippclub\"";
    char a3[26] = "AT+CWJAP=\"IOT\",\"11111111\"";
    //char a4[39] = "AT+CIPSTART=\"TCP\",\"223.3.156.253\",8080";
    //char a4[39] = "AT+CIPSTART=\"TCP\",\"192.168.191.1\",8080";
    char a4[40] = "AT+CIPSTART=\"TCP\",\"101.200.45.113\",8080";
  
    Serial.print(a3);
    Serial.println();
    delay(15000);
    
    Serial.print(a4);
    Serial.println();
    delay(2000);
    isInit = true;

    char head1[13] = "AT+CIPSEND=1";
    char data2[2] = "1";
    Serial.print(head1);
    Serial.println();
    delay(10);
    Serial.print(data2);
    Serial.println();
    
}

void jy901(){
    JY901.GetAcc();
    for(int j = 0;j<3;++j){
      int i1 = JY901.stcAcc.a[j];
    }
   
    JY901.GetGyro();  

    for(int j = 0;j<3;++j){
      int i1 = JY901.stcGyro.w[j];
    }

    JY901.GetAngle();
    for(int j = 0;j<3;++j){
      int i1 = JY901.stcAngle.Angle[j];
    }
}

void short2char(short d,int curr){//curr 第几个short
  int p = curr*3;
  data[p] = (char)((d >> 8) & 0xFF);
  data[p+1] = (char)(d & 0xFF);
  dealShortData(curr);
}

void int2char(unsigned long t,int curr){
  unsigned long d = millis();
  int p = curr*3;
  data[p] = (char)((d >> 24) & 0xFF);
  data[p+1] = (char)((d >> 16) & 0xFF);
  dealShortData(curr);
  data[p+3] = (char)((d >> 8) & 0xFF);
  data[p+4] = (char)(d & 0xFF);
  dealShortData(curr+1);
}



