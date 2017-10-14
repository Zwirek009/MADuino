#include <MADuino.h>
#include "printf.h"

MADuino slave(2, 0xF0F0F0F0D2LL, 0xF0F0F0F0E1LL);

void setup() {
  // put your setup code here, to run once:
  	Serial.begin(57600);
	printf_begin();

	slave.slaveSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  //master.runMaster();
  slave.runSlave();
}