SdFat sd;
SdFile myFile;


// ----------------------------------------------------------------------------------------------------------------------
//
// Function to write data in a 2D array onto an SD card
//
// Input arguments: 2D Array of type double, my2DArray[][4]
//                  An integer of how many rows are in the 2D Array, noOfRows
//
// Return type: void.
//
// Required header files: SdFat.h
//
// Warning(s): noOfRows should be the same as the number of columns in the 2D array
//
// ----------------------------------------------------------------------------------------------------------------------
void writeDataToSD(double my2DArray[][4], int noOfRows)
{
  if(!myFile.open("test.txt", O_RDWR | O_CREAT | O_AT_END))
  //check if file 'test.txt' can be opened
  {
    sd.errorHalt("opening peatdata.txt for write failed");
    //error which completely stops the code
  }

  //myFile.print("05/04/2023");
  //myFile.print("\t");
  //myFile.print("15:05:34");
  //myFile.print("\t");

  for(int N=0; N<noOfRows; ++N)
  {
    for(int M=0; M<3; ++M)
    {
      myFile.print(my2DArray[N][M]);
      myFile.print("\t");
    }

    myFile.println(my2DArray[N][3]);

    //loop through passed 2D array, writing all data to micro-SD card
  }

  myFile.close();
  //close the file after writing
  delay(5000);
}
