SdFat sd;
SdFile myFile;

// ----------------------------------------------------------------------------------------------------------------------
//
// Function to get the write the current date of the device to the SD card
//
// Input arguments: None.
//
// Return type: None.
//
// Required header files: stdlib.h
//                        TimeLib.h
//
// Warning(s): Printed value is based on constants from when the timer started - not the system clock / serial monitor.
//
// ----------------------------------------------------------------------------------------------------------------------
void writeDateToSD()
{
  char date[20] = {0};

  char __day[5];
  char __month[5];
  char __year[5];

  strcat(date, itoa(day(), __day, 10));
  strcat(date, "/");
  strcat(date, itoa(month(), __month, 10));
  strcat(date, "/");
  strcat(date, itoa(year(), __year, 10));

  //convert integer to a string, then append to the end of date

  myFile.print(date);
  myFile.print("\t");
}

// ----------------------------------------------------------------------------------------------------------------------
//
// Function to get the write the current time of the device to the SD card
//
// Input arguments: None.
//
// Return type: None.
//
// Required header files: stdlib.h
//                        TimeLib.h
//
// Warning(s): Printed value is based on constants from when the timer started - not the system clock / serial monitor.
//
// ----------------------------------------------------------------------------------------------------------------------
void writeTimeToSD()
{
  char time[20] = {0};

  char __hour[5];
  char __minute[5];
  char __second[5];

  strcat(time, itoa(hour(), __hour, 10));
  strcat(time, ":");
  strcat(time, itoa(minute(), __minute, 10));
  strcat(time, ":");
  strcat(time, itoa(second(), __second, 10));

  //convert integer to a string, then append to the end of time

  myFile.print(time);
  myFile.print("\t");

  return;
}



// ----------------------------------------------------------------------------------------------------------------------
//
// Function to write data in a 2D array onto an SD card
//
// Input arguments: Pointer to a string storing the date, fullDate
//                  Pointer to a string storing the time, fullTime
//                  A pointer to an array of pointers (2D Array)
//                  An integer to symbolise how many columns are in the array
//
// Return type: void.
//
// Required header files: SdFat.h
//
// Warning(s): noOfSlaves should be the same as the number of columns in the 2D array
//
// ----------------------------------------------------------------------------------------------------------------------
void writeDataToSD(double** my2DArray, int noOfSlaves)
{
  if(!myFile.open("peatdata.txt", O_RDWR | O_CREAT | O_AT_END))
  {
    //some sort of error message that will not completely stop the code
    //LED ?
    sd.errorHalt("opening peatdata.txt for write failed");
  }

  for(int N=0; N<noOfSlaves; ++N)
  //print each row and column from the 2D array
  {

    writeDateToSD();
    writeTimeToSD();

    //print date and time to SD card first

    for(int M=0; M<3; ++M)
    {
      myFile.print(my2DArray[N][M]);
      myFile.print("\t");
      //print data from 2D array
    }

    myFile.println(my2DArray[N][3]);
    //print final array value
  }

  myFile.close();
  delay(100);
}

void writeErrorMessageToSD()
{
  if(!myFile.open("peatdata.txt", O_RDWR | O_CREAT | O_AT_END))
  {
    //some sort of error message that will not completely stop the code
    //LED ?
    sd.errorHalt("opening peatdata.txt for write failed");
  }

  writeDateToSD();
  writeTimeToSD();

  myFile.println("No data received.");

  myFile.close();
  delay(100);
}
