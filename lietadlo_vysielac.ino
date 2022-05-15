
#include <SFE_BMP180.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
SFE_BMP180 pressure;
#define ALTITUDE 362.0 // Altitude Banska Bystrica m n.m.   //BMP180
float voltage = 0.0;
float sensorValue = 0.0f; 
float vout = 0.0;
float vin = 0.0;
float R1 = 30000.0;
float R2 = 7450.0;

void setup()
{
  Serial.begin(9600);
  Serial.println("");
  Serial.println("REBOOT SERIAL");
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }
  if (!LoRa.begin(433E6)) { // or 915E6, the MHz speed of yout module
    Serial.println("Starting Lora failed!");
    while (1);
  }
  Serial.println("Serial Sender started success");
}

void loop()
{
  LoRa.beginPacket();
  for(int counter = 1, analogIN = 0; analogIN < 8;){
  if(analogIN == 4) analogIN = analogIN + 2;              //VOLTAGE   preskakujem pin A4 a A5
    sensorValue = analogRead(analogIN);
    vout = (sensorValue * 5.0) / 1024.0; 
    voltage = vout / (R2/(R1+R2));   
    Serial.println("");                                     //VOLTAGE
    Serial.print("Battery ");                               //VOLTAGE
    Serial.print(counter);                                  //VOLTAGE
    Serial.print(" voltage: ");                             //VOLTAGE
    Serial.print(voltage);//print the voltge              //VOLTAGE
    LoRa.println(voltage, 1);                             //LORA
    Serial.print(" V");                                     //VOLTAGE
    analogIN++;                                             //VOLTAGE
    counter++;
    
    delay(100);                                             //VOLTAGE
  }
  char status;                                  //BMP180
  double T,P,p0,a;                              //BMP180
  status = pressure.startTemperature();         //BMP180
  if (status != 0)                              //BMP180
  {
    // Wait for the measurement to complete:    //BMP180
    delay(status);                              //BMP180
    status = pressure.getTemperature(T);        //BMP180
    if (status != 0)                            //BMP180
    {
      // Print out the measurement:
      Serial.println("");                       //BMP180
      Serial.print("temperature: ");            //BMP180
      Serial.print(T,2);                        //BMP180
      LoRa.println(T,0);
      Serial.print(" deg C");                   //BMP180
      status = pressure.startPressure(3);       //BMP180
      if (status != 0)                          //BMP180
      {
        // Wait for the measurement to complete:
        delay(status);                          //BMP180
        status = pressure.getPressure(P,T);     //BMP180
        if (status != 0)                        //BMP180
        {
          // Print out the measurement:
          Serial.println("");                   //BMP180
          Serial.print("absolute pressure: ");  //BMP180
          Serial.print(P,2);                    //BMP180
          LoRa.println(P,1);
          Serial.print(" mb");                  //BMP180
        }
        else Serial.println("error retrieving pressure measurement\n");   //BMP180
      }
      else Serial.println("error starting pressure measurement\n");       //BMP180
    }
    else Serial.println("error retrieving temperature measurement\n");    //BMP180
  }
  else Serial.println("error starting temperature measurement\n");        //BMP180

  
  Serial.print("lietadlo_vysliela");
  LoRa.endPacket();
  delay(1000);  // Pause for 5 seconds.                                   //BMP180
}
