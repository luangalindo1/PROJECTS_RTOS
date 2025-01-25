// see blink.oil head of file for information about the compilation process.

#include "tpl_os.h"
#include "Arduino.h"
#include "board.h"

void setup()
{
	Serial.begin(9600); // funcionou com esse
	pinMode(ECU3_DOUT1, OUTPUT); //COM22
}

//The TASK is activated by the alarm "periodicAl":
//* The alarm "periodicAl" is configured in the .oil file to activate task 
//"periodicTask" each 1000 pulses of counter SystemCounter.
//* The SystemCounter is the same as the systick used in Arduino, a tick each 1024us on a 16MHz arduino.
//* This task is periodic, with a period of 1024ms.

volatile unsigned int tot_exec = 0;

TASK(periodicTask_T1)
{
	static unsigned int nb = 0;
	nb++;
	if(nb & 1) digitalWrite(ECU3_DOUT1, HIGH); //odd
	else digitalWrite(ECU3_DOUT1, LOW);        //even

	Serial.print(nb);
	Serial.println("x Task1");
	tot_exec++;
	TerminateTask();
}

TASK(periodicTask_T2)
{
	static unsigned int nb = 0;
	nb++;
	if(nb & 1) digitalWrite(ECU3_DOUT1, HIGH); //odd
	else digitalWrite(ECU3_DOUT1, LOW);        //even

	Serial.print(nb);
	Serial.println("x Task2");
	tot_exec++;
	TerminateTask();
}

TASK(periodicTask_T3)
{
	static unsigned int nb = 0;
	nb++;
	if(nb & 1) digitalWrite(ECU3_DOUT1, HIGH); //odd
	else digitalWrite(ECU3_DOUT1, LOW);        //even

	Serial.print(nb);
	Serial.println("x Task3");
	tot_exec++;
	TerminateTask();
}

TASK(periodicTask_T4)
{
	static unsigned int nb = 0;
	nb++;
	if(nb & 1) digitalWrite(ECU3_DOUT1, HIGH); //odd
	else digitalWrite(ECU3_DOUT1, LOW);        //even

	Serial.print(nb);
	Serial.println("x Task4");
	tot_exec++;
	Serial.print(tot_exec);
	Serial.println("x exec. total");
	TerminateTask();
}

