#include <MADuino.h>

void setup() {
  // put your setup code here, to run once:
  MADuino agent(master, 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL);
}

void loop() {
  // put your main code here, to run repeatedly:
  agent.runMaster();
}
