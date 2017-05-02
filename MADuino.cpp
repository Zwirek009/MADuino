/// Project: MADuino
/// Author: Maciej Wiraszka
/// Version: v0.02 01.05.17 
///
/// MADuino - Arduino library for creating multi-agent systems based on Arduino boards.
///
/// File: MADuino.cpp - implementation file for class MADuino that represents a single agent 
///

#include "MADuino.h"

MADuino::MADuino(role r, const uint64_t listenAddr, const uint64_t sendAddr)
{
	// prepaire LED for signalizing message send or receive
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

	id = now(); // actual time in milis since 1970
	agentRole = r;
	pipeListen = listenAddr;
	pipeSend = sendAddr;

	radio = new RF24(9,10);

	Serial.begin(57600);
  	printf_begin();
  	printf("Agent started --> role: %s\n\n", roleName[agentRole]);

  	radio.begin();

  	radio.openWritingPipe(pipeSend);
  	radio.openReadingPipe(pipeListen);

  	radio.startListening();
  	radio.printDetails();
}

void MADuino::runMaster()
{
	// create request for lightning message
	messageToBeSent = new MessageStruct();
	messageToBeSent->performative = "Request\0";
	messageToBeSent->sender = id;
	messageToBeSent->content = "Light up one diod\0";
	messageToBeSent->reply-with = (id+(nxtMessageNr++));
	messageToBeSent->conversation-id = (id+(nxtConversationNr++));

	// send prepaired request message
	digitalWrite(LED_BUILTIN, HIGH);
	printf("Sending request for lightning up...\n");
	sendMessage();
	digitalWrite(LED_BUILTIN, LOW);

	delay(3000);
}

void MADuino::runSlave()
{
	// TODO
}

boolean MADuino::sendMessage()
{
	Message mess = new Message(messageToBeSent, radio, pipeSend);
	mess.createAndSendJSON();
	delete mess;
}

