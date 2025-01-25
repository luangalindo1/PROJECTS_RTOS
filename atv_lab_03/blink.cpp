// see blink.oil head of file for information about the compilation process.

#include "tpl_os.h"
#include "Arduino.h"
#include "board.h"

volatile unsigned int period = 2000;

void setup()
{
	Serial.begin(9600); // baud do setup do cpp 57600
	pinMode(ECU3_DOUT1, OUTPUT); //COM22
}

TASK(periodicTask_T1)
{
	static unsigned int nb = 0;
	nb++;
	if(nb & 1){
		digitalWrite(ECU3_DOUT1, HIGH); // odd
		period *=2;
		Serial.print("Faster:");
		Serial.println(period);
		CancelAlarm(periodicAl1);
		SetRelAlarm(periodicAl1, period, period);
	}
	else {
		digitalWrite(ECU3_DOUT1, LOW); //even
		period /=2;
		Serial.print("Slower:");
		Serial.println(period);
		CancelAlarm(periodicAl1);
		SetRelAlarm(periodicAl1, period, period);
	}
	
	Serial.print("Luan Galindo, ");
	Serial.print(nb);
	Serial.println(" execucoes"); // Terminar com ln (arduino meio
	TerminateTask();              // que espera outra entrada cc)
}



