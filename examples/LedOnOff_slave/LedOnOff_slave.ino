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
      	Serial.println(slave.messageReceived->performative);
      	Serial.println(slave.messageReceived->sender);
				Serial.println(slave.messageReceived->content);
				Serial.println(slave.messageReceived->replyWith);
      	Serial.println(slave.messageReceived->inReplyTo);
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