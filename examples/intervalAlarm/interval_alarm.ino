//Interrupts for Battery management/saving using MCU power down mode. /INT from DS3231 is connected to INT0 of MCU.


#include <Wire.h>
#include "DS3231.h"

#define interruptPin 6

int ALARM_TRIGGERED=0;

DS3231 rtc; //Create the DS3231 object
static uint8_t prevSecond=0; 
const int interval_sec = 5; //interval in seconds


void updateAlarm (int interval_sec) {

  rtc.clearAlarm(); //resets the alarm interrupt status on the RTC
  attachInterrupt(digitalPinToInterrupt(interruptPin), EIC_ISR, FALLING);  // Attach interrupt to pin 6 with an ISR and when the pin state CHANGEs
  DateTime now = rtc.now(); //get the current time
  DateTime nextAlarm = DateTime(now.unixtime() + interval_sec);
  rtc.enableAlarm(nextAlarm); //Sets the alarm on the RTC to the specified time (using the DateTime Object passed in)
  delay(10); //wait for a moment for everything to complete
  ALARM_TRIGGERED=0;
}


void setup () 
{

  
	 Serial.begin(9600);
   Serial.println('beginning ...');
	 Wire.begin();
	
	 rtc.begin();
	  attachInterrupt(digitalPinToInterrupt(interruptPin), EIC_ISR, FALLING);  // Attach interrupt to pin 6 with an ISR and when the pin state CHANGEs
 
   updateAlarm(interval_sec);
  
   
}



void loop () 
{

	DateTime now = rtc.now(); //get the current date-time    
  
	if((now.second()) !=  prevSecond )
	{
	//print only when there is a change in seconds
	Serial.print(now.year(), DEC);
	Serial.print('/');
	Serial.print(now.month(), DEC);
	Serial.print('/');
	Serial.print(now.date(), DEC);
	Serial.print(' ');
	Serial.print(now.hour(), DEC);
	Serial.print(':');
	Serial.print(now.minute(), DEC);
	Serial.print(':');
	Serial.print(now.second(), DEC);
	Serial.println(' ');
	}
	prevSecond = now.second();

  delay(10);

  if (ALARM_TRIGGERED==1) {
   for (int i=0;i<3;i++) { // indicate visually
    Blink(LED_BUILTIN,100);
   }
   Serial.println(" External Interrupt detected "); //indicate on serial port
   
   updateAlarm(interval_sec);
   
  }

   delay(10);
} 

  
//Interrupt service routine for external interrupt on INT0 pin conntected to /INT
void EIC_ISR()
{
  //Keep this as short as possible. Possibly avoid using function calls
  detachInterrupt(digitalPinToInterrupt(interruptPin));
   ALARM_TRIGGERED=1;
  
}

void Blink(byte PIN, int DELAY_MS) {
  //Blink an LED
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN, LOW);
  delay(DELAY_MS);
}

