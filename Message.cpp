/// Project: MADuino
/// Author: Maciej Wiraszka
/// Version: v0.02 01.05.17
///
/// MADuino - Arduino library for creating multi-agent systems based on Arduino boards.
///
/// File: Message.cpp - implementation file for class Message that represents a single message 
///

#include "Message.h"

Message::Message(MessageStruct *cont, RF24 *rad, const uint64_t pipeAddr)
{
	this.contents = cont;
	this.radio = rad;
	this.pipe_address = pipeAddr;
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

	radio->stopListening();

	printf("Now sending...\t");
	boolean ok = radio->write(&buffer, sizeof(buffer));

	radio->continueListening();

	if (ok)
	{
    	printf("Sent message.\n\r");
    	return true;
    }
    else
    {
    	printf("Failed to send message.\n\r");
    	return false;
    }
}