// -------------------------------------------------------------------------------------
//
// Peat bog monitoring system master device code
//
// Program to control the master device, including:
// - Use of an internal interrupt timer to control peripherals and ask slaves for data
// - Request and reception of data from slave devices 
// - Storage of recorded data on an SD card
// - Monitoring of time and date internally for printing on the SD card
//
// Required header files:
// Publically available header files:
// - SPI.h
// - RH_RF95.h
// - SdFat.h
// - avr/sleep.h
// - avr/power.h
// - avr/wdt.h
// - TimeLib.h
//
// Bespoke header files:
// - masterComms.h
// - microSDwrite.h
// - watchdog.h
//
// With special thanks to:
// - Mike McCauley of the RadioHead library: https://www.airspayce.com/mikem/arduino/RadioHead/
// - Paul Stogffregen of the TimeLib.h library: https://github.com/PaulStoffregen/Time
//   (Note: if commercialising permission would be required from Paul)
// - Bill Greiman of the SdFat.h library: https://github.com/greiman/SdFat
//
// Adam Machin, 2023
//
// --------------------------------------------------------------------------------------
#include <SPI.h>
#include <RH_RF95.h>
#include <stdlib.h>

#include <SdFat.h>

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#include <TimeLib.h>

#include "masterComms.h"
#include "microSDwrite.h"
#include "watchdog.h"

const int chipSelect = 4; //from SDwrite
#define NO_OF_SLAVES 1  //update after testing to real value

#define _hour 10
#define _minute 14
#define _second 0
#define _day 9
#define _month 05
#define _year 23
//USER WILL NEED TO UPDATE THESE VALUES BEFORE DEPLOYMENT

double previousPositions[NO_OF_SLAVES] = {0};

//Please have device connected by USB to monitor setup progress
//Do not remove device from power after setup
void setup() 
{
  if(!loraSetup())
  //setup lora communication module 
  {
    Serial.println("Error in LoRa setup");
    delay(100);
    exit(1);
    //exit if failed with error code 1
  }

  setTime(_hour, _minute, _second, _day, _month, _year);
  //set the time using the user inputted values

  watchDogInit();
  //initialise internal watchdog timer

  Serial.println("Initialisation complete.");
  delay(100); 

  //Serial.flush();

}


void loop()
{
  if(timeForData() == true)
  // function based on watchdog timer count
  {
      const int rows = 4;
      // represents the number of data points to collect: ID, height, pressure, temperature
      
      // dynamically allocate 2D array
      double* dataArray[rows];
      for(int i=0; i<rows; i++)
      {
        dataArray[i] = (double*)malloc(NO_OF_SLAVES * sizeof(double));
      }

       //collects data from slaves and updates height based on previous value
      if (getDataFromSlaves(dataArray) == true)
      {

        //Include for testing:
        for(int N=0; N<NO_OF_SLAVES; ++N)
        {
          for(int M=0; M<4; ++M)
          {
            Serial.println(dataArray[N][M]);
          }
        } 

        if(sd.begin(chipSelect, SPI_FULL_SPEED))
        //Test to see if SD card is inserted
        {
          writeDataToSD(dataArray, NO_OF_SLAVES);
        }
        else
        {
          //Include for testing:
          //Serial.println("No SD card detected");
        }
      }
      else
      {
        if(sd.begin(chipSelect, SPI_FULL_SPEED))
        //Test to see if SD card is inserted
        {
          writeErrorMessageToSD();
        }
        else
        {
          //Include for testing:
          //Serial.println("No SD card detected");
        }
      }


      //free up memory used by 2D array
      for (int i = 0; i < rows; i++)
      {
        free(dataArray[i]);
      }

      //re-enter sleep mode for power saving
      enterSleep();

      f_wdt=0;
  }
  else
  {
    
  }

}
