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

class MADuino
{
public:
	unsigned long get_id();

	// methods that may be used in the future
	unsigned long get_nxt_conversation_nr();
	unsigned long get_nxt_message_nr();
private:
	unsigned long id;	// unique agent ID --> compitation time footprint in seconds since 1 Jan 1970

	unsigned long nxt_conversation_nr = 1;	// conversation and message IDs are created as
	unsigned long nxt_message_nr = 1;		// id + nxt_conversation/message_nr
};

#endif