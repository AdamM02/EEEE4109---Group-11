#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 3
// pin number definitions

#define NO_OF_SLAVES 1  //update after testing to real value

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 434.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

bool loraSetup();
bool getDataFromSlaves(double** dataArray);
bool isConnectionGood();
void receiveData(double** dataArray);

// ----------------------------------------------------------------------------------------------------------------------
//
// Function to initialise the LoRa communication device's pin connections, frequency 
// and transmission power
//
// Input arguments: None
//
// Return type: Bool
//
// Required header files: None.
//
// Warning(s): Hardware connections with the Arduino must agree with the pin number
//             definitions above.
//             For more details on the pin connections, see: 
//             https://learn.adafruit.com/adafruit-rfm69hcw-and-rfm96-rfm95-rfm98-lora-packet-padio-breakouts/pinouts
//
// ----------------------------------------------------------------------------------------------------------------------
bool loraSetup()
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(9600);
  delay(100);

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed"); //for testing
    return false;
  }
  Serial.println("LoRa radio init OK!"); //for testing

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    return false;
  }
  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(10, false);

  return true;  
}

// ----------------------------------------------------------------------------------------------------------------------
//
// Function to test whether the connection between Master and Slave is estabilshed, and if so request data from slaves
//
// Input arguments: A pointer to an array of pointers (2D Array)
//
// Return type: Void
//
// Required header files: None.
//
// Warning(s): 
//
// ----------------------------------------------------------------------------------------------------------------------
bool getDataFromSlaves(double** dataArray)
{
  if(isConnectionGood() == false)
  {
    Serial.println("Error: No good connection established!");
    return false;
    //for debugging purposes.
  }
  else
  {
    receiveData(dataArray);
    return true;
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
// Warning(s): 
//
// ----------------------------------------------------------------------------------------------------------------------
bool isConnectionGood()
{
  char radiopacket[2];
  radiopacket[0] = 1;   // the 0th element has the value 1, which is used in the slave to determine that an incoming signal 
                        // is only testing that there is a connection
  rf95.send((uint8_t *)radiopacket, 2);
  Serial.println("Check has been sent"); //to be removed after testing
  rf95.waitPacketSent();
  
  // send a message to the slave to be received

  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  int receiverChecks = 0;
  // iterator to give the device 3 chances to establish a connection

  // note: a connection is established by the reception of data back from the slave

  do
  {
    if(rf95.waitAvailableTimeout(1000))
    //wait for reply from slave
    {
      if (rf95.recv(buf, &len))
      {
        Serial.println("Valid connection!"); //to be removed after testing
        return true;
      }
      else
      {
        ++receiverChecks; //increment upon failure
        delay(1000);
      }
    }
    else
    {
      ++receiverChecks; //increment upon failure
      delay(1000);
    }
  }
  while (receiverChecks < 3);

  Serial.println("Poor connection"); //to be removed after testing
  return false;
}

// ----------------------------------------------------------------------------------------------------------------------
//
// Function to ask for, and receive, data from the slaves
//
// Input arguments: A pointer to an array of pointers (2D Array)
//
// Return type: void
//
// Required header files: RH_RF95.h
//                        stdlib.h
//
// Warning(s): Should only be called once there is a verified connection between the master and the slave (isConnectionGood())
//             dataArray should have 4 rows
//
// ----------------------------------------------------------------------------------------------------------------------
void receiveData(double** dataArray)
{
  char radiopacket[20] = "Data prompt";
  rf95.send((uint8_t *)radiopacket, 20);
  //wait for packet to finish sending
  rf95.waitPacketSent();

  // send a message to the slave to request data

  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

    //no test for reception (should be done first in isConnectionGood())

    if(rf95.waitAvailableTimeout(10000))
    //wait for reply
    {
      if (rf95.recv(buf, &len))
      // on reception of a signal back from the slave
      {
        // code to get data from the slave into dataArray

        int counter(0);
        // tracks what data piece is currently being stored

        char workingCharacter;

        char slaveID[2];
        char height[8];
        char pressure[8];
        char temp[8];

        memset(slaveID, '\0', sizeof(slaveID));
        memset(height, '\0', sizeof(height));
        memset(pressure, '\0', sizeof(pressure));
        memset(temp, '\0', sizeof(temp));

        char workingString[10];
        memset(workingString, '\0', sizeof(temp));

        int iterator2 = 0;

          for(int i=1; i<30; ++i)
          { 
            if((char)buf[i] != ',')
            {
              workingCharacter = (char)buf[i];
              workingString[iterator2] = workingCharacter;
              ++iterator2;
            }
            else
            {
              if(counter==0)
              {
                for (int j=0; j<iterator2; ++j)
                {
                  slaveID[j] = workingString[j];
                }
              }
              else if(counter==1)
              {
                for (int j=0; j<iterator2; ++j)
                {
                  height[j] = workingString[j];
                }
              }
              else if(counter==2)
              {
                for (int j=0; j<iterator2; ++j)
                {
                  pressure[j] = workingString[j];
                }
              }
              else
              {
                for (int j=0; j<iterator2; ++j)
                {
                  temp[j] = workingString[j];
                }
              }

              memset(workingString, '\0', 8);
              counter++;
              iterator2 = 0;
            }
          }

        char *ptr;

        double slaveIDdouble = strtod(slaveID, &ptr);

        for (int i=0; i< NO_OF_SLAVES+1; ++i)
        {
          if(slaveIDdouble==i)
          {
            //Data is to be received in the form:
            //[ID, height, pressure, temperature]
  
            dataArray[i-1][0] = slaveIDdouble;
            dataArray[i-1][1] = strtod(height, &ptr);
            dataArray[i-1][2] = strtod(pressure, &ptr);
            dataArray[i-1][3] = strtod(temp, &ptr);
            //store values into dataArray
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
