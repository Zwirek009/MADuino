/// Project: MADuino
/// Author: Maciej Wiraszka
/// Version: v0.02 02.05.17
///
/// MADuino - Arduino library for creating multi-agent systems based on Arduino boards.
///
/// File: Message.cpp - implementation file for class Message that represents a single message 
///

#include "Message.h"

Message::Message(MessageStruct *cont, RF24 *rad, const uint64_t pipeAddr)
	: pipeAddress(pipeAddr)
{
	contents = cont;
	radio = rad;

	//radio->setPayloadSize(300);
}

Message::Message(char *buffer)
{
	contents = new MessageStruct();

	JsonArray& root = jsonBuffer.parseArray(buffer);
	if (!root.success())
	{
  		Serial.println("parseArray() failed");
	}

	// retrive the values
	contents->performative = root[0];
	contents->sender = root[1];
	contents->content = root[2];
	//...
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
	array.add(contents->content);
	// ...

	char buffer[300];
	array.printTo(buffer, sizeof(buffer));
	Serial.println(buffer);

	radio->stopListening();
	Serial.print("Now sending...\t");
	boolean ok = radio->write(&buffer, sizeof(buffer));

	//bool msg = true;
	//bool ok = radio->write(&msg, sizeof(bool));

	radio->startListening();

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