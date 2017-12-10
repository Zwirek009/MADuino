#include <MADuino.h>

#define ID                      1
#define NUM_OF_AGENTS           2
#define NUM_OF_AVAILABLE_COLORS 2

enum color
{
    GREEN = 5,
    RED = 6,
    YELLOW = 7
};

RF24 radio(8,9);
RF24Network network(radio);

MADuino agent(&radio, &network, String(ID));

color agentView[NUM_OF_AGENTS];
color availableColors[NUM_OF_AVAILABLE_COLORS] = {GREEN, RED};
color currentColor;

char contentBuffer[20];
char tempReciverID[2];

void setup() 
{
    agent.agentSetup();
    setupLeds();
    chooseInitColor();

    Serial.print("### ABPAlgorithm --> id: ");
    Serial.print(agent.id);
    Serial.println(" ###");
    Serial.println();

    if (ID == 1) 
        initMsg();
}

void loop() 
{
    agent.onLoopStart();
}

void refreshCurrentColor()
{
    for (int i = 5; i < 8; ++i)
        digitalWrite(i, LOW);
    
    digitalWrite(currentColor, HIGH);
}

void createOkQuestionContent()
{
    String temp = "ok?(" + String(ID) + ',' + String(currentColor) + ')';

  temp.toCharArray(contentBuffer, 9);
}

void setupLeds()
{
    // declare connected Leds
    for (int i = 2; i < 8; ++i)
        pinMode(i, OUTPUT);

    // light on available colors
    for (int i = 0; i < NUM_OF_AVAILABLE_COLORS; ++i)
        digitalWrite(availableColors[i] - 3, HIGH);
}

void chooseInitColor()
{
    // random color choose from availableColors
    currentColor = availableColors[(int)random(0,NUM_OF_AVAILABLE_COLORS)];
    refreshCurrentColor();
}

void initMsg()
{
    agent.newConversationSetup();
    createOkQuestionContent();

    for(int i = 2; i <= NUM_OF_AGENTS; ++i)
    {
        String(i).toCharArray(tempReciverID, 2);
        agent.createMessage(QUERY_IF, contentBuffer, tempReciverID);
    }
}