#include <MADuino.h>

MADuino master(1, 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL);

void setup() {
	master.agentSetup();

	Serial.println("LedOnOff --> Agent started --> role: Master");
	Serial.println();
}

void loop() {
	char request[] = "Request";
	char content[] = "Change led state";

	master.messageToBeSent = new MessageStruct();	
	master.messageToBeSent->performative = request;
	master.messageToBeSent->sender = master.id;	
	master.messageToBeSent->content = content;
	master.messageToBeSent->replyWith = (master.id + (master.nxtMessageNr++));
	master.messageToBeSent->conversationId = (master.id + (master.nxtConversationNr++));

	// send prepaired request message
	Serial.println("Sending request for changing led state...");
	master.sendMessage();

	delay(3000);
}
