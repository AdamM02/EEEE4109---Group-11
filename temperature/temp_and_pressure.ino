#include <Wire.h>
#include "SparkFun_LPS28DFW_Arduino_Library.h"

#define temp_sensor A0

// Create a new sensor object
LPS28DFW pressureSensor;

// I2C address selection
//uint8_t i2cAddress = LPS28DFW_I2C_ADDRESS_DEFAULT; // 0x5C
uint8_t i2cAddress = LPS28DFW_I2C_ADDRESS_SECONDARY; // 0x5D

void setup() {
 // put your setup code here, to run once:
  pinMode(temp_sensor, INPUT);
  analogReference(INTERNAL); // Taking internal reference 1.1V ADC of arduino
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

   // Serial.println("LPS28DFW connected!");
}

void loop() {
  // Get measurements from the sensor. This must be called before accessing
  // the pressure data, otherwise it will never update
  pressureSensor.getSensorData();
  delay(100);
  
  float sensorValue = analogRead(A0);
  float voltage = sensorValue * (1.1/ 1023.0);
  voltage = voltage-0.101; //calibration when changing rheostat to fixed resistor
  
 //y=mx+c
  float slope=21.255;
  float c=92.16;
  float R=slope*voltage+c;
 
  Serial.print("R= ");
  Serial.print(R);
  Serial.print(" ohm, ");

  float temp=(R-100)/0.385;
  temp=temp-0.2; //calibration
  Serial.print("Temperature Sensor: ");
  Serial.print(temp);
  Serial.print(" deg\n");

  // Print temperature and pressure
  Serial.print("Pressure Sensor: ");
  Serial.print(pressureSensor.data.heat.deg_c);
  Serial.print(" deg\n");
  float diff=pressureSensor.data.heat.deg_c-temp;
  Serial.print("Temp diference between sensors: ");
  Serial.print(diff);
  Serial.print("\n");
  delay(1000);
}
