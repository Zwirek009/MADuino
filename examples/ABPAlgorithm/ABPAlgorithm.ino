#include <MADuino.h>

#define ID                      1
#define NUM_OF_AGENTS           2
#define NUM_OF_AVAILABLE_COLORS 2

enum color
{
    GREEN,
    RED,
    YELLOW
};

RF24 radio(8,9);
RF24Network network(radio);

MADuino agent(&radio, &network, String(ID));

color agentView[NUM_OF_AGENTS];
color availableColors[NUM_OF_AVAILABLE_COLORS] = {GREEN, RED};
color currentColor;

void setup() 
{
    agent.agentSetup();
    declareOutputLeds();
    lightOnAvailableColors();

    Serial.print("### ABPAlgorithm --> id: ");
    Serial.print(agent.id);
    Serial.println(" ###");
    Serial.println();

}

void loop() 
{
    agent.onLoopStart();
}

void declareOutputLeds()
{
    // declare connected Leds
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
}

void lightOnAvailableColors()
{

}