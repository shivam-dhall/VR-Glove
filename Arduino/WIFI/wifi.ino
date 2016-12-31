#include <SoftwareSerial.h>
SoftwareSerial softSerial(10, 11); // RX, TX

int state = 0;
void setup() 
{
  uint32_t baud = 115200;
  Serial.begin(baud);
  softSerial.begin(baud);
  Serial.print("SETUP!! @");
  Serial.println(baud);
}

void loop() 
{
    if(state == 0)
    {
      Serial.println("----");
       delay(2000);
        sendRST();
        ++state;
    }
    else if(state==1){
      
    }

      while(softSerial.available() > 0) 
    {
      char a = softSerial.read();
      if(a == '\0')
        continue;
      if(a != '\r' && a != '\n' && (a < 32))
        continue;
      Serial.print(a);
    }
    
    while(Serial.available() > 0)
    {
      char a = Serial.read();
      softSerial.write(a);
    }
}

void sendRST(){

    char a0[3] = "AT";
    char a1[7] = "AT+RST";
    char a2[12] = "AT+CWMODE=1";
    char a3[27] = "AT+CWJAP=\"IOT1\",\"11111111\"";
    //char a4[39] = "AT+CIPSTART=\"TCP\",\"223.3.156.253\",8080";
    char a4[39] = "AT+CIPSTART=\"TCP\",\"192.168.191.1\",8080";
    char a5[14] = "AT+CIPSEND=10";
    char a6[11] = "0123456789";
   
    Serial.println("s6666");
    softSerial.print(a3);
    softSerial.println();
    delay(15000);
    softPrint();
    delay(100);
    softPrint();
    delay(100);
    softPrint();

    Serial.println("s7777");
    softSerial.print(a4);
    softSerial.println();
    delay(2000);
    softPrint();

    Serial.println("s8888");
    softSerial.print(a5);
    softSerial.println();
    delay(20);
    softSerial.print(a6);
    softSerial.println();
    delay(100);
    softPrint();
    
}

void softPrint(){
  while(softSerial.available() > 0) 
    {
      char a = softSerial.read();
      if(a == '\0')
        continue;
      if(a != '\r' && a != '\n' && (a < 32))
        continue;
      Serial.print(a);
    }
}


