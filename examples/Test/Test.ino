#include <MADuino.h>
#include "printf.h"

MADuino agent(1, 1, 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL);

void setup() {
  // put your setup code here, to run once:
  	Serial.begin(57600);
	//Serial.print("Agent started --> role: ");
	//Serial.println("Master");
	printf_begin();
	agent.agentSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  agent.runMaster();
}
