#include <MADuino.h>

RF24 radio(9,10);
RF24Network network(radio);

MADuino proposeInitiator(&radio, &network, "INIT");

char 			participantID[] = "PART";
unsigned long 	waitForResponseMillis = 10000;

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
	unsigned long startTime = millis();
	boolean protocolSucces = false;

	// catch response
	while ( (millis() - startTime) < waitForResponseMillis )
	{
		if (proposeInitiator.isMessageReceived() &&
			proposeInitiator.messageReceived->inReplyTo == proposeInitiator.sendConversationId )
		{
			// reponse catched
			if(proposeInitiator.messageReceived->performative == (performative)ACCEPT_PROPOSAL)
			{
				Serial.println("OK, I am doing something.\n");
				protocolSucces = true;
			}
			else if (proposeInitiator.messageReceived->performative == (performative)REJECT_PROPOSAL)
			{
				Serial.println("OK, I will not do that.\n");
				protocolSucces = true;
			}
			// otherwise not propper response
			
			break;
		}
	}

	if (!protocolSucces)
	{
		// TODO #58
		Serial.println("Propper protocol response from participant not received\n");
	}
	
	delay(5000);
}