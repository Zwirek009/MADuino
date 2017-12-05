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

boolean Message::createAndSendJSON(MessageStruct *cont, RF24Network *net, char buf[])
{
	StaticJsonBuffer<200> tempJsonBuffer;
	JsonArray& array = tempJsonBuffer.createArray();

	array.add(cont->performative);
	array.add(cont->sender);
	array.add(cont->reciver);
	array.add(cont->content);
	array.add(cont->replyWith);
	array.add(cont->replyBy);
	array.add(cont->inReplyTo);
	array.add(cont->language);
	array.add(cont->ontology);
	array.add(cont->protocol);
	array.add(cont->conversationId);

	array.printTo(buf, sizeof(buf));
	Serial.println(sizeof(buf));
	Serial.println(buf);
	Serial.print("Now sending...\t");
	RF24NetworkHeader header(00);
	Serial.println(strlen(buf)+1);
	boolean ok = net->multicast(header, &buf, strlen(buf)+1, 0);

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