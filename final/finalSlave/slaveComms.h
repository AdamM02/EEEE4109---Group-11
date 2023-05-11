#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 434.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Unique identifier of this device
// Each slave should have a different ID
const double SLAVE_ID = 1;

// ----------------------------------------------------------------------------------------------------------------------
//
// Function to initialise the LoRa communication device's pin connections, frequency and transmission power
//
// Input arguments: None
//
// Return type: Bool
//
// Required header files: None.
//
// Warning(s): Hardware connections with the Arduino must agree with the pin number definitions above.
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


double mockTemperature() //pretend function only for testing
{
  return(random(10, 30));
}


// ------------------------------------------------------------------------------
//
// Function to send height, pressure and temperature to the master device
//
// Input arguments: double representing the recorded position
//                  double representing the recorded pressure
//
// Return type: None.
//
// Required header files: RH_RF95.h
//
// Warning(s): 
//
// ------------------------------------------------------------------------------
void sendDataToMaster(double myposition, double pressureVal, double temperatureVal)
{        
  digitalWrite(RFM95_CS, LOW);
    
  //transform doubles to chars
  char dataPacket[30] = {0};
  dataPacket[0] = "$";

  char slaveID[2];
  char height[8];
  char pressure[8];
  char temp[8];

  char comma = ','; 

  //convert data into a string and append onto the string dataPacket
  //seperate data with commas.
  strcat(dataPacket, dtostrf(SLAVE_ID, 1, 0, slaveID));
  strncat(dataPacket, &comma, 1);


  strcat(dataPacket, dtostrf(myposition, 1, 2, height));
  strncat(dataPacket, &comma, 1);

  strcat(dataPacket, dtostrf(pressureVal,1, 2, pressure));
  strncat(dataPacket, &comma, 1);

  strcat(dataPacket, dtostrf(temperatureVal, 1, 2, temp));
  strncat(dataPacket, &comma, 1);

  rf95.send((uint8_t *)dataPacket, 30);
  rf95.waitPacketSent();

  // -----  to be removed after testing  -----  
  Serial.println("Sending data to you now...");
  Serial.println((char *)dataPacket);
  // -----------------------------------------      

}


void confirmConnection()
{
  digitalWrite(RFM95_CS, LOW);

  Serial.println("Test passed.");
  //connection for the test from transmitter, 'isConnectionGood()' function
  char radiopacket[20] = "Test Passed.";

  rf95.send((uint8_t *)radiopacket, 20);
  rf95.waitPacketSent();

}
