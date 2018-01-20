#include <MADuino.h>

RF24 radio(8,9);
RF24Network network(radio);

MADuino slave(&radio, &network);

bool slaveLedState = false;

void setup() {
  slave.agentSetup();

  pinMode(7, OUTPUT);	// prepare LED for signalizing message send or receive
  
	Serial.print("Agent started --> role: Slave, id: ");
	Serial.println(slave.id);
  Serial.println();
}

void loop() {
	slave.onLoopStart();

  if ( slave.isMessageReceived() )
    {
				Serial.print("performative: ");
				Serial.println(slave.messageReceived->performative);
				Serial.print("sender: ");
				Serial.println(slave.messageReceived->sender);
				Serial.print("receiver: ");
				Serial.println(slave.messageReceived->receiver);
				Serial.print("reply-to: ");
				Serial.println(slave.messageReceived->replyTo);
				Serial.print("content: ");
				Serial.println(slave.messageReceived->content);
				Serial.print("language: ");
				Serial.println(slave.messageReceived->language);
				Serial.print("encoding: ");
				Serial.println(slave.messageReceived->encoding);
				Serial.print("ontology: ");
				Serial.println(slave.messageReceived->ontology);
				Serial.print("protocol: ");
				Serial.println(slave.messageReceived->protocol);
				Serial.print("conversation-id: ");
				Serial.println(slave.messageReceived->conversationId);
				Serial.print("reply-with: ");
				Serial.println(slave.messageReceived->replyWith);
				Serial.print("in-reply-to: ");
				Serial.println(slave.messageReceived->inReplyTo);
				Serial.print("reply-by: ");
				Serial.println(slave.messageReceived->replyBy);
      	Serial.println();
				FREERAM_PRINT;
				Serial.println();

      	if(slave.messageReceived->sender != 0)
      	{
      		if (slaveLedState == true)
      		{
      			digitalWrite(7, LOW);
      			slaveLedState = false;
      		}
      		else
      		{
      			digitalWrite(7, HIGH);
      			slaveLedState = true;
      		}
		  	}
      	slave.deleteReceivedMessage();
  	}
}