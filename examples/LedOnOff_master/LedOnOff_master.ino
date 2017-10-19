#include <MADuino.h>

MADuino master(1, 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL);

void setup() {
	master.agentSetup();

	Serial.println("LedOnOff --> Agent started --> role: Master");
	Serial.println();
}

void loop() {
	char request[] = "Request";
	char content[] = "Change led state";

	master.createSingleMessage(request, content);

	// send prepaired request message
	Serial.println("Sending request for changing led state...");
	master.sendMessage();

	delay(3000);
}
