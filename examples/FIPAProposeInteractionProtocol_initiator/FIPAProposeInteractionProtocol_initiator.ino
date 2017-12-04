#include <MADuino.h>

RF24 radio(9,10);
RF24Network network(radio);

MADuino proposeInitiator(&radio, &network);

void setup() {
	proposeInitiator.agentSetup();

	Serial.print("FIPA Request Interaction Protocol example --> Agent started --> role: Initiator, id: ");
	Serial.println(proposeInitiator.id);
	Serial.println();
}

void loop() {
	proposeInitiator.onLoopStart();

	// TODO
	
	delay(3000);
}