#include <MADuino.h>

RF24 radio(9,10);
RF24Network network(radio);

MADuino proposeParticipant(&radio, &network, "PART");

boolean cancelOK = true;

void setup() {
	proposeParticipant.agentSetup();

	Serial.print("FIPA Request IP --> role: ");
	Serial.println(proposeParticipant.id);
	Serial.println();
}

void loop() {
	proposeParticipant.onLoopStart();

	if ( proposeParticipant.isMessageReceived() )
    {
		// check if it is a propper propose msg
		if( proposeParticipant.messageReceived->performative == PROPOSE &&
			strcmp(proposeParticipant.messageReceived->content, "I can do something.") == 0 )
		{
			proposeParticipant.storeReceivedCommunicativeAct();

			boolean cancelReceived = false;

			proposeParticipant.startCounting(5000);
			while ( proposeParticipant.isNotExceededTime() )
			{
				if ( proposeParticipant.isMessageReceived() )
				{
					if (proposeParticipant.messageReceived->performative == CANCEL &&
						strcmp(proposeParticipant.messageReceived->inReplyTo, proposeParticipant.receiveMessageId) == 0 &&
						strcmp(proposeParticipant.messageReceived->sender, proposeParticipant.receiveId) == 0 &&
						strcmp(proposeParticipant.messageReceived->conversationId, proposeParticipant.receiveConversationId) == 0)
					{
						// cancel received
						if (cancelOK)
						{
							proposeParticipant.createReply(INFORM, "done");
						}
						else
						{
							proposeParticipant.createReply(FAILURE, "error");
						}
						cancelOK = !(cancelOK);
						cancelReceived = true;
					}
				}
			}

			if (!cancelReceived)
			{
				Serial.println("OK, do it !!!\n");
				proposeParticipant.createReply(ACCEPT_PROPOSAL, "OK, do it !!!");
			}
		}
		else
		{
			Serial.println("Msg not understood... Informing about it");
			proposeParticipant.createNotUnderstoodReply();
		}
		proposeParticipant.deleteMessages();
	}
	
}