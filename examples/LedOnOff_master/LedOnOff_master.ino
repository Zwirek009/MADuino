#include <MADuino.h>

RF24 radio(9,10);
RF24Network network(radio);

MADuino master(1, &radio, &network);

void setup() {
	master.agentSetup();

	Serial.println("LedOnOff --> Agent started --> role: Master");
	Serial.println();
}

void loop() {
	master.onLoopStart();

	char request[] = "Request";
	char content[] = "Change led state 123456781234567812345678123456789 end";

	master.createSingleMessage(request, content);

	// send prepaired request message
	Serial.println("Sending request for changing led state...");
	master.sendMessage();

	delay(3000);
}
