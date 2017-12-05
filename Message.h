/// Project: MADuino
/// Author: Maciej Wiraszka
///
/// MADuino - Arduino library for creating multi-agent systems based on Arduino boards.
///
/// File: Message.cpp - heather file for class Message that represents a single message 
///

#ifndef MESSAGE_H
#define MESSAGE_H

#include "Arduino.h"
#include "Enums.h"

// JSON parser
#include <ArduinoJson.h>

// send device specified
#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>

// structure representing single Message, implements FIPA-ACL standard
struct MessageStruct
{
	unsigned int performative;
	char *sender;
	char *reciver;
	char *content;
	char *replyWith;	// message ID
	char *replyBy;	// not used
	char *inReplyTo;
	unsigned int language;
	unsigned int ontology;
	unsigned int protocol;
	char *conversationId;
};

// class represending a single message
class Message
{
public:
	MessageStruct *contents;		// holds "unJSONed" message 
	RF24Network *network;			// represents the radio module

	// send message constructors
	Message(MessageStruct *cont, RF24Network *net);	// standard

	// recceive message constructor
	static MessageStruct* parseToMessageStruct(char * buffer);

	// basic message destructor
	~Message();

	boolean createAndSendJSON();	// method that encapsulate MessageStruct data into a JSON
									// and sends it using radio on pipe with pipe_address
	
	// scheme: lH (8 4 2 1) + rH (8 4 2 1)
	// both arguments must be < than 16
	static byte boundToByte(byte lH, byte rH);
	static void extractBoundedByte(byte source, byte * lH, byte * rh);

private:
	StaticJsonBuffer<200> jsonBuffer;	// for creating JSON purposes
};

#endif