#include <MADuino.h>

RF24 radio(8,9);
RF24Network network(radio);

MADuino agent(&radio, &network);

void setup() {
	agent.agentSetup();
	
	// other setup ...
}

void loop() {
	agent.onLoopStart();

	// other loop logic to be executed periodically ...
}

// agent logic (methods) ...