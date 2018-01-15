/// Project: MADuino
/// Author: Maciej Wiraszka
///
/// MADuino - Arduino library for creating multi-agent systems based on Arduino boards.
///
/// File: MADuino.cpp - implementation file for class MADuino that represents a single agent 
///

#include "MADuino.h"

void MADuino::init(RF24 *rad, RF24Network *net)
{
	radio = rad;
	network = net;
}

MADuino::MADuino(RF24 *rad, RF24Network *net) 
{
	init(rad, net);
}

MADuino::MADuino(RF24 *rad, RF24Network *net, String agentId) 
{
	agentId.toCharArray(id, 6);
	randomId = false;
	init(rad, net);
}

void MADuino::agentSetup()
{
	Serial.begin(57600);

	// IMPORTANT: analog pin A0 should be unconnected in order to use it as random seed
	randomSeed(analogRead(0));
	if (randomId) createId(id);
	SPI.begin();
	radio->begin();
	network->begin(channel, node_id);
}

void MADuino::onLoopStart()
{
	network->update(); 
}

void MADuino::newConversationSetup()
{
	createId(sendConversationId);
}

void MADuino::basicMessageFill(performative performative, char * content, boolean newMsgId = true)
{
	if (newMsgId) createId(sendMessageId);
	messageToBeSent = new MessageStruct();	
	messageToBeSent->performative = performative;
	messageToBeSent->sender = id;
	messageToBeSent->replyTo = &empty;
	messageToBeSent->content = content;
	messageToBeSent->language = language;
	messageToBeSent->encoding = encoding;
	messageToBeSent->ontology = ontology;
	messageToBeSent->protocol = protocol;
	messageToBeSent->replyWith = sendMessageId;
	messageToBeSent->replyBy = &empty;
	
}

void MADuino::createMessage(performative performative, char * content, char *reciver)
{
	basicMessageFill(performative, content);
	messageToBeSent->reciver = reciver;
	messageToBeSent->inReplyTo = &empty;
	messageToBeSent->conversationId = sendConversationId;
	sendMessage();
}

void MADuino::createMessageToAll(performative performative, char * content)
{
	basicMessageFill(performative, content);
	messageToBeSent->reciver = all;
	messageToBeSent->inReplyTo = &empty;
	messageToBeSent->conversationId = sendConversationId;
	sendMessage();
}

void MADuino::createReply(performative performative, char * content)
{
	basicMessageFill(performative, content);
	messageToBeSent->reciver = messageReceived->sender;
	messageToBeSent->inReplyTo = messageReceived->replyWith;
	messageToBeSent->conversationId = messageReceived->conversationId;
	messageToBeSent->protocol = messageReceived->protocol;
	sendMessage();
}

void MADuino::createReplyToAll(performative performative, char * content)
{
	basicMessageFill(performative, content);
	messageToBeSent->reciver = all;
	messageToBeSent->inReplyTo = messageReceived->replyWith;
	messageToBeSent->conversationId = messageReceived->conversationId;
	messageToBeSent->protocol = messageReceived->protocol;
	sendMessage();
}

void MADuino::createNotUnderstoodReply()
{
	basicMessageFill(NOT_UNDERSTOOD, messageReceived->content);
	messageToBeSent->reciver = messageReceived->sender;
	messageToBeSent->inReplyTo = messageReceived->replyWith;
	messageToBeSent->conversationId = messageReceived->conversationId;
	messageToBeSent->protocol = messageReceived->protocol;
	sendMessage();
}

void MADuino::cancelProtocol(char * content, char *reciver)
{
	basicMessageFill(CANCEL, content, false);
	messageToBeSent->reciver = reciver;
	messageToBeSent->inReplyTo = sendMessageId;
	messageToBeSent->replyWith = createId(tempMessageId);
	messageToBeSent->conversationId = sendConversationId;
	sendMessage();
}

void MADuino::sendMessageAndForget()
{
	sendMessage();
	deleteMessages();
}

void MADuino::deleteSentMessage()
{
	delete messageToBeSent;
	messageToBeSent = nullptr;
}

void MADuino::deleteReceivedMessage()
{
	delete messageReceived;
	messageReceived = nullptr;
}

void MADuino::deleteMessages()
{
	deleteSentMessage();
	deleteReceivedMessage();
}

void MADuino::sendMessage()
{
	network->update();
	createAndSendJSON();
}

boolean MADuino::isMessageReceived()
{
	network->update(); 

	while( network->available() )
	{
		Serial.println("Message catched !");

		RF24NetworkHeader header;
		network->read(header, &buffer, sizeof(buffer));
		Serial.println(buffer);
		
		messageReceived = parseToMessageStruct();

		if(	strcmp(messageReceived->reciver, all) == 0 ||
			strcmp(messageReceived->reciver, id) == 0)
			return true;
	}
	deleteReceivedMessage();
	return false;
}

boolean MADuino::isResponseReceived()
{
	if ( isMessageReceived() &&
		 strcmp(messageReceived->conversationId, sendConversationId) == 0 &&
		 strcmp(messageReceived->inReplyTo, messageToBeSent->replyWith) == 0 )
	{
		return true;
	}
	else 
	{
		deleteReceivedMessage();
		return false;
	}
}

