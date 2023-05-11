// -------------------------------------------------------------------------------------
//
// Peat bog monitoring system slave device code
//
// Program to control the slave device, including:
// - Reception of requests from a master device
// - Sensing: Height, pressure and temperature
// - Sending of data back to the master device
//
// Required header files:
// Publically available header files:
// - Wire.h
// - SPI.h
// - RH_RF95.h
// - stdio.h
// - string.h
// - SparkFun_LPS28DFW_Arduino_Library.h
//
// Bespoke header files:
// - slaveComms.h
// - RotEncoder.h
//
// Adam Machin, 2023
//
// --------------------------------------------------------------------------------------
#include <Wire.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <stdio.h>
#include <string.h>
#include "slaveComms.h"

#include "RotEncoder.h"

#include <SparkFun_LPS28DFW_Arduino_Library.h>

#include "temperature.h"

using namespace std;

LPS28DFW pressureSensor; //create a pressure sensor object
uint8_t i2cAddress = LPS28DFW_I2C_ADDRESS_SECONDARY; // 0x5D

//Please have device connected by USB to monitor setup progress
//Do not remove device from power after setup
void setup()
{
  Serial.begin(9600);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, INPUT);
  pinMode(clockPin, OUTPUT);
  // initialise rotary encoder pins

  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);

  pinMode(thermistor, INPUT);

  if (!loraSetup())
    //setup lora communication module
  {
    Serial.println("Error in LoRa setup");
    delay(100);
    exit(1);
    //exit if failed with error code 1
  }

  Wire.begin();
  //enable I2C connection

  while (pressureSensor.begin(i2cAddress) != LPS28DFW_OK)
    //continue looping until pressure sensor has been correctly initialised
  {
    Serial.println("Error: LPS28DFW not connected, check wiring and I2C address!");
    // Print to serial monitor if pressure sensor has not initialised correctly
    delay(1000);
  }

  Serial.println("LPS28DFW connected!");

}

void loop()
{

  if (rf95.available())
    //if there is a connection with the master
  {

    //Only when the slave receives a '1' can it reply to the master saying there is a good connection
    //when there is a good connection, the master can send back the data prompt ('D') which the slave
    //needs to begin collecting data
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    digitalWrite(RFM95_CS, LOW);

    if(rf95.recv(buf, &len))
    {
      if(buf[0] == 1)
      {
        //this is only a prompt do not send data back
        confirmConnection();
      }
      else if (buf[0] == "T")
      {
        //not meant for the slave
      }
      else if (buf[0] == "$")
      {
        //not meant for the slave
      }
      else if (*buf == 68)
      {
        pressureSensor.getSensorData();
        //get updated data from the pressure sensor

        sendDataToMaster(convertToHeight(RotPrevPosCheck()), (double)pressureSensor.data.pressure.hpa * 100, calculateTemperature());
        //fetch and send relevant data to be sent to the master

        digitalWrite(5, HIGH);
        delay(1000);
        digitalWrite(5, LOW);
        delay(1000);
      }
      else
      {

      }

    }

    memset(buf, 0, len);
  }
  else
  {

  }
}
