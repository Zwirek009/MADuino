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
	network = new RF24Network(&radio);
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
	while( network->available() )
	{
		Serial.println("Message catched !\n");

		RF24NetworkHeader header;
		network.read(header,&payload,sizeof(buffer));
		Serial.println(buffer);
		
		messageReceived = Message::parseToMessageStruct(buffer);

		return true;
	}
	else
	{
		return false;
	}
}

