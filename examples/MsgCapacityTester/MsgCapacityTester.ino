#include <MADuino.h>

RF24 radio(8,9);
RF24Network network(radio);

MADuino agent(&radio, &network);

int buttonPin = 2;
unsigned long numOfMsgSent = 0;
unsigned long startTime;
unsigned long endTime;
unsigned long testTime;

performative inform = INFORM;
char content[] = "Capacity test";

void setup() {
	agent.agentSetup();

	pinMode(buttonPin, INPUT_PULLUP); // button
	agent.newConversationSetup();

	Serial.print("MsgCapacityTester agent started --> id: ");
	Serial.println(agent.id);
	Serial.println();
	Serial.println("Started sending...\n");
	startTime = millis();
}

void loop() {
	agent.onLoopStart();

	agent.createMessageToAll(inform, content);
	agent.deleteSentMessage();
	numOfMsgSent++;

	if ( digitalRead(buttonPin) == LOW )
	{
		endTime = millis();
		testTime = (endTime - startTime)/1000;

		Serial.println("Sending stopped\n");
		Serial.print("Number of msg sent:\t");
		Serial.println(numOfMsgSent);
		Serial.print("Testing time:\t");
		Serial.print(testTime);
		Serial.println(" s");
		Serial.print("Capacity:\t");
		Serial.print((unsigned long)(numOfMsgSent/testTime));
		Serial.println(" msg/s\n");
		Serial.println("Restart Arduino to test again...");

		while(true) {}	// wait for user to reset the Arduino
	}
}
