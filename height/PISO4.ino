// for Arduino Microcontrollers
int latchPin = 3;
int clockPin = 4;
int dataPin = 2;

int j;
int value;
byte switchVar = 0; // for pull down configuration
//byte switchVar = 1; // for pull up configuration

//-------------------------------------------------------------------------------------
void setup() {
  delay(5000);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, INPUT);
  pinMode(clockPin, OUTPUT);

  Serial.begin(9600);
}
//-------------------------------------------------------------------------------------
unsigned long LastCycle = 0;
unsigned long HrLastCycle = 0;
int InitPrevPos = 1;

//-------------------------------------------------------------------------------------
void loop() {
  unsigned long t = millis();
    byte x = 0;
    static int PrevPos;
    int CurPos;
    static int EncDiff;
    static int EncCount;
    static int HrPrevPos;
    static int HrPos;
    static int HrEncDiff;

  //int CurrentPos;

  if (InitPrevPos == 1) {
    
    PrevPos = GetPosition();
    HrPrevPos = PrevPos;
    EncCount = 0;
    InitPrevPos = 2;
  }

  if (t - LastCycle >= 4000) {
    EncCount = PositionFunction(&PrevPos, &EncDiff, EncCount);
    LastCycle = t;
    Serial.println();
    Serial.print("EncDiff:");
    Serial.print(EncDiff);
    Serial.println();
    Serial.print("EncCountvloop: ");
    Serial.print(EncCount);
  }

  if (t - HrLastCycle >= 20000) {
    HrPositionFunction(&HrPos, &HrPrevPos, &HrEncDiff, EncCount);
    HrLastCycle = t;
    EncCount = 0;
    Serial.println();
    Serial.print("HrEncDiff: ");
    Serial.print(HrEncDiff);

  }


}
//-------------------------------------------------------------------------------------
// Function to find the initial position of the array, returns the
int GetPosition() {

  int binarray[8];
  byte dataIn = 0;

  digitalWrite(latchPin, 0);
  digitalWrite(clockPin, 0);
  digitalWrite(clockPin, 1);
  digitalWrite(latchPin, 1);

  //printing binary word aswell
  for (j = 0; j < 8; j++) {
    value = digitalRead(dataPin);
    //place values into an array
    binarray[j] = value;


    if (value) {
      int a = (1 << j);
      dataIn = dataIn | a;
    }
    digitalWrite(clockPin, LOW);
    digitalWrite(clockPin, HIGH);
  }
  //  Serial.println("binarray: ");
  //  for (j = 0; j < 8; j++) {
  //    Serial.print(binarray[j]);
  //  }

  int DecNum = 0;
  for (int i = 0; i < 8; i++) {
    if (binarray[i] == 1) {
      DecNum = DecNum +  pow(2, (7 - i)) + 0.5;
    }
  }
  //  Serial.println("DecNum: ");
  //  Serial.println(DecNum);

  int CurrentPos;
  int FindVal = DecNum;
  int EncoderArray[128] = {127, 63, 62, 58, 56, 184, 152, 24, 8, 72, 73, 77, 79, 15, 47, 175,
                           191, 159, 31, 29, 28, 92, 76, 12, 4, 36, 164, 166, 167, 135, 151, 215, 223, 207, 143, 142,
                           14, 46, 38, 6, 2, 18, 82, 83, 211, 195, 203, 235, 239, 231, 199, 71, 7, 23, 19, 3, 1, 9, 41,
                           169, 233, 225, 229, 245, 247, 243, 227, 163, 131, 139, 137, 129, 128, 132, 148, 212, 244, 240,
                           242, 250, 251, 249, 241, 209, 193, 197, 196, 192, 64, 66, 74, 106, 122, 120, 121, 125, 253,
                           252, 248, 232, 224, 226, 98, 96, 32, 33, 37, 53, 61, 60, 188, 190, 254, 126, 124, 116, 112,
                           113, 49, 48, 16, 144, 146, 154, 158, 30, 94, 95
                          };

  for (int i = 0; i < 128; i++) {
    if (FindVal == EncoderArray[i]) {
      CurrentPos = i;
      Serial.println();
      Serial.println("First Pos Found");
      Serial.print("CurrentPos: ");
      Serial.print(CurrentPos);

      return CurrentPos;
    }
  }
}
//-----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------
int PositionFunction(int *PrevPos, int *EncDiff, int EncCount) {
  int CurrentPosition = GetPosition();
  int EncoderDiff;

      //clockwise over boundary
      if (CurrentPosition < 15 and *PrevPos > 112) {
        EncoderDiff = CurrentPosition - *PrevPos;
        EncoderDiff = 128 + EncoderDiff;
        EncCount = EncCount + 1;
        Serial.println();
        Serial.print("EncCount: ");
        Serial.print(EncCount);
      }
      //anti-clockwise over boundary
      else if (CurrentPosition > 112 and * PrevPos < 15) {
        EncoderDiff = CurrentPosition - *PrevPos;
        EncoderDiff = -(128 - EncoderDiff);
        EncCount = EncCount - 1;
        Serial.println();
        Serial.print("EncCount: ");
        Serial.print(EncCount);
      }
      else {
        EncoderDiff = CurrentPosition - *PrevPos;
      }
  *EncDiff = EncoderDiff ;
  *PrevPos = CurrentPosition;
  return EncCount;

    }

//-----------------------------------------------------------------------------------
int HrPositionFunction(int *HrPos, int *HrPrevPos, int *HrEncDiff, int EncCount) {
  *HrPos = GetPosition();
  int EncCounter = EncCount;
  int HrEncDiff1 = (*HrPos - *HrPrevPos) + (EncCounter * 128);
  Serial.println();
  Serial.print("EncCounter: ");
  Serial.print(EncCounter);
  Serial.println();
  Serial.print("HrPrevPos: ");
  Serial.print(*HrPrevPos);
  Serial.println();
  Serial.print("HrPos: ");
  Serial.print(*HrPos);
  Serial.println();
  Serial.print("HrEncDiff: ");
  Serial.print(HrEncDiff1);

  *HrEncDiff = HrEncDiff1;
  *HrPrevPos = *HrPos;
}
