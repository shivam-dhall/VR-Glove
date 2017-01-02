const float R = 20000.0;
 
void setup()
{
  Serial.begin( 9600 );
}
 
void loop()
{
    int value0 = analogRead( A0 );
    int value1 = analogRead( A1 );
    int value2 = analogRead( A2 );
    int value3 = analogRead( A3 );
    int value6 = analogRead( A6 );
    int value7 = analogRead( A7 );
    float curr =  value7 / R ;
     
    //float r1 = (value) / curr;//20000
    
    Serial.print(value7/ curr);
    Serial.print("  ");
    Serial.print((value6-value7)/ curr);
    Serial.print("  ");
    Serial.print((value3-value6)/ curr);
    Serial.print("  ");
    Serial.print((value2-value3)/ curr);
    Serial.print("  ");
    Serial.print((value1-value2)/ curr);
    Serial.print("  ");
    Serial.print((value0-value1)/ curr);
    Serial.print("  ");
    Serial.print((1023-value0)/ curr);
    Serial.println("  ");

    delay( 1000 );
}
