#include <MADuino.h>

RF24 radio(8,9);
RF24Network network(radio);

MADuino proposeInitiator(&radio, &network, "INIT");

char 	participantID[] = "PART";
boolean cancelProtocol = true;

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
    boolean protocolSucces = false;
	Serial.print("||| New IP Cancel conv --> conversationId: ");
	Serial.println(proposeInitiator.sendConversationId);
	Serial.println();

	// PROPOSE
    proposeInitiator.createMessage(PROPOSE, "I can do something.", participantID);
    if (cancelProtocol)
    {
        proposeInitiator.startCounting(2000);
        while ( proposeInitiator.isNotExceededTime() )
        {
            if ( proposeInitiator.isResponseReceived() )
            {
                // reponse catched
                if(proposeInitiator.messageReceived->performative == ACCEPT_PROPOSAL ||
                   proposeInitiator.messageReceived->performative == REJECT_PROPOSAL)
                {
                    Serial.println("Received response too early.\n");
                }
                // otherwise not propper response
                proposeInitiator.deleteReceivedMessage();
                break;
            }
        }
        
        proposeInitiator.cancelProtocol("", participantID);

        proposeInitiator.startCounting(8000);
        while ( proposeInitiator.isNotExceededTime() )
        {
            if ( proposeInitiator.isResponseReceived() )
            {
                // reponse catched
                if(proposeInitiator.messageReceived->performative == INFORM &&
                   strcmp(proposeInitiator.messageReceived->content, "done") == 0)
                {
                    Serial.println("Cancel OK.\n");
                    protocolSucces = true;
                }
                else if (proposeInitiator.messageReceived->performative == FAILURE)
                {
                    Serial.println("Cancel NOK.\n");
                    protocolSucces = true;
                }
                // otherwise not propper response
                proposeInitiator.deleteReceivedMessage();
                break;
            }
        }
    }
    else
    {
        // classic fipa propose ip
        proposeInitiator.startCounting(10000);
        
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
    }

    if (protocolSucces)
    {
        Serial.println("||| Cancel conv OK\n");
    }
    else
    {
        Serial.println("||| Cancel conv NOK\n");
    }

    cancelProtocol = !(cancelProtocol);
	delay(5000);
}