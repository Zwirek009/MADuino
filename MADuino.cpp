/// Project: MADuino
/// Author: Maciej Wiraszka
///
/// MADuino - Arduino library for creating multi-agent systems based on Arduino boards.
///
/// File: MADuino.cpp - implementation file for class MADuino that represents a single agent 
///

#include "MADuino.h"

void MADuino::init(RF24 *rad, RF24Network *net)
{
	radio = rad;
	network = net;
	nxtConversationNr = 1;
	nxtMessageNr = 1;
}

MADuino::MADuino(RF24 *rad, RF24Network *net) 
{
	init(rad, net);
}

MADuino::MADuino(RF24 *rad, RF24Network *net, String agentId) 
{
	agentId.toCharArray(id, 6);
	randomId = false;
	init(rad, net);
}

void MADuino::agentSetup()
{
	Serial.begin(57600);

	// IMPORTANT: analog pin A0 should be unconnected in order to use it as random seed
	randomSeed(analogRead(0));
	if (randomId) createKey(id);
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

void MADuino::createKey(char *out)
{
	byte i;
	for(i=0; i < 5; i++)
		out[i] = random(33,127);
	out[i] = '\0';
}

