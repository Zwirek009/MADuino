/// Project: MADuino
/// Author: Maciej Wiraszka
///
/// MADuino - Arduino library for creating multi-agent systems based on Arduino boards.
///
/// File: MADuino.cpp - implementation file for class MADuino that represents a single agent 
///

#include "MADuino.h"

MADuino::MADuino(unsigned long agentId, RF24 *rad, RF24Network *net) 
{
	id = agentId; // to change into GUID ?
	radio = rad;
	network = net;
	nxtConversationNr = 1;
	nxtMessageNr = 1;
}

void MADuino::agentSetup()
{
	Serial.begin(57600);

	// IMPORTANT: analog pin A0 should be unconnected in order to use it as random seed
	randomSeed(analogRead(0));
	
	SPI.begin();
	radio->begin();
	network->begin(channel, node_id);
}

void MADuino::onLoopStart()
{
	network->update(); 
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
	Message *mess = new Message(messageToBeSent, network);
	mess->createAndSendJSON();
	delete mess;

	Serial.println(createRandomLong());
}

void MADuino::reply()
{
	messageToBeSent->inReplyTo = messageReceived->replyWith;
	
	sendMessage();
}

boolean MADuino::isMessageReceived()
{
	while( network->available() )
	{
		Serial.println("Message catched !\n");

		RF24NetworkHeader header;
		network->read(header, &buffer, sizeof(buffer));
		Serial.println(buffer);
		
		messageReceived = Message::parseToMessageStruct(buffer);

		return true;
	}

	return false;
}

long MADuino::createRandomLong()
{
	if (random(2)) return random(2147483647);
	else return -(random(2147483647));
}

