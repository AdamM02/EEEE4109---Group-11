#define thermistor A2

double calculateTemperature()
{
  double V_ther= (analogRead(thermistor) * (3.3/ 662.0));
  return(-6.2961*V_ther*V_ther*V_ther + 35.282*V_ther*V_ther - 96.941*V_ther+ 117.09);
}
