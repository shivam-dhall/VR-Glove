#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <JY901.h>

Adafruit_ADS1115 ads(0x48);
const int timesPer100 = 10;
const int dataLength = 9*4*timesPer100+5+10*4;
char data[dataLength];
int pos = 0;
float Voltage = 0.0;
int const_dianzu = 20000;
bool isInit = false;
char head[15] = "AT+CIPSEND=100";
char d[100];

void setup() 
{
  uint32_t baud = 115200;
  Serial.begin(baud);
  JY901.StartIIC();
  ads.begin();
  for(int i=0;i<100;++i)
    d[i] = 'a';
  delay(2000);
  sendRST();
} 

void loop() 
{

 int2char(dataLength-4);
  for(int i=0;i< 3;++i){
       JY901.GetAcc();
       int2char((int)JY901.stcAcc.a[0]);// /32768*16
       int2char(JY901.stcAcc.a[1]);
       int2char(JY901.stcAcc.a[2]);
       JY901.GetGyro();  
       int2char(JY901.stcGyro.w[0]);// /32768*2000
       int2char(JY901.stcGyro.w[1]);
       int2char(JY901.stcGyro.w[2]);
       JY901.GetAngle();
       int2char(JY901.stcAngle.Angle[0]);// /32768*180
       int2char(JY901.stcAngle.Angle[1]);
       int2char(JY901.stcAngle.Angle[2]);

      if(i==9){
         int value0 = analogRead( A0 );
         int value1 = analogRead( A1 );
         int value2 = analogRead( A2 );
         int value3 = analogRead( A3 );
         int value6 = analogRead( A6 );
         int value7 = analogRead( A7 );
         //float curr =  value7 / const_dianzu;
  
         //int2char((int)((value6-value7)/ curr));
         //int2char((int)((value3-value6)/ curr));
         //int2char((int)((value2-value3)/ curr));
         //int2char((int)((value1-value2)/ curr));
         //int2char((int)((value0-value1)/ curr));
         //int2char((int)((1023-value0)/ curr));
         int2char(value0);
         int2char(value1);
         int2char(value2);
         int2char(value3);
         int2char(value6);
         int2char(value7);
  
         int adc0,adc1,adc2,adc3;  // we read from the ADC, we have a sixteen bit integer as a result
         adc0 = ads.readADC_SingleEnded(0);
         adc1 = ads.readADC_SingleEnded(1);
         adc2 = ads.readADC_SingleEnded(2);
         adc3 = ads.readADC_SingleEnded(3);
         //float current = adc0/const_dianzu;
         //int2char((int)((adc2-adc3)/current));
         //int2char((int)((adc1-adc2)/current));
         //int2char((int)((adc0-adc1)/current));
         //int2char((int)((22170-adc0)/current));   
         int2char(adc0);
         int2char(adc1);
         int2char(adc2);
         int2char(adc3);
      }
  
  }
  data[pos]='\0';
  pos = 0;
  
  if(isInit){
      Serial.print(head);
      Serial.println();
      delay(10);
      Serial.print(d);
      Serial.println();
  }

}

void test(){
   
}

void int2char(int size){
  data[pos++] = (char)((size >> 24) & 0xFF);
  data[pos++] = (char)((size >> 16) & 0xFF);
  data[pos++] = (char)((size >> 8) & 0xFF);
  data[pos++] = (char)(size & 0xFF);
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











