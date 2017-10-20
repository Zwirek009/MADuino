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
}

void MADuino::agentSetup()
{
	Serial.begin(57600);
	radio = new RF24(9,10);	
	radio->begin();
	radio->openWritingPipe(pipeSend);
	radio->openReadingPipe(1, pipeListen);
	radio->startListening();
}

void MADuino::createSingleMessage(char * performative, char * content)
{
	messageToBeSent = new MessageStruct();	
	messageToBeSent->performative = performative;
	messageToBeSent->content = content;

	// complete single message struct
	messageToBeSent->sender = id;
	messageToBeSent->replyWith = nxtMessageNr++;
	messageToBeSent->conversationId = nxtConversationNr++;
}

void MADuino::sendMessage()
{
	Message *mess = new Message(messageToBeSent, radio, pipeSend);
	mess->createAndSendJSON();
	delete mess;
}

void MADuino::reply()
{
	messageToBeSent->inReplyTo = messageReceived->replyWith;
	
	sendMessage();
}

boolean MADuino::isMessageReceived()
{
	if( radio->available() )
	{
		Serial.println("Message catched !\n");

		bool readingDone = false;

		while (!readingDone)
		{
		  	// Fetch the payload, and see if this was the last one.
		  	readingDone = radio->read(&buffer, sizeof(buffer));
			delay(20);
		}

		Serial.println(buffer);
		
		messageReceived = Message::parseToMessageStruct(buffer);

		return true;
	}
	else
	{
		return false;
	}
}

