#include <MADuino.h>

RF24 radio(9,10);
RF24Network network(radio);

MADuino noiseMaker(&radio, &network, "NOISE");

void setup() {
	noiseMaker.agentSetup();

	Serial.print("NoiseMaker --> id: ");
	Serial.println(noiseMaker.id);
	Serial.println();
}

void loop() {
	noiseMaker.onLoopStart();

	// prepare new conversation
	noiseMaker.newConversationSetup();
	Serial.print("New unknown conversation --> conversationId: ");
	Serial.println(noiseMaker.sendConversationId);
	Serial.println();

	// ---------------------- Send well formed msg with wrong content ----------------------
	noiseMaker.createMessageToAll(PROPOSE, "Bla bla bla...");
	noiseMaker.startCounting(10000);

	boolean anyResponseAcquaired = false;

	// catch response
	while ( noiseMaker.isNotExceededTime() )
	{
		if ( noiseMaker.isResponseReceived() )
		{
			// reponse catched
			if(noiseMaker.messageReceived->performative == NOT_UNDERSTOOD)
			{
				Serial.println("OK Response\n");
            }
            else
            {
                Serial.println("Non-standard Response\n");
            }
            anyResponseAcquaired = true;
            noiseMaker.deleteReceivedMessage();
        }
	}
    noiseMaker.deleteSentMessage();

	if (!anyResponseAcquaired)
	{
		Serial.println("No Responses\n");
	}
	// -------------------------------------------------------------------------------------
	delay(1000);
}