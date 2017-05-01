/// Project: MADuino
/// Author: Maciej Wiraszka
/// Version: v0.02 01.05.17 
///
/// MADuino - Arduino library for creating multi-agent systems based on Arduino boards.
///
/// File: MADuino.cpp - implementation file for class MADuino that represents a single agent 
///

#include "MADuino.h"

MADuino::MADuino(role r, const uint64_t listen_addr, const uint64_t send_addr)
{
	id = now(); // actual time in milis since 1970
	agent_role = r;
	pipe_listen = listen_addr;
	pipe_send = send_addr;

	radio = new RF24(9,10);

	Serial.begin(57600);
  	printf_begin();
  	printf("Agent started --> role: %s\n\n", role_name[agent_role]);

  	radio.begin();

  	radio.openWritingPipe(pipe_send);
  	radio.openReadingPipe(pipe_listen);

  	radio.startListening();
}

void MADuino::run_master()
{
	// TODO
}

void MADuino::run_slave()
{
	// TODO
}

boolean MADuino::send_message()
{
	// TODO
}

