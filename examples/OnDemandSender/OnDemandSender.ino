#include <MADuino.h>

RF24 radio(8,9);
RF24Network network(radio);

MADuino agent(&radio, &network);

int buttonPin = 2;
performative inform = INFORM;
char content[] = "Msg generated by OnDemandSender";

void setup() {
	agent.agentSetup();

	pinMode(buttonPin, INPUT_PULLUP); // button

	Serial.print("OnDemandSender agent started --> id: ");
	Serial.println(agent.id);
	Serial.println();
}

void loop() {
	agent.onLoopStart();

	if(digitalRead(buttonPin) == LOW)
	{
		// button pressed
		Serial.println("\nCreating and sending message...");
		agent.newConversationSetup();
		agent.createMessageToAll(inform, content);
		agent.deleteMessages();

		// wait until user unpress the button
		while (digitalRead(buttonPin) == LOW) {}
		delay(20);
	}
}
