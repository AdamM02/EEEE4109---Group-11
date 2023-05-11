// rotary encoder
byte switchVar = 0; // for pull down configuration
//byte switchVar = 1; // for pull up configuration

// Hardware pin connections
const int latchPin = 7;
const int clockPin = 6;
const int dataPin = 8;

const double peatHeightOnEntry = 237;  //Update to actual value after testing
//in millimeters

// ------------------------------------------------------------------------------
//
// Function to get the decimal position of the rotary encoder
//
// Input arguments: None.
//
// Return type: Int representing the decimal position
//
// Required header files: None.
//
// Warning(s): 
//
// ------------------------------------------------------------------------------
int RotPrevPosCheck() 
{
  int binarray[8]={0};
  int dataIn(0);

  digitalWrite(latchPin, 0);
  digitalWrite(clockPin, 0);
  digitalWrite(clockPin, 1);
  digitalWrite(latchPin, 1);
  //reset

  int value(0);
  //printing binary word aswell
  for (int j = 0; j < 8; ++j)
  {
    value = digitalRead(dataPin);
    //place values into an array
    binarray[j] = value;

    dataIn = dataIn | (1 << j);

    digitalWrite(clockPin, LOW);
    digitalWrite(clockPin, HIGH);
  }

  int DecNum(0);
  for (int i = 0; i < 8; ++i) 
  {
    if (binarray[i] == 1) 
    {
      DecNum += pow(2, (7 - i)) + 0.5;
    }
  }

  const int EncoderArray[128] = {127, 63, 62, 58, 56, 184, 152, 24, 8, 72, 73, 77, 79, 15, 47, 175,
                                 191, 159, 31, 29, 28, 92, 76, 12, 4, 36, 164, 166, 167, 135, 151, 215, 223, 207, 143, 142,
                                 14, 46, 38, 6, 2, 18, 82, 83, 211, 195, 203, 235, 239, 231, 199, 71, 7, 23, 19, 3, 1, 9, 41,
                                 169, 233, 225, 229, 245, 247, 243, 227, 163, 131, 139, 137, 129, 128, 132, 148, 212, 244, 240,
                                 242, 250, 251, 249, 241, 209, 193, 197, 196, 192, 64, 66, 74, 106, 122, 120, 121, 125, 253,
                                 252, 248, 232, 224, 226, 98, 96, 32, 33, 37, 53, 61, 60, 188, 190, 254, 126, 124, 116, 112,
                                 113, 49, 48, 16, 144, 146, 154, 158, 30, 94, 95
                                 };

  digitalWrite(latchPin, 0);
  digitalWrite(clockPin, 0);

  for (int i = 0; i < 128; ++i) 
  {
    if (DecNum == EncoderArray[i]) 
    {
      return i;
    }
    else
    {

    }
  }
}

double convertToHeight(int rotaryEncoderValue)
{
  return (peatHeightOnEntry + ((rotaryEncoderValue - 64)*0.981));
  //0.981 is the resolution, change in height for an increment in the rotary encoder
}
