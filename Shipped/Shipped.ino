/*
 Name:		Shipped.ino
 Created:	03/08/2019 19:38:15
 Author:	Logikoz
*/

void setup()
{
	pinMode(12, OUTPUT);
}

void loop()
{
	digitalWrite(12, HIGH);
	delay(1000);
	digitalWrite(12, LOW);
}
