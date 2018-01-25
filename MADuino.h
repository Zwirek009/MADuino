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

#include <MemoryUsage.h>

struct MessageStruct
{
	unsigned int performative;
	char *sender;
	char *receiver;
	char *replyTo;
	char *content;
	unsigned int language;
	unsigned int encoding;
	unsigned int ontology;
	unsigned int protocol;
	char *conversationId;
	char *replyWith;
	char *inReplyTo;
	char *replyBy;
};

// class representing a single agent
class MADuino
{
public:

	char id[6];
	char sendMessageId[6];
	char sendConversationId[6];

	char receiveId[6];
	char receiveMessageId[6];
	char receiveConversationId[6];

	protocol protocol = NO_PROTOCOL;
	ontology ontology = NO_ONTOLOGY;
	encoding encoding = NOT_DEFINED;
	language language = USER_DEFINED;

	MessageStruct *messageToBeSent = nullptr;
	MessageStruct *messageReceived = nullptr;

	MADuino(RF24 *rad, RF24Network *net);	// constructor with generating random ID
	MADuino(RF24 *rad, RF24Network *net, String agentId);	// constructor with custom ID
	~MADuino() {}	// basic destructor

	void  agentSetup();				// basic MADuino setup methods
	void  newConversationSetup();	// 
	char* createId(char *out);		// 
	void  onLoopStart();			// 

	// sending messages
	void  createMessage(performative performative, char *content, char *receiver);
	void  createMessageToAll(performative performative, char *content);
	void  createReply(performative performative, char *content);
	void  createReplyToAll(performative performative, char *content);
	void  createNotUnderstoodReply();
	
	void  deleteSentMessage();		// deleting messages
	void  deleteReceivedMessage();	//
	void  deleteMessages();			//

	boolean isMessageReceived();	// receiving message methods
	boolean isResponseReceived();	//

	void storeSentCommunicativeAct();		// saving most important message fields
	void storeReceivedCommunicativeAct();	//

	void startCounting(unsigned long numOfMilis);	// time management methods
	boolean isNotExceededTime();					//
	unsigned long getElapsedTime();					//

	void cancelProtocol(char * content, char *receiver); // FIPA Cancel Meta-Protocol

private:
	RF24 *radio;			// specify all radio actions for radio module
	RF24Network *network;	//

	const uint16_t node_id = 00;	// RF24Network node id, all MADuinos has the same as 
									// message-flow on RF24Network logic level 
									// is based on multicast.
	const uint8_t channel = 90;	// RF24Network default.
	bool randomId = true;

	// measuring time connected variables and methods
	unsigned long numberOfMilisToWait;
	unsigned long startCountingTimespan = 0;

	char tempMessageId[6]; // for cancelProtocol usage

	char empty = '\0';			// default values
	char all[2] = {'*','\0'};	//
	char buffer[140];	// for parsing messages

	void basicMessageFill(performative performative, char *content, boolean newMsgId = true);
	void init(RF24 *rad, RF24Network *net);
	void sendMessage();
	MessageStruct* parseToMessageStruct();
	boolean createAndSendJSON();

};

#endif