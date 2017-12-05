#include <MADuino.h>

RF24 radio(9,10);
RF24Network network(radio);

MADuino proposeParticipant(&radio, &network, "PART");

boolean acceptProposal = true;

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
      Serial.println("test");
		// check if it is a propper propose msg
		if( proposeParticipant.messageReceived->performative == PROPOSE &&
			proposeParticipant.messageReceived->content == "I can do something." );
		{
			if (acceptProposal)
			{
				Serial.println("OK, do it !!!\n");
				proposeParticipant.createReply((performative)ACCEPT_PROPOSAL, "OK, do it !!!");
			}
			else
			{
				Serial.println("DO NOT do it !!!\n");
				proposeParticipant.createReply((performative)REJECT_PROPOSAL, "DO NOT do it !!!");
			}
			proposeParticipant.sendMessage();
      acceptProposal = !(acceptProposal);
		}
		// otherwise not propper response
	}
	
}