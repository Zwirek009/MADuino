/// Project: MADuino
/// Author: Maciej Wiraszka
/// Version: v0.02 01.05.17
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
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

struct MessageStruct
{
	char performative[17];
	unsigned long sender;
	char content[100];
	unsigned long reply-with;	// message ID
	unsigned long reply-by;
	unsigned long in-reply-to;
	char language[10];
	char ontology[10];
	char protocol[10];
	unsigned long conversation-id;
};

class Message
{
public:
	MessageStruct *contents;
	RF24 *radio;
	const uint64_t pipe_address;

	// send message constructors
	Message(MessageStruct *cont, RF24 *rad, const uint64_t pipe_addr);	// standard

	// recceive message constructor
	Message(StaticJsonBuffer<300> json_buff);

	~Message();

private:
	StaticJsonBuffer<300> json_buffer;

	boolean create__and_send_JSON();
};

#endif