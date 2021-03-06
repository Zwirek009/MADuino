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

color agentView[NUM_OF_AGENTS+1];
color availableColors[NUM_OF_AVAILABLE_COLORS] = {GREEN, RED};
color currentColor = 0;

char contentBuffer[30];
char tempReciverID[2];

boolean terminate = false;

void setup() 
{
    agent.agentSetup();
    setupLeds();

    Serial.print("### ABPAlgorithm --> id: ");
    Serial.print(agent.id);
    Serial.println(" ###");
    Serial.println();

    if (ID == 1)
    { 
        chooseInitColor();
        createAndSendOkQuestion();
    }
}

void loop() 
{
    if (terminate != true) isABPMsgReceived();
}

void refreshCurrentColor()
{
    for (int i = 5; i < 8; ++i)
        digitalWrite(i, LOW);
    
    digitalWrite(currentColor, HIGH);
}

void createOkQuestionContent()
{
    String temp = "ok?,(" + String(ID) + ',' + String(currentColor) + ')';
    temp.toCharArray(contentBuffer, temp.length()+1);
}

void createNogoodContent()
{
    String temp = "nogood,{" + agentViewToString() + "}";
    temp.toCharArray(contentBuffer, temp.length()+1);
}

void createTerminateContent()
{
    String temp = "terminate";
    temp.toCharArray(contentBuffer, temp.length()+1);
}

String agentViewToString()
{
    String agentViewString = "";

    for (int i = 1; i <= NUM_OF_AGENTS; ++i)
    {
        if(agentView[i] != 0)
        {
            if (agentViewString != "")
                agentViewString += ',';
            agentViewString += '(' + String(i) + ',' + String(agentView[i]) + ')';
        }
    }

    return agentViewString;
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

void turnCurrentColorLedOff()
{
    digitalWrite(currentColor, LOW);
}

void chooseInitColor()
{
    // random color choose from availableColors
    currentColor = availableColors[(int)random(0,NUM_OF_AVAILABLE_COLORS)];
    refreshCurrentColor();
}

void createAndSendOkQuestion()
{
    agent.newConversationSetup();
    agent.onLoopStart();
    createOkQuestionContent();

    for(int i = 1; i <= NUM_OF_AGENTS; ++i)
    {
        if (i != ID)
        {
            String(i).toCharArray(tempReciverID, 2);
            agent.createMessage(QUERY_IF, contentBuffer, tempReciverID);
            agent.deleteSentMessage();
        }
    }
}

void sendNogoodToLowestPrioityAndRemoveItFromAgentView()
{
    agent.newConversationSetup();
    agent.onLoopStart();

    for(int i = NUM_OF_AGENTS; i >= 1; --i)
    {
        if(agentView[i] != 0)
        {
            String(i).toCharArray(tempReciverID, 2);
            agent.createMessage(INFORM, contentBuffer, tempReciverID);
            agent.deleteSentMessage();

            agentView[i] = 0;
            break;
        }
    }

    checkAgentView();
}

void createAndSendTerminate()
{
    agent.newConversationSetup();
    createTerminateContent();

    agent.createMessageToAll(FAILURE, contentBuffer);
    agent.deleteSentMessage();
}

boolean isABPMsgReceived()
{
    if (agent.isMessageReceived())
    {
        String temp = String(agent.messageReceived->content);
        agent.deleteReceivedMessage();
        if (temp.substring(0, 3) == "ok?")
        {
            reviseAgentViewOnOkMsg(temp);
            checkAgentView();
            return true;
        }
        else if (temp.substring(0, 6) == "nogood")
        {
            reviseAgentViewOnNogoodMsg(temp);
            checkAgentView();
            return true;
        }
        else if (temp.substring(0,9) == "terminate")
        {
            terminate = true;
            return true;
        }
    }
    return false;
}

void reviseAgentViewOnOkMsg(String questionContent)
{
    Serial.print("Cached ok? question --> ");
    Serial.println(questionContent);

    agentView[questionContent[5] - '0'] = (color)(questionContent[7] - '0');
    
    printAgentView();
}

void reviseAgentViewOnNogoodMsg(String questionContent)
{
    Serial.println("Cached nogood message");

    int currentElementIndex = 8;

    while (questionContent[currentElementIndex] == '(')
    {
        agentView[questionContent[currentElementIndex+1] - '0'] = (color)(questionContent[currentElementIndex+3] - '0');
        currentElementIndex += 6;
    }

    printAgentView();
}

void printAgentView()
{
    Serial.println("Current agent view: id color");
    for (int i = 1; i <= NUM_OF_AGENTS; ++i)
    {
        Serial.print(i);
        Serial.print(" - ");
        Serial.println(agentView[i]);
    }
    Serial.println();
}

void checkAgentView()
{
    if (isConsistence(currentColor) == false)
    {
        currentColor = isConsistentColorAvailable();
        refreshCurrentColor();
        agentView[ID] = 0;

        if (currentColor == 0)
        {
            backtrack();
        }
        else
        {
            createAndSendOkQuestion();
        }

    }
}

void backtrack()
{
    createNogoodContent();
    Serial.println(contentBuffer);

    if(agentViewToString() == "" || ID == 1)
    {
        createAndSendTerminate();
        turnCurrentColorLedOff();
        terminate = true;
        Serial.println("termnating");
    }
    else
    {
        sendNogoodToLowestPrioityAndRemoveItFromAgentView();
    }
}

boolean isConsistence(color color)
{
    if (color == 0)
        return false;

    for (int i = 1; i <= NUM_OF_AGENTS; ++i)
    {   
        if (agentView[i] != 0)
        {
            if (agentView[i] == color)
                return false;

            for (int j = (i+1); j <= NUM_OF_AGENTS; ++j)
            {
                if (agentView[i] == agentView[j])
                    return false;
            }
        }
    }

    return true;
}

int isConsistentColorAvailable()
{
    for(int i = 0; i < NUM_OF_AVAILABLE_COLORS; ++i)
    {
        if (isConsistence(availableColors[i]) == true)
            return availableColors[i];
    }

    return 0;
}
