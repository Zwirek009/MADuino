#include <MADuino.h>

RF24 radio(9,10);
RF24Network network(radio);

MADuino proposeInitiator(&radio, &network, "INIT");

char participantID[] = "PART";

void setup() {
	proposeInitiator.agentSetup();
	proposeInitiator.protocol = PROPOSE_INTERACTION_PROTOCOL;

	Serial.print("FIPA Request Interaction Protocol example --> Agent started --> role: Initiator, id: ");
	Serial.println(proposeInitiator.id);
	Serial.println();
}

void loop() {
	proposeInitiator.onLoopStart();

	// prepare new conversation
	proposeInitiator.newConversationSetup();
	Serial.print("Starting new propose IP conversation --> conversationId: ");
	Serial.println(proposeInitiator.sendConversationId);
	Serial.println();

	// PROPOSE
	proposeInitiator.createMessage((performative)PROPOSE, "I can do something.", participantID);
	proposeInitiator.sendMessage();
	
	delay(5000);
}