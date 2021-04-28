#include "include.h"


uint8 Edison_DBG = 0;
int ps2_ok;
void setup() 
{
	pinMode(LED,OUTPUT);
	pinMode(BUZZER,OUTPUT);
	pinMode(KEY,INPUT_PULLUP);

	InitPWM();
	InitTimer2();
	InitUart1();
  	ps2_ok = InitPS2();
	InitFlash();
	InitMemory();
}

void loop() 
{
	TaskRun(ps2_ok);
}
