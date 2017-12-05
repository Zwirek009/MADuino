#include <MADuino.h>

RF24 radio(9,10);
RF24Network network(radio);

MADuino proposeInitiator(&radio, &network, "INIT");

char 			participantID[] = "PART";
unsigned long 	waitForResponseMillis = 10000;

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
	proposeInitiator.sendMessage();
	unsigned long startTime = millis();
	boolean protocolSucces = false;

	// catch response
	while ( (millis() - startTime) < waitForResponseMillis )
	{
		if ( proposeInitiator.isMessageReceived() &&
			 strcmp(proposeInitiator.messageReceived->conversationId, proposeInitiator.sendConversationId) == 0 &&
			 strcmp(proposeInitiator.messageReceived->inReplyTo, proposeInitiator.messageToBeSent->replyWith) == 0 )
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
			proposeInitiator.deleteSentMessage();
			break;
		}
		else
		{
			//Serial.println(proposeInitiator.messageToBeSent->conversationId);
			//Serial.println(proposeInitiator.messageToBeSent->replyWith);
		}
	}

	if (!protocolSucces)
	{
		// TODO #58
		Serial.println("Protocol NOK\n");
	}
	
	delay(5000);
}