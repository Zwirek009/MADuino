/// Project: MADuino
/// Author: Maciej Wiraszka
///
/// MADuino - Arduino library for creating multi-agent systems based on Arduino boards.
///
/// File: Message.cpp - implementation file for class Message that represents a single message 
///

#include "Message.h"

Message::Message(MessageStruct *cont, RF24Network *net)
{
	contents = cont;
	network = net;
}

Message::~Message()
{
	delete contents;
}

boolean Message::createAndSendJSON()
{
	JsonArray& array = jsonBuffer.createArray();

	array.add(contents->performative);
	array.add(contents->sender);
	array.add(contents->reciver);
	array.add(contents->content);
	array.add(contents->replyWith);
	array.add(contents->replyBy);
	array.add(contents->inReplyTo);
	array.add(contents->language);
	array.add(contents->ontology);
	array.add(contents->protocol);
	array.add(contents->conversationId);

	char buffer[200];
	array.printTo(buffer, sizeof(buffer));
	Serial.println(buffer);
	Serial.print("Now sending...\t");
	RF24NetworkHeader header(00);
	boolean ok = network->multicast(header, &buffer, strlen(buffer)+1, 0);

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

MessageStruct* Message::parseToMessageStruct(char * buffer)
{
	MessageStruct* messStruct = new MessageStruct();
	StaticJsonBuffer<200> tempJsonBuffer;

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

byte Message::boundToByte(byte lH, byte rH)
{
	return (B00001111 & rH) | (lH << 4);
}

void Message::extractBoundedByte(byte source, byte * lH, byte * rH)
{
	*rH = B00001111 & source;
	*lH = (B11110000 & source) >> 4;
}