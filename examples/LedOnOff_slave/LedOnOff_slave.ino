#include <MADuino.h>

MADuino slave(2);

bool slaveLedState = false;

void setup() {
  slave.agentSetup();

	// prepaire LED for signalizing message send or receive
  pinMode(7, OUTPUT);
  
  Serial.println("Agent started --> role: Slave");
  Serial.println();
}

void loop() {
	slave.onLoopStart();
	
  if ( slave.isMessageReceived() )
    {
      	Serial.println(slave.messageReceived->performative);
      	Serial.println(slave.messageReceived->sender);
        Serial.println(slave.messageReceived->content);
        Serial.println(slave.buffer);
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