
#define LED_PIN 7   //testing only

volatile int f_wdt=1;
int timerCount = 0;

int multipleOf8ToStopAt = 429;
// Important variable to determine how often data is recorded
// Watchdog timer is 8 seconds, therefore multiple x 8.3897 = timer time. Where 0.3897 is the recorded offset
// For example, if multipleOf8ToStopAt = 8
// timer time = 8.3897 * 8 = 64 seconds

void watchDogInit()
{
  /*** Setup the WDT ***/
  
  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  
  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  /* set new watchdog timeout prescaler value */
  WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */
  
  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
}

/***************************************************
 *  Name:        ISR(WDT_vect)
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Watchdog Interrupt Service. This
 *               is executed when watchdog timed out.
 *
 ***************************************************/
ISR(WDT_vect)
{
  if(f_wdt == 0)
  {
    f_wdt=1;
  }
  else
  {
    Serial.println("WDT Overrun!!!");
    watchDogInit();
  }
}

/***************************************************
 *  Name:        enterSleep
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Enters the arduino into sleep mode.
 *
 ***************************************************/
void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  sleep_enable();
  
  /* Now enter sleep mode. */
  sleep_mode();
  
  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  
  /* Re-enable the peripherals. */
  power_all_enable();
}

void checkWDT()
{
  if(f_wdt == 1)
  {
    timerCount++;
    f_wdt = 0;

    Serial.println(timerCount);
  }
  else
  {
      
  }
}

bool timeForData()
{
  if(f_wdt == 1)
  {
    timerCount++;
    f_wdt = 0;

    Serial.println(timerCount);
  }
  else
  {
      
  }
  
  if(timerCount == multipleOf8ToStopAt)
  {
    /* Toggle the LED */
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      
    /* Don't forget to clear the flag. */
    timerCount = 0;
      
    return true;
    
   }
   else
   {
      return false;
   }
}
