#include <MADuino.h>

RF24 radio(9,10);
RF24Network network(radio);

MADuino proposeParticipant(&radio, &network, "PART");

char 

void setup() {
	proposeParticipant.agentSetup();

	Serial.print("FIPA Request Interaction Protocol example --> Agent started --> role: Initiator, id: ");
	Serial.println(proposeParticipant.id);
	Serial.println();
}

void loop() {
	proposeParticipant.onLoopStart();

	// TODO
	
	delay(3000);
}