#include <MADuino.h>

RF24 radio(8,9);
RF24Network network(radio);

MADuino master(&radio, &network);

void setup() {
	master.agentSetup();
}

void loop() {
	master.onLoopStart();
	
	byte first, second, third, fl, fr, sl, sr, tl, tr;

	first = MADuino::boundToByte(15, 15);
	second = MADuino::boundToByte(0, 0);
	third = MADuino::boundToByte(B1001, B0110);

	Serial.println(MADuino::boundToByte(15, 15)); 		// 255
	Serial.println(MADuino::boundToByte(0, 0)); 		// 0
	Serial.println(MADuino::boundToByte(B1001, B0110)); // 150

	MADuino::extractBoundedByte(first, &fl, &fr);   // 15 15
	MADuino::extractBoundedByte(second, &sl, &sr);  // 0 0
	MADuino::extractBoundedByte(third, &tl, &tr);   // 9 6

	Serial.println(fl);                                 // 15
	Serial.println(fr);                                 // 15
	Serial.println(sl);                                 // 0
	Serial.println(sr);                                 // 0 
	Serial.println(tl);                                 // 9
	Serial.println(tr);                                 // 6

    Serial.println("--------------");
    Serial.println();

	delay(3000);
}
