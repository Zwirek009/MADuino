#include <MADuino.h>

RF24 radio(8,9);
RF24Network network(radio);

MADuino master(&radio, &network);

performative request = REQUEST;
char content[] = "Change led state 123456781234567812345678123456789 end";

void setup() {
	master.agentSetup();

	Serial.print("LedOnOff --> Agent started --> role: Master, id: ");
	Serial.println(master.id);
	Serial.println();
}

void loop() {
	master.onLoopStart();

	FREERAM_PRINT;
	Serial.println("\nCreating and sending request for changing led state...");
	master.newConversationSetup();
	master.createMessageToAll(request, content);
	master.deleteMessages();

	delay(3000); // wait 3 seconds and start loop() again
}












