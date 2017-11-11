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

// class representing a single agent
class MADuino
{
public:
	//unsigned long get_id();

		// methods that may be used in the future
	//unsigned long get_nxt_conversation_nr();
	//unsigned long get_nxt_message_nr();

	MADuino(unsigned long agentId, RF24 *rad, RF24Network *net);	// basic constructor

	~MADuino() {}	// basic destructor

	void agentSetup();
	void onLoopStart();	// must be called at each program loop start, for RF24Network purposes
	void createSingleMessage(char *performative, char *content);
	void sendMessage();	// create and send message, using Message library
	void reply();

	boolean isMessageReceived();

	unsigned long id;		// unique agent ID --> change into GUID ?

	char buffer[200];
	
	unsigned long nxtConversationNr;	// conversation and message IDs are created as
	unsigned long nxtMessageNr;			// id + nxt_conversation/message_nr

	MessageStruct *messageToBeSent;		// using Message library
	MessageStruct *messageReceived;		//

private:
	RF24 *radio;			// specify all radio actions for radio module
	RF24Network *network;		//

	const uint16_t node_id = 00;	// RF24Network node id, all MADuinos has the same as 
									// message-flow on RF24Network logic level 
									// is based on multicast
	const uint8_t channel = 90;	// RF24Network default
};

#endif