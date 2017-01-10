#include <Wire.h>
#include <JY901.h>
/*
Test on Uno R3.
JY901    UnoR3
SDA <---> SDA
SCL <---> SCL
*/
void setup() 
{
  Serial.begin(9600);
  JY901.StartIIC();
} 

void loop() 
{
          
  JY901.GetAcc();
  Serial.print("Acc:");Serial.print((float)JY901.stcAcc.a[0]/32768*16);Serial.print(" ");Serial.print((float)JY901.stcAcc.a[1]/32768*16);Serial.print(" ");Serial.println((float)JY901.stcAcc.a[2]/32768*16);
  
  JY901.GetGyro();  
  Serial.print("Gyro:");Serial.print((float)JY901.stcGyro.w[0]/32768*2000);Serial.print(" ");Serial.print((float)JY901.stcGyro.w[1]/32768*2000);Serial.print(" ");Serial.println((float)JY901.stcGyro.w[2]/32768*2000);
  
  JY901.GetAngle();
  Serial.print("Angle:");Serial.print((float)JY901.stcAngle.Angle[0]/32768*180);Serial.print(" ");Serial.print((float)JY901.stcAngle.Angle[1]/32768*180);Serial.print(" ");Serial.println((float)JY901.stcAngle.Angle[2]/32768*180);
  
 
  delay(500);
}



