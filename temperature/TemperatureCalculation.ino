void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  
//Voltage = -0.0002Temp2 - 0.0265Temp + 2.4217;
float Temperature;
float a = -0.0002;
float b = 2.4217;
float c = -0.0265-VoltageIn;
float TemperaturePlus = (-b+sqrt(b^2-4*a*c))/(2*a);
float TemperatureMinus = (-b-sqrt(b^2-4*a*c))/(2*a);
if(TemperaturePlus>TemperatureMinus){
  Temperature = TemperaturePlus;
}
else if(TemperaturePlus<TemperatureMinus){
  Temperature = TemperatureMinus;
}

}
