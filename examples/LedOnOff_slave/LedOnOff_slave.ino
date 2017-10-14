#include <MADuino.h>

MADuino slave(2, 0xF0F0F0F0D2LL, 0xF0F0F0F0E1LL);

bool slaveLedState = false;

void setup() {
  slave.agentSetup();

	// prepaire LED for signalizing message send or receive
  pinMode(7, OUTPUT);
  
  Serial.println("Agent started --> role: Slave");
  Serial.println();
}

void loop() {
  if ( slave.radio->available() )
    {
    	Serial.println("Cos zlapalem");
    	Serial.println();

      	bool done = false;

      	//bool msg;

      	while (!done)
      	{
        	// Fetch the payload, and see if this was the last one.
        	//done = slave.radio->read(&msg, sizeof(bool));
      		done = slave.radio->read(&(slave.buffer), sizeof(slave.buffer));
          delay(20);
      	}

      	Message *mess = new Message(slave.buffer);
      	Serial.println(mess->contents->performative);
      	Serial.println(mess->contents->sender);
        Serial.println(mess->contents->content);
        Serial.println(slave.buffer);
      	Serial.println();

      	if(mess->contents->sender != 0)
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

      	delete mess;
  	}
}