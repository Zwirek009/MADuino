#include <MADuino.h>

RF24 radio(9,10);
RF24Network network(radio);

MADuino slave(&radio, &network);

bool slaveLedState = false;

void setup() {
  slave.agentSetup();

	// prepaire LED for signalizing message send or receive
  pinMode(7, OUTPUT);
  
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
				Serial.print("reciver: ");
				Serial.println(slave.messageReceived->reciver);
				Serial.print("content: ");
				Serial.print(slave.messageReceived->content);
				Serial.print("replyWith: ");
				Serial.println(slave.messageReceived->replyWith);
				Serial.print("inReplyTo: ");
				Serial.println(slave.messageReceived->inReplyTo);
				Serial.print("language: ");
				Serial.println(slave.messageReceived->language);
				Serial.print("ontology: ");
				Serial.println(slave.messageReceived->ontology);
				Serial.print("protocol: ");
				Serial.println(slave.messageReceived->protocol);
				Serial.print("conversationId: ");
				Serial.println(slave.messageReceived->conversationId);
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

      	delete slave.messageReceived;
  	}
}