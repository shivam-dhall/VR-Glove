#include <Wire.h>
#include <JY901.h>

bool isInit = false;

void setup() 
{
  uint32_t baud = 115200;
  Serial.begin(baud);
  delay(2000);
  sendRST();
}

void loop() 
{
    char a5[14] = "AT+CIPSEND=10";
    char a6[11] = "0123456789";
    if(isInit){
      Serial.print(a5);
      Serial.println();
      delay(10);
      Serial.print(a6);
      Serial.println();
    }
}

void printJY901(){
  
}

void sendRST(){
    char a0[3] = "AT";
    char a1[7] = "AT+RST";
    char a2[12] = "AT+CWMODE=1";
    char a3[36] = "AT+CWJAP=\"Ippclub\",\"coseastippclub\"";
    //char a3[27] = "AT+CWJAP=\"IOT1\",\"11111111\"";
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
}




