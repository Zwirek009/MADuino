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

class Message
{
public:

private:
	// to be stored in Message JSON
	char performative[17];
	unsigned long sender;
	char content[100];
	unsigned long reply-with;	// message ID
	unsigned long reply-by;
	unsigned long in-reply-to;
	char language[10];
	char ontology[10];
	char protocol[10];
	unsigned long conversation-id
};

#endif