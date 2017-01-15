#include <Wire.h>
#include <JY901.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads(0x48);

char head[15] = "AT+CIPSEND=186";
//char data[1];
char data1[63];
char data2[60];
char data3[60];
bool isInit = false;

void setup() 
{
   Serial.begin(115200);
   JY901.StartIIC();
   ads.begin();
   for(int i=0;i<60;++i){
      data3[i] = 'c';
      data2[i] = 'b';
   }
    for(int i=0;i<63;++i){
      data1[i] = 'a';
   }

//   delay(2000);
//   sendRST();
} 

void loop() 
{
  
   unsigned long start = millis();
   int2char(start,19,1);
   jy901();//1
   
   short adc0,adc1,adc2,adc3;  // we read from the ADC, we have a sixteen bit integer as a result
   adc0 = ads.readADC_SingleEnded(0);
   adc1 = ads.readADC_SingleEnded(1);
   
   writeTime(start,0,2);
   jy901();//2
   
   adc2 = ads.readADC_SingleEnded(2);
   adc3 = ads.readADC_SingleEnded(3);
   
   writeTime(start,0,3);
   jy901();//3
   
   short value0 = analogRead( A0 );
   short value1 = analogRead( A1 );
   short value2 = analogRead( A2 );
   short value3 = analogRead( A3 );
   short value6 = analogRead( A6 );
   short value7 = analogRead( A7 );

   char temp1[63];
   for(int i=0;i<63;++i)
      temp1[i] = data1[i];
   
   Serial.print(head);
   Serial.println();

   Serial.println(temp1);
   writeTime(start,10,2);
   jy901();//4
      char temp2[60];
   for(int i=0;i<60;++i)
      temp2[i] = data2[i];
   Serial.println(temp2);
   
   writeTime(start,10,3);
   jy901();//5
      char temp3[60];
   for(int i=0;i<60;++i)
      temp3[i] = data3[i];
   Serial.println(temp3);
   //Serial.print(data3);
   Serial.println();
//   Serial.print("cost");
//   Serial.println(millis()-start);
    
}

void writeTime(unsigned long start,int curr,int type){
   int t = (millis()-start);
   short2char(t,curr,type);
}


void dealShortData(int curr,int type){
    char* data;
    if(type==1)
      data = data1;
    else if(type==2)
      data = data2;
    else if(type==3)
      data = data3;
  
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

void short2char(short d,int curr,int type){//curr 第几个short
  char* data;
  if(type==1)
    data = data1;
  else if(type==2)
    data = data2;
  else if(type==3)
    data = data3;
  
  int p = curr*3;
  data[p] = (char)((d >> 8) & 0xFF);
  data[p+1] = (char)(d & 0xFF);
  dealShortData(curr,type);
}

void int2char(unsigned long t,int curr,int type){
  char* data;
  if(type==1)
    data = data1;
  else if(type==2)
    data = data2;
  else if(type==3)
    data = data3;
  
  unsigned long d = millis();
  int p = curr*3;
  data[p] = (char)((d >> 24) & 0xFF);
  data[p+1] = (char)((d >> 16) & 0xFF);
  dealShortData(curr,type);
  data[p+3] = (char)((d >> 8) & 0xFF);
  data[p+4] = (char)(d & 0xFF);
  dealShortData(curr+1,type);
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



