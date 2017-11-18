/// Project: MADuino
/// Author: Maciej Wiraszka
///
/// MADuino - Arduino library for creating multi-agent systems based on Arduino boards.
///
/// File: MADuino.h - heather file for class MADuino that represents a single agent 
///

#ifndef MADUINO_H
#define MADUINO_H

#include "Arduino.h"
#include "Message.h"

// send device specified
#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>

enum performative
{
	REQUEST,
	PROPOSE,
	REFUSE,
	AGREE
};

enum language
{
	MADUINO
};

enum ontology
{
	NO_ONTOLOGY
};

enum protocol
{
	REQUEST_INTERACTION_PROTOCOL
};

// class representing a single agent
class MADuino
{
public:
	void init(RF24 *rad, RF24Network *net);
	MADuino(RF24 *rad, RF24Network *net); 		// basic constructor with random agent's id
	MADuino(RF24 *rad, RF24Network *net, String agentId);	// constructor with own agent's
															// ID definition

	~MADuino() {}	// basic destructor

	void  agentSetup();
	char* createId(char *out);
	void  onLoopStart();	// must be called at each program loop start, for RF24Network purposes
	void  createSingleMessage(char *performative, char *content);
	void  sendMessage();	// create and send message, using Message library
	void  reply();

	boolean isMessageReceived();

	char id[6];		// unique agent ID --> change into GUID ?
	char sendMessageId[6];
	char sendConversationId[6];

	char receiveId[6];		// unique agent ID --> change into GUID ?
	char receiveMessageId[6];
	char receiveConversationId[6];
	char empty = '\0';

	char buffer[200];

	MessageStruct *messageToBeSent;		// using Message library
	MessageStruct *messageReceived;		//

private:
	RF24 *radio;			// specify all radio actions for radio module
	RF24Network *network;		//

	const uint16_t node_id = 00;	// RF24Network node id, all MADuinos has the same as 
									// message-flow on RF24Network logic level 
									// is based on multicast
	const uint8_t channel = 90;	// RF24Network default
	bool randomId = true;
};

#endif