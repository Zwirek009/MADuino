#include <MADuino.h>
#include "printf.h"

MADuino master(1, 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
	printf_begin();

	master.masterSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  master.runMaster();
}
