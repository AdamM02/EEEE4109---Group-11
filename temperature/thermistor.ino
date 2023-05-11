//https://www.thinksrs.com/downloads/programs/therm%20calc/ntccalibrator/ntccalculator.html

#include <Wire.h>
#include "SparkFun_LPS28DFW_Arduino_Library.h"

#define thermistor A0

// Create a new sensor object
LPS28DFW pressureSensor;

// I2C address selection
//uint8_t i2cAddress = LPS28DFW_I2C_ADDRESS_DEFAULT; // 0x5C
uint8_t i2cAddress = LPS28DFW_I2C_ADDRESS_SECONDARY; // 0x5D

void setup() {
 // put your setup code here, to run once:
  pinMode(thermistor, INPUT);
 // analogReference(INTERNAL); // Taking internal reference 1.1V ADC of arduino
  Serial.begin(9600);

  // Initialize the I2C library
  Wire.begin();
   while(pressureSensor.begin(i2cAddress) != LPS28DFW_OK)
    {
        // Not connected, inform user
        Serial.println("Error: LPS28DFW not connected, check wiring and I2C address!");

        // Wait a bit to see if connection is established
        delay(1000);
    }

    Serial.println("LPS28DFW connected!");
}
float R1 = 10000;
float logR2, R2, T;
float c1 = 0.8817492013e-3, c2 = 2.526768788e-04, c3 = 1.855573319e-07;

void loop() {
  // put your main code here, to run repeatedly:
  pressureSensor.getSensorData();
  delay(100);
   
  float Vin=3.3;
  float sensorValue = analogRead(A0);
  float V_ther= sensorValue * (3.3/ 662.0);
  Serial.print("sensorValue");
  Serial.print(sensorValue);
  Serial.print(" , V-ther= ");
  Serial.print(V_ther);
  Serial.print("\n");
 // R2=R1*((Vin/V_ther)-1);
 // logR2=log(R2);
  //T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T= -6.2961*V_ther*V_ther*V_ther + 35.282*V_ther*V_ther - 96.941*V_ther+ 117.09;
  T=T;
//float  Tc = T - 273.15;
  Serial.print("Temperature: "); 
 // Serial.print(Tf);
 // Serial.print(" F; ");
  Serial.print(T);
  Serial.println(" deg");
  Serial.print("Pressure Sensor: ");
  Serial.print(pressureSensor.data.heat.deg_c);
  Serial.print(" deg\n");
  float diff=pressureSensor.data.heat.deg_c-T;
  Serial.print("Temp diference between sensors: ");
  Serial.print(diff);
  Serial.print("\n");
  
  delay(1000);
}
