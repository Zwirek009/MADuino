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
	sendMessage();
}

void MADuino::createMessageToAll(performative performative, char * content)
{
	basicMessageFill(performative, content);
	messageToBeSent->reciver = all;
	messageToBeSent->inReplyTo = &empty;
	messageToBeSent->conversationId = sendConversationId;
	sendMessage();
}

void MADuino::createReply(performative performative, char * content)
{
	basicMessageFill(performative, content);
	messageToBeSent->reciver = messageReceived->sender;
	messageToBeSent->inReplyTo = messageReceived->replyWith;
	messageToBeSent->conversationId = messageReceived->conversationId;
	messageToBeSent->protocol = messageReceived->protocol;
	sendMessage();
}

void MADuino::createReplyToAll(performative performative, char * content)
{
	basicMessageFill(performative, content);
	messageToBeSent->reciver = all;
	messageToBeSent->inReplyTo = messageReceived->replyWith;
	messageToBeSent->conversationId = messageReceived->conversationId;
	messageToBeSent->protocol = messageReceived->protocol;
	sendMessage();
}

void MADuino::sendMessageAndForget()
{
	sendMessage();
	deleteMessages();
}

void MADuino::deleteSentMessage()
{
	delete messageToBeSent;
}

void MADuino::deleteReceivedMessage()
{
	delete messageReceived;
}

void MADuino::deleteMessages()
{
	deleteSentMessage();
	deleteReceivedMessage();
}

void MADuino::sendMessage()
{
	network->update(); 
	createAndSendJSON();
}

boolean MADuino::isMessageReceived()
{
	network->update(); 

	while( network->available() )
	{
		Serial.println("Message catched !\n");

		RF24NetworkHeader header;
		Serial.println(sizeof(buffer));
		network->read(header, &buffer, sizeof(buffer));
		Serial.println(buffer);
		
		messageReceived = parseToMessageStruct();

		if(	strcmp(messageReceived->reciver, all) == 0 ||
			strcmp(messageReceived->reciver, id) == 0)
			return true;
	}

	return false;
}

boolean MADuino::isResponseReceived()
{
	if ( isMessageReceived() &&
		 strcmp(messageReceived->conversationId, sendConversationId) == 0 &&
		 strcmp(messageReceived->inReplyTo, messageToBeSent->replyWith) == 0 )
	{
		return true;
	}
	else return false;

}

void MADuino::startCounting(unsigned long numOfMilis)
{
	startCountingTimespan = millis();
	numberOfMilisToWait = numOfMilis;
}

boolean MADuino::isNotExceededTime()
{
	if ((unsigned long)(millis() - startCountingTimespan) < numberOfMilisToWait)
		return true;
	else
		return false;
}

boolean MADuino::createAndSendJSON()
{
	StaticJsonBuffer<140> tempJsonBuffer;
	JsonArray& array = tempJsonBuffer.createArray();

	array.add(messageToBeSent->performative);
	array.add(messageToBeSent->sender);
	array.add(messageToBeSent->reciver);
	array.add(messageToBeSent->content);
	array.add(messageToBeSent->replyWith);
	array.add(messageToBeSent->replyBy);
	array.add(messageToBeSent->inReplyTo);
	array.add(messageToBeSent->language);
	array.add(messageToBeSent->ontology);
	array.add(messageToBeSent->protocol);
	array.add(messageToBeSent->conversationId);

	char tempBuffer[140];
	array.printTo(tempBuffer, sizeof(tempBuffer));
	Serial.println(sizeof(tempBuffer));
	Serial.println(tempBuffer);
	Serial.print("Now sending...\t");
	RF24NetworkHeader header(00);
	Serial.println(strlen(tempBuffer)+1);
	boolean ok = network->multicast(header, &tempBuffer, strlen(tempBuffer)+1, 0);

	if (ok)
	{
		Serial.println("Sent message.");
		Serial.println();
    	return true;
    }
    else
    {
    	Serial.println("Failed to send message.");
    	Serial.println();
    	return false;
    }
}

MessageStruct* MADuino::parseToMessageStruct()
{
	MessageStruct* messStruct = new MessageStruct();
	StaticJsonBuffer<140> tempJsonBuffer;

	JsonArray& root = tempJsonBuffer.parseArray(buffer);
	if (!root.success())
	{
		Serial.println("ERROR: Cannot parse given buffer to JSON !");
		delete messStruct;
		return NULL;
	}

	// retrive the values
	messStruct->performative = root[0];
	messStruct->sender = root[1];
	messStruct->reciver = root[2];
	messStruct->content = root[3];
	messStruct->replyWith = root[4];
	messStruct->replyBy = root[5];
	messStruct->inReplyTo = root[6];
	messStruct->language = root[7];
	messStruct->ontology = root[8];
	messStruct->protocol = root[9];
	messStruct->conversationId = root[10];

	return messStruct;
}

char* MADuino::createId(char *out)
{
	byte i;
	for(i=0; i < 5; i++)
		out[i] = random(33,127);
	out[i] = '\0';
	return out;
}

byte MADuino::boundToByte(byte lH, byte rH)
{
	return (B00001111 & rH) | (lH << 4);
}

void MADuino::extractBoundedByte(byte source, byte * lH, byte * rH)
{
	*rH = B00001111 & source;
	*lH = (B11110000 & source) >> 4;
}

