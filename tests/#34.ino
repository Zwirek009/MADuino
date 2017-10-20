#include <MADuino.h>

MADuino master(1, 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL);

void setup() {
	master.agentSetup();
}

void loop() {
	byte first, second, third, fl, fr, sl, sr, tl, tr;

	first = Message::boundToByte(15, 15);
	second = Message::boundToByte(0, 0);
	third = Message::boundToByte(B1001, B0110);

	Serial.println(Message::boundToByte(15, 15)); 		// 255
	Serial.println(Message::boundToByte(0, 0)); 		// 0
	Serial.println(Message::boundToByte(B1001, B0110)); // 150

	Message::extractBoundedByte(first, &fl, &fr);   // 15 15
	Message::extractBoundedByte(second, &sl, &sr);  // 0 0
	Message::extractBoundedByte(third, &tl, &tr);   // 9 6

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