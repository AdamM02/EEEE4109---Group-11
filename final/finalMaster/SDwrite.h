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
  myFile.print(day());
  myFile.print("/");
  myFile.print(month());
  myFile.print("/");
  myFile.print(year());
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
  myFile.print(hour());
  myFile.print(":");
  myFile.print(minute());
  myFile.print(":");
  myFile.print(second());
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
    //sd.errorHalt("opening peatdata.txt for write failed");
    return;
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
    //sd.errorHalt("opening peatdata.txt for write failed");
    return;
  }

  writeDateToSD();
  writeTimeToSD();

  myFile.println("No data received.");

  myFile.close();
  delay(100);
}
