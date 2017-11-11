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

// JSON parser
#include <ArduinoJson.h>

// send device specified
#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>

// structure representing single Message, implements FIPA-ACL standard
struct MessageStruct
{
	char *performative;
	unsigned long sender; // zamien na np MAC i prefix
	char *content;
	unsigned long replyWith;	// message ID
	unsigned long replyBy;		// zamien te trzy na stringi
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
	RF24Network *network;			// represents the radio module

	// send message constructors
	Message(MessageStruct *cont, RF24Network *net);	// standard

	// recceive message constructor
	Message(char * buffer);
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