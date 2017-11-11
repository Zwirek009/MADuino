#include <MADuino.h>

MADuino master(1);

void setup() {
	master.agentSetup();

	Serial.println("LedOnOff --> Agent started --> role: Master");
	Serial.println();
}

void loop() {
	master.onLoopStart();

	char request[] = "Request";
	char content[] = "Change led state";

	master.createSingleMessage(request, content);

	// send prepaired request message
	Serial.println("Sending request for changing led state...");
	master.sendMessage();

	delay(3000);
}
