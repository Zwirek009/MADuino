/// Project: MADuino
/// Author: Maciej Wiraszka
/// Version: v0.02 01.05.17 
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
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// possible agent's roles
typedef enum { master = 1, slave } role;
const char *role_name[] = {"invalid", "Master", "Slave"};

// class representing a single agent
class MADuino
{
public:
	//unsigned long get_id();

		// methods that may be used in the future
	//unsigned long get_nxt_conversation_nr();
	//unsigned long get_nxt_message_nr();

	MADuino(role r, const uint64_t listen_addr, const uint64_t send_addr);	// basic constructor

	~MADuino() {}	// basic destructor

	void run_master();	// methods specifying specyfic role agents in Arduino loop()
	void run_slave();	//

	boolean send_message();	// create and send message, using Message library
private:
	role agent_role;
	unsigned long id;		// unique agent ID --> compilation time footprint in seconds since 1 Jan 1970

	unsigned long nxt_conversation_nr = 1;	// conversation and message IDs are created as
	unsigned long nxt_message_nr = 1;		// id + nxt_conversation/message_nr

	MessageStruct *message_to_be_sent;		// using Message library
	MessageStruct *message_received;		//

	const uint64_t pipe_listen;	// pipes (send and receive channels) addresses
	const uint64_t pipe_send;	//

	RF24 *radio;	// specify all radio actions for radio module
};

#endif