#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 434.0


// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

const double SLAVE_ID = 1;

// ----------------------------------------------------------------------------------------------------------------------
//
// Function to test whether there is any message for the slave to receive
//
// Input arguments: None.
//
// Return type: bool
//
// Required header files: RH_RF95.h
//
// Warning(s): None.
//
// ----------------------------------------------------------------------------------------------------------------------

bool checkIfAvailable()
{
  if(rf95.available())
  {
    return true;
  }
  else
  {
    return false;
  }
}

// ------------------------------------------------------------------------------
//
// Function to send height, pressure and temperature to the master device
//
// Input arguments: double representing the height
//                  double representing the pressure
//                  double representing the temperature
//
// Return type: None.
//
// Required header files: RH_RF95.h
//
// Warning(s): None.
//
// ------------------------------------------------------------------------------
void sendDataToMaster(double dubHeight, double dubPressure, double dubTemperature)
{
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  digitalWrite(RFM95_CS, LOW);

  if(rf95.recv(buf, &len))
  {

    Serial.println(*buf);

    if(buf[0] == 1)
    {      
      Serial.println("Test passed.");
      //connection for the test from transmitter, 'isConnectionGood()' function
      char radiopacket[20] = "Test Passed.";

      rf95.send((uint8_t *)radiopacket, 20);
      rf95.waitPacketSent();

    }
    else if(buf[0] ==  "$")
    {

    }
    else if(buf[0] == "T")
    {

    }
    else
    {      
      //transform doubles to chars
      char dataPacket[30] = {0};
      dataPacket[0] = "$";

      char slaveID[2];
      char height[8];
      char pressure[8];
      char temp[8];

      char comma = ',';

      dtostrf(SLAVE_ID, 1, 0, slaveID);
      dtostrf(dubHeight, 1, 2, height);
      dtostrf(dubPressure,1, 2, pressure);
      dtostrf(dubTemperature, 1, 2, temp);

      strcat(dataPacket, slaveID);
      strncat(dataPacket, &comma, 1);

      strcat(dataPacket, height);
      strncat(dataPacket, &comma, 1);

      strcat(dataPacket, pressure);
      strncat(dataPacket, &comma, 1);

      strcat(dataPacket, temp);

      rf95.send((uint8_t *)dataPacket, 30);
      rf95.waitPacketSent();

      Serial.println("Sending data to you now...");
      Serial.println((char *)dataPacket);
      
    }
  }
  else
  {
    
  }
}
