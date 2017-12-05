#include <MADuino.h>

RF24 radio(9,10);
RF24Network network(radio);

MADuino master(&radio, &network);

void setup() {
	master.agentSetup();

	Serial.print("LedOnOff --> Agent started --> role: Master, id: ");
	Serial.println(master.id);
	Serial.println();
}

void loop() {
	master.onLoopStart();

	performative request = REQUEST;
	char content[] = "Change led state 123456781234567812345678123456789 end";

	master.newConversationSetup();
	master.createMessageToAll(request, content);

	// send prepaired request message
	Serial.println("Sending request for changing led state...");
	master.sendMessageAndForget();

	delay(3000);
}
