int mode = 0;

void setup() 
{
  // declare connected Leds
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop() 
{
  turnAllLedsOff();
  // choose mode
  switch(mode++)
  {
    case 0:
      oneLedCircle();
      break;
    case 1:
      twoLedsThereAndBackAgain();
      break;
    default:
      mode = 0;
  }
}

void oneLedCircle()
{
  int ledOn = 0;
  
  for (int i = 0; i < 100; i++)
  {
    digitalWrite(ledOn++, LOW);
    if (ledOn == 8)
      ledOn = 0;
    digitalWrite(ledOn, HIGH);
    delay(500);
  }
}

void twoLedsThereAndBackAgain()
{
  int led1On = 0;
  int led2On = 7;

  digitalWrite(led1On, HIGH);
  digitalWrite(led2On, HIGH);
  
  for(int i = 0; i < 67; i++)
  {
    
    digitalWrite((i % 4), LOW);
    digitalWrite((7 - (i %4)), LOW);
    digitalWrite(((i+1) % 4), HIGH);
    digitalWrite((7 - ((i+1) % 4)), HIGH);
    delay(750);
  }
}

void turnAllLedsOff()
{
  for(int i = 0; i < 8; i++)
    digitalWrite(i, LOW); 
}


