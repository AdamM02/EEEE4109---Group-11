#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 3
// pin number definitions

#define NO_OF_SLAVES 1  //update after testing to real value

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 434.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void getDataFromSlaves(double my2DArrayA[NO_OF_SLAVES][4]);
bool isConnectionGood();
void receiveData(double my2DArrayB[NO_OF_SLAVES][4]);

// ----------------------------------------------------------------------------------------------------------------------
//
// Function to test whether the connection between Master and Slave is estabilshed, and if so request data from slaves
//
// Input arguments: A 2D Array with NO_OF_SLAVES rows and 4 columns
//
// Return type: Void
//
// Required header files: None.
//
// Warning(s): None.
//
// ----------------------------------------------------------------------------------------------------------------------
void getDataFromSlaves(double my2DArrayA[NO_OF_SLAVES][4])
{
  if(isConnectionGood() == false)
    {
      Serial.println("Error: No good connection established!");
      //for debugging purposes.
    }
    else
    {
      receiveData(my2DArrayA);
    }

    return;
}

// ----------------------------------------------------------------------------------------------------------------------
//
// Function to test if there is a valid connection established between the master and the slaves
//
// Input arguments: None.
//
// Return type: bool.
//
// Required header files: RH_RF95.h
//
// Warning(s): None.
//
// ----------------------------------------------------------------------------------------------------------------------
bool isConnectionGood()
{
  
  char radiopacket[2];
  radiopacket[0] = 1;
  rf95.send((uint8_t *)radiopacket, 2);
  rf95.waitPacketSent();

  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  int receiverChecks = 0;

  do
  {
    if(rf95.waitAvailableTimeout(1000))
    //wait for reply
    {
      if(rf95.recv(buf, &len))
      {
        Serial.println("Valid connection!");
        return true;
      }
      else
      {
        ++receiverChecks;
        delay(3000);
      }
    }
    else
    {
      ++receiverChecks;
      delay(3000);
    }
  }
  while (receiverChecks < 3);

  Serial.println("Poor connection");
  return false;
}

// ----------------------------------------------------------------------------------------------------------------------
//
// Function to ask for, and receive, data from the slaves
//
// Input arguments: A 2D Array of NO_OF_SLAVES rows and 4 columns
//
// Return type: void
//
// Required header files: RH_RF95.h
//
// Warning(s): Should only be called once there is a verified connection between the master and the slave (isConnectionGood())
//
// ----------------------------------------------------------------------------------------------------------------------
void receiveData(double my2DArrayB[NO_OF_SLAVES][4])
{
  char radiopacket[20] = "Data prompt";
  rf95.send((uint8_t *)radiopacket, 20);
  //wait for packet to finish sending
  rf95.waitPacketSent();

  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  //no need to test if reception is good as this is tested in isConnectionGood which was tested first.

    if(rf95.waitAvailableTimeout(1000))
    //wait for reply
    {
      if (rf95.recv(buf, &len))
      {

        int counter(0);

        char mycharacter;

        char slaveID[2];
        char height[8];
        char pressure[8];
        char temp[8];

        memset(slaveID, '\0', sizeof(slaveID));
        memset(height, '\0', sizeof(height));
        memset(pressure, '\0', sizeof(pressure));
        memset(temp, '\0', sizeof(temp));


        for(int noOfCommas=0; noOfCommas<3; ++noOfCommas)
        {
          for(int i=1; i<30; ++i)
          {
            if((char)buf[i] != ',')
            {
              mycharacter = (char)buf[i];
              //add to end of either id, height, pressure or temp based
              //on 'counter'
              if(counter==0)
              {
                strncat(slaveID, &mycharacter, 1);
              }
              else if(counter==1)
              {
                strncat(height, &mycharacter, 1);
              }
              else if(counter==2)
              {
                strncat(pressure, &mycharacter, 1);
              }
              else if(counter==3)
              {
                strncat(temp, &mycharacter, 1);
              }
              else
              {

              }
            }
            else
            {
              counter++;
            }
          }
        }

        char *ptr;

        double iddub = strtod(slaveID, &ptr);
        double heightdub = strtod(height, &ptr);
        double pressuredub = strtod(pressure, &ptr);
        double tempdub = strtod(temp, &ptr);

        //convert to doubles to be able to operate on data if required

        for (int i=0; i< NO_OF_SLAVES+1; ++i)
        {
          if(iddub==i)
          {
            //store data in 2D array 
            my2DArrayB[i-1][0] = iddub;
            my2DArrayB[i-1][1] = heightdub;
            my2DArrayB[i-1][2] = pressuredub;
            my2DArrayB[i-1][3] = tempdub;
          }
          else
          {
      
          }
        }
      }
      else
      {

      }
    }
    else
    {

    }  
}
