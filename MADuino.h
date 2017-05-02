/// Project: MADuino
/// Author: Maciej Wiraszka
/// Version: v0.02 02.05.17 
///
/// MADuino - Arduino library for creating multi-agent systems based on Arduino boards.
///
/// File: MADuino.h - heather file for class MADuino that represents a single agent 
///

#ifndef MADuino_h
#define MADuino_h

#include "Arduino.h"
#include "Message.h"
#include <Time.h>

// send device specified
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <printf.h>

// possible agent's roles
typedef enum { master = 1, slave } role;
const char *roleName[] = {"invalid", "Master", "Slave"};

// class representing a single agent
class MADuino
{
public:
	//unsigned long get_id();

		// methods that may be used in the future
	//unsigned long get_nxt_conversation_nr();
	//unsigned long get_nxt_message_nr();

	MADuino(role r, const uint64_t listenAddr, const uint64_t sendAddr);	// basic constructor

	~MADuino() {}	// basic destructor

	void runMaster();	// methods specifying specyfic role agents in Arduino loop()
	void runSlave();	//

	boolean sendMessage();	// create and send message, using Message library
private:
	role agent_role;
	unsigned long id;		// unique agent ID --> compilation time footprint in seconds since 1 Jan 1970

	unsigned long nxtConversationNr = 1;	// conversation and message IDs are created as
	unsigned long nxtMessageNr = 1;			// id + nxt_conversation/message_nr

	MessageStruct *messageToBeSent;		// using Message library
	MessageStruct *messageReceived;		//

	const uint64_t pipeListen;	// pipes (send and receive channels) addresses
	const uint64_t pipeSend;	//

	RF24 *radio;	// specify all radio actions for radio module
};

#endif