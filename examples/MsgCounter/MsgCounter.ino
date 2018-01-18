#include <MADuino.h>

RF24 radio(8,9);
RF24Network network(radio);

MADuino agent(&radio, &network);

int buttonPin = 2;
unsigned long numOfMsgReceived = 0;

void setup() {
	agent.agentSetup();

	pinMode(buttonPin, INPUT_PULLUP); // button

	Serial.print("MsgCounter agent started --> id: ");
	Serial.println(agent.id);
	Serial.println();
	Serial.println("Started counting...\n");
}

void loop() {
	if ( agent.isMessageReceived() )
	{
		numOfMsgReceived++;
	}

	if ( digitalRead(buttonPin) == LOW )
	{
		Serial.print("Total number of msg received:\t");
		Serial.println(numOfMsgReceived);

		// wait until user unpress the button
		while (digitalRead(buttonPin) == LOW) {}
		delay(20);
	}
}
