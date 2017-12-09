#include <MADuino.h>

#define ID                      1
#define NUM_OF_AGENTS           2
#define NUM_OF_AVAILABLE_COLORS 4

enum color
{
    GREEN,
    RED,
    YELLOW,
    BLUE
};

RF24 radio(9,10);
RF24Network network(radio);

MADuino agent(&radio, &network, String(ID));

color agentView[NUM_OF_AGENTS];
color availableColors[NUM_OF_AVAILABLE_COLORS] = {GREEN, RED};
color currentColor;

void setup() 
{
    agent.agentSetup();

    Serial.print("### ABPAlgorithm --> id: ");
    Serial.print(agent.id);
    Serial.println(" ###");
    Serial.println();
}

void loop() 
{
    agent.onLoopStart();
}