void MADuino::storeSentCommunicativeAct()
{
	Serial.println("storeSentCommAct(): ");
	memcpy(sendMessageId, messageToBeSent->sender, strlen(messageToBeSent->sender));
	Serial.println(sendMessageId);
	memcpy(sendConversationId, messageToBeSent->conversationId, strlen(messageToBeSent->conversationId));
	Serial.println(sendConversationId);
	Serial.println();
}

void MADuino::storeReceivedCommunicativeAct()
{
	Serial.println("storeReceivedCommAct(): ");
	memcpy(receiveId, messageReceived->sender, strlen(messageReceived->sender));
	Serial.println(receiveId);
	memcpy(receiveMessageId, messageReceived->replyWith, strlen(messageReceived->replyWith));
	Serial.println(receiveMessageId);
	memcpy(receiveConversationId, messageReceived->conversationId, strlen(messageReceived->conversationId));
	Serial.println(receiveConversationId);
	Serial.println();
}

void MADuino::retreiveReceivedCommunicativeAct()
{
	Serial.println("retreiveReceivedCommAct(): ");
	messageReceived->sender = receiveId;
	Serial.println(messageReceived->sender);
	messageReceived->replyWith = receiveMessageId;
	Serial.println(messageReceived->replyWith);
	messageReceived->conversationId = receiveConversationId;
	Serial.println(messageReceived->conversationId);
	Serial.println();
}

void MADuino::startCounting(unsigned long numOfMilis)
{
	startCountingTimespan = millis();
	numberOfMilisToWait = numOfMilis;
}

boolean MADuino::isNotExceededTime()
{
	if (startCountingTimespan != 0 && (unsigned long)(millis() - startCountingTimespan) >= numberOfMilisToWait)
	{
		startCountingTimespan = 0;
		return false;
	}
	else
	{
		return true;
	}
}

unsigned long MADuino::getElapsedTime()
{
	if (startCountingTimespan != 0)
	{
		return (unsigned long)(millis() - startCountingTimespan);
	}
	else 
	{
		return 0;
	}
}

boolean MADuino::createAndSendJSON()
{
	StaticJsonBuffer<140> tempJsonBuffer;
	JsonArray& array = tempJsonBuffer.createArray();

	array.add(messageToBeSent->performative);
	array.add(messageToBeSent->sender);
	array.add(messageToBeSent->reciver);
	array.add(messageToBeSent->replyTo);
	array.add(messageToBeSent->content);
	array.add(messageToBeSent->language);
	array.add(messageToBeSent->encoding);
	array.add(messageToBeSent->ontology);
	array.add(messageToBeSent->protocol);
	array.add(messageToBeSent->conversationId);
	array.add(messageToBeSent->replyWith);
	array.add(messageToBeSent->inReplyTo);
	array.add(messageToBeSent->replyBy);

	char tempBuffer[140];
	RF24NetworkHeader header(00);
	array.printTo(tempBuffer, sizeof(tempBuffer));
	Serial.println(tempBuffer);
	Serial.print("Now sending\t");
	Serial.print(strlen(tempBuffer)+1);
	Serial.println("\tbytes...");
	boolean ok = network->multicast(header, &tempBuffer, strlen(tempBuffer)+1, 0);

	if (ok)
	{
		Serial.println("Sent message.");
		Serial.println();
    	return true;
    }
    else
    {
    	Serial.println("Failed to send message.");
    	Serial.println();
    	return false;
    }
}

MessageStruct* MADuino::parseToMessageStruct()
{
	MessageStruct* messStruct = new MessageStruct();
	StaticJsonBuffer<140> tempJsonBuffer;

	JsonArray& root = tempJsonBuffer.parseArray(buffer);
	if (!root.success())
	{
		Serial.println("ERROR: Cannot parse given buffer to JSON !");
		delete messStruct;
		deleteReceivedMessage();
		return nullptr;
	}
	else if (root.size() != 13)
	{
		Serial.println("ERROR: Received JSON is incomplete !");
		delete messStruct;
		deleteReceivedMessage();
		return nullptr;
	}

	// retrive the values
	messStruct->performative = root[0];
	messStruct->sender = root[1];
	messStruct->reciver = root[2];
	messStruct->replyTo = root[3];
	messStruct->content = root[4];
	messStruct->language = root[5];
	messStruct->encoding = root[6];
	messStruct->ontology = root[7];
	messStruct->protocol = root[8];
	messStruct->conversationId = root[9];
	messStruct->replyWith = root[10];
	messStruct->inReplyTo = root[11];
	messStruct->replyBy = root[12];
	
	return messStruct;
}

char* MADuino::createId(char *out)
{
	byte i;
	for(i=0; i < 5; i++)
		out[i] = random(33,127);
	out[i] = '\0';
	return out;
}

// DEPRECATED
//
// byte MADuino::boundToByte(byte lH, byte rH)
// {
// 	return (B00001111 & rH) | (lH << 4);
// }

// void MADuino::extractBoundedByte(byte source, byte * lH, byte * rH)
// {
// 	*rH = B00001111 & source;
// 	*lH = (B11110000 & source) >> 4;
// }

