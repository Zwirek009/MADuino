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
#include "Enums.h"

// JSON parser
#include <ArduinoJson.h>

// send device specified
#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>

// structure representing single Message, implements FIPA-ACL standard
struct MessageStruct
{
	unsigned int performative;
	char *sender;
	char *reciver;
	char *content;
	char *replyWith;	// message ID
	char *replyBy;	// not used
	char *inReplyTo;
	unsigned int language;
	unsigned int ontology;
	unsigned int protocol;
	char *conversationId;
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
	void  newConversationSetup();
	char* createId(char *out);
	void  onLoopStart();	// must be called at each program loop start, for RF24Network purposes
	void  createMessage(performative performative, char *content, char *reciver);
	void  createMessageToAll(performative performative, char *content);
	void  createReply(performative performative, char *content);
	void  createReplyToAll(performative performative, char *content);
	void  sendMessage();
	void  sendMessageAndForget();
	void  deleteSentMessage();
	void  deleteReceivedMessage();

	boolean isMessageReceived();


	MessageStruct* parseToMessageStruct();
	boolean createAndSendJSON();	// method that encapsulate MessageStruct data into a JSON
	// and sends it using radio on pipe with pipe_address

	char id[6];		// unique agent ID --> change into GUID ?
	char sendMessageId[6];
	char sendConversationId[6];

	char receiveId[6];		// unique agent ID --> change into GUID ?
	char receiveMessageId[6];
	char receiveConversationId[6];
	char empty = '\0';
	char all[2] = {'*','\0'};

	protocol protocol = NO_PROTOCOL;
	ontology ontology = NO_ONTOLOGY;
	language language = MADUINO_MINIMAL;

	char buffer[200];

	MessageStruct *messageToBeSent = nullptr;		// using Message library
	MessageStruct *messageReceived = nullptr;		//

	// scheme: lH (8 4 2 1) + rH (8 4 2 1)
	// both arguments must be < than 16
	static byte boundToByte(byte lH, byte rH);
	static void extractBoundedByte(byte source, byte * lH, byte * rh);

private:
	RF24 *radio;			// specify all radio actions for radio module
	RF24Network *network;		//

	const uint16_t node_id = 00;	// RF24Network node id, all MADuinos has the same as 
									// message-flow on RF24Network logic level 
									// is based on multicast
	const uint8_t channel = 90;	// RF24Network default
	bool randomId = true;

	void basicMessageFill(performative performative, char *content);
};

#endif