/// Project: MADuino
/// Author: Maciej Wiraszka
/// Version: v0.02 02.05.17
///
/// MADuino - Arduino library for creating multi-agent systems based on Arduino boards.
///
/// File: Message.cpp - heather file for class Message that represents a single message 
///

#ifndef Message_h
#define Message_h

#include "Arduino.h"

// JSON parser
#include <ArduinoJson.h>

// send device specified
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <printf.h>

// structure representing single Message, implements FIPA-ACL standard
struct MessageStruct
{
	char performative[17];
	unsigned long sender;
	char content[100];
	unsigned long replyWith;	// message ID
	unsigned long replyBy;
	unsigned long inReplyTo;
	char language[10];
	char ontology[10];
	char protocol[10];
	unsigned long conversationId;
};

// class represending a single message
class Message
{
public:
	MessageStruct *contents;		// holds "unJSONed" message 
	RF24 *radio;					// represents the radio module
	const uint64_t pipeAddress;	// pipe where to send the message to

	// send message constructors
	Message(MessageStruct *cont, RF24 *rad, const uint64_t pipeAddr);	// standard

	// recceive message constructor
	Message(StaticJsonBuffer<300> jsonBuff);

	// basic message destructor
	~Message() {}

private:
	StaticJsonBuffer<300> jsonBuffer;	// for creating JSON purposes

	boolean createAndSendJSON();	// method that encapsulate MessageStruct data into a JSON
									// and sends it using radio on pipe with pipe_address
};

#endif