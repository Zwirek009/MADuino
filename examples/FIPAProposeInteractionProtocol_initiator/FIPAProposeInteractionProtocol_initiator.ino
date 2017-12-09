#include <MADuino.h>

RF24 radio(8,9);
RF24Network network(radio);

MADuino proposeInitiator(&radio, &network, "INIT");

char 			participantID[] = "PART";

void setup() {
	proposeInitiator.agentSetup();
	proposeInitiator.protocol = PROPOSE_INTERACTION_PROTOCOL;

	Serial.print("FIPA Request IP --> role: ");
	Serial.println(proposeInitiator.id);
	Serial.println();
}

void loop() {
	proposeInitiator.onLoopStart();

	// prepare new conversation
	proposeInitiator.newConversationSetup();
	Serial.print("New IP conversation --> conversationId: ");
	Serial.println(proposeInitiator.sendConversationId);
	Serial.println();

	// PROPOSE
	proposeInitiator.createMessage(PROPOSE, "I can do something.", participantID);
	proposeInitiator.startCounting(10000);

	boolean protocolSucces = false;

	// catch response
	while ( proposeInitiator.isNotExceededTime() )
	{
		if ( proposeInitiator.isResponseReceived() )
		{
			// reponse catched
			if(proposeInitiator.messageReceived->performative == ACCEPT_PROPOSAL)
			{
				Serial.println("OK, I am doing something.\n");
				protocolSucces = true;
			}
			else if (proposeInitiator.messageReceived->performative == REJECT_PROPOSAL)
			{
				Serial.println("OK, I will not do that.\n");
				protocolSucces = true;
			}
			// otherwise not propper response
			proposeInitiator.deleteMessages();
			break;
		}
	}

	if (!protocolSucces)
	{
		Serial.println("Protocol NOK\n");
	}
	
	delay(5000);
}