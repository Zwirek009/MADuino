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
	if (randomId) createId(id);
	SPI.begin();
	radio->begin();
	network->begin(channel, node_id);
}

void MADuino::onLoopStart()
{
	network->update(); 
}

void MADuino::newConversationSetup()
{
	createId(sendConversationId);
}

void MADuino::basicMessageFill(performative performative, char *content)
{
	messageToBeSent = new MessageStruct();	
	messageToBeSent->performative = (unsigned int)performative;
	messageToBeSent->content = content;
	messageToBeSent->sender = id;
	messageToBeSent->language = language;
	messageToBeSent->ontology = ontology;
	messageToBeSent->protocol = protocol;
	messageToBeSent->replyWith = createId(sendMessageId);
	messageToBeSent->replyBy = &empty;
}

void MADuino::createMessage(performative performative, char * content, char *reciver)
{
	basicMessageFill(performative, content);
	messageToBeSent->reciver = reciver;
	messageToBeSent->inReplyTo = &empty;
	messageToBeSent->conversationId = sendConversationId;
}

void MADuino::createMessageToAll(performative performative, char * content)
{
	basicMessageFill(performative, content);
	messageToBeSent->reciver = all;
	messageToBeSent->inReplyTo = &empty;
	messageToBeSent->conversationId = sendConversationId;
}

void MADuino::createReply(performative performative, char * content)
{
	basicMessageFill(performative, content);
	messageToBeSent->reciver = messageReceived->sender;
	messageToBeSent->inReplyTo = messageReceived->replyWith;
	messageToBeSent->conversationId = messageReceived->conversationId;
}

void MADuino::createReplyToAll(performative performative, char * content)
{
	basicMessageFill(performative, content);
	messageToBeSent->reciver = all;
	messageToBeSent->inReplyTo = messageReceived->replyWith;
	messageToBeSent->conversationId = messageReceived->conversationId;
}

void MADuino::sendMessage()
{
	Message *mess = new Message(messageToBeSent, network);
	mess->createAndSendJSON();
	delete mess;
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

		if(	strcmp(messageReceived->reciver, all) == 0 ||
			strcmp(messageReceived->reciver, id) == 0)
			return true;
	}

	return false;
}

char* MADuino::createId(char *out)
{
	byte i;
	for(i=0; i < 5; i++)
		out[i] = random(33,127);
	out[i] = '\0';
	return out;
}

