/// Project: MADuino
/// Author: Maciej Wiraszka
/// Version: v0.02 02.05.17 
///
/// MADuino - Arduino library for creating multi-agent systems based on Arduino boards.
///
/// File: MADuino.cpp - implementation file for class MADuino that represents a single agent 
///

#include "MADuino.h"

MADuino::MADuino(unsigned long agentId, int r, const uint64_t listenAddr, const uint64_t sendAddr) 
	: pipeListen(listenAddr), pipeSend(sendAddr)
{
	// prepaire LED for signalizing message send or receive
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

	id = agentId; // to change into actual time in milis since 1970
	agentRole = r;

}

void MADuino::masterSetup()
{
	radio = new RF24(9,10);

	//Serial.begin(57600);
  	Serial.println("Agent started --> role: Master");
  	Serial.println();

  	radio->begin();

  	radio->openWritingPipe(pipeSend);
  	radio->openReadingPipe(1, pipeListen);

  	radio->startListening();
  	radio->printDetails();
}

void MADuino::slaveSetup()
{
	radio = new RF24(9,10);

	printf("Agent started --> role: Slave\n\n");

	radio->begin();

	radio->openWritingPipe(pipeSend);
  	radio->openReadingPipe(1, pipeListen);
  	radio->startListening();
  	radio->printDetails();
}
void MADuino::runMaster()
{
	// create request for lightning message
	messageToBeSent = new MessageStruct();
	char request[] = "Request";
	messageToBeSent->performative = request;
	messageToBeSent->sender = id;
	char content[] = "Light up one diod";
	messageToBeSent->content = content;
	messageToBeSent->replyWith = (id+(nxtMessageNr++));
	messageToBeSent->conversationId = (id+(nxtConversationNr++));

	// send prepaired request message
	digitalWrite(LED_BUILTIN, HIGH);
	Serial.println("Sending request for lightning up...");
	sendMessage();
	digitalWrite(LED_BUILTIN, LOW);

	delete messageToBeSent;
	delay(3000);
}

void MADuino::runSlave()
{
	if ( radio->available() )
    {
    	Serial.println("Cos zlapalem");
      	bool done = false;

      	//bool msg;
		char buffer[300];

      	digitalWrite(LED_BUILTIN, HIGH);
      	while (!done)
      	{
        	// Fetch the payload, and see if this was the last one.
        	//done = radio->read(&msg, sizeof(bool));
      		done = radio->read(&buffer, sizeof(buffer));

			delay(20);
      	}
      	digitalWrite(LED_BUILTIN, LOW);
      	Serial.println(buffer);

      	Serial.println("Got payload...");
  	}
}

boolean MADuino::sendMessage()
{
	Message *mess = new Message(messageToBeSent, radio, pipeSend);
	mess->createAndSendJSON();
	delete mess;
}

