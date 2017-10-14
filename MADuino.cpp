/// Project: MADuino
/// Author: Maciej Wiraszka
///
/// MADuino - Arduino library for creating multi-agent systems based on Arduino boards.
///
/// File: MADuino.cpp - implementation file for class MADuino that represents a single agent 
///

#include "MADuino.h"

MADuino::MADuino(unsigned long agentId, const uint64_t listenAddr, const uint64_t sendAddr) 
	: pipeListen(listenAddr), pipeSend(sendAddr)
{

	id = agentId; // to change into GUID ?

	nxtConversationNr = 1;
	nxtMessageNr = 1;
	slaveLedState = false; 	// for testing purposes --> led on-off
}

void MADuino::masterSetup() // only for testing purposes --> led on-off
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

void MADuino::slaveSetup() // only for testing purposes --> led on-off
{
	radio = new RF24(9,10);

	// prepaire LED for signalizing message send or receive
	pinMode(7, OUTPUT);

	printf("Agent started --> role: Slave\n\n");

	radio->begin();

	radio->openWritingPipe(pipeSend);
  	radio->openReadingPipe(1, pipeListen);
  	radio->startListening();
  	radio->printDetails();
}
void MADuino::runMaster() // only for testing purposes --> led on-off
{
	// create request for lightning message
	messageToBeSent = new MessageStruct();
	char request[] = "Request";
	messageToBeSent->performative = request;
	messageToBeSent->sender = id;
	char content[] = "Light down";
	messageToBeSent->content = content;
	messageToBeSent->replyWith = (id+(nxtMessageNr++));
	messageToBeSent->conversationId = (id+(nxtConversationNr++));

	// send prepaired request message
	Serial.println("Sending request for lightning up...");
	sendMessage();

	delay(3000);
}

void MADuino::runSlave() // only for testing purposes --> led on-off
{
	if ( radio->available() )
    {
    	Serial.println("Cos zlapalem");
    	Serial.println();

      	bool done = false;

      	//bool msg;

      	while (!done)
      	{
        	// Fetch the payload, and see if this was the last one.
        	//done = radio->read(&msg, sizeof(bool));
      		done = radio->read(&buffer, sizeof(buffer));

			delay(20);
      	}

      	Message *mess = new Message(buffer);
      	Serial.println(mess->contents->performative);
      	Serial.println(mess->contents->sender);
      	Serial.println(mess->contents->content);
      	Serial.println();

      	if(mess->contents->sender != 0)
      	{
      		if (slaveLedState == true)
      		{
      			digitalWrite(7, LOW);
      			slaveLedState = false;
      		}
      		else
      		{
      			digitalWrite(7, HIGH);
      			slaveLedState = true;
      		}
		  }

      	delete mess;

      	//Serial.println(buffer);
  	}
}

boolean MADuino::sendMessage()
{
	Message *mess = new Message(messageToBeSent, radio, pipeSend);
	mess->createAndSendJSON();
	delete mess;
}

