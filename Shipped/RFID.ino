#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

//Digital pins
const unsigned short int SdaPin = 10;
const unsigned short int RtsPin = 9;
const unsigned short int BuzzerPin = 8;
const unsigned short int LedRedPin = 7;
const unsigned short int LedGreenPin = 6;
const unsigned short int RegisterEmployeeButtonPin = 5;
const unsigned short int LedYellowPin = 4;

//card admin for add or remove employee
String CardAdminID = "8bcbafd";
//array containing allowed employees
String RegistredEmployeeID[] = { "34611b" };
//create a object of type MFRC522
MFRC522 ReadRFID(SdaPin, RtsPin);

SoftwareSerial espS(2, 3);

void setup()
{
	// Initialize serial communications.
	Serial.begin(115200);
	espS.begin(115200);
	// Init SPI bus
	SPI.begin();
	// Init MFRC522 card
	ReadRFID.PCD_Init();

	pinMode(LedGreenPin, OUTPUT);
	pinMode(LedRedPin, OUTPUT);
	pinMode(LedYellowPin, OUTPUT);
	pinMode(BuzzerPin, OUTPUT);

	pinMode(RegisterEmployeeButtonPin, INPUT);
}

void loop()
{
	if (Serial.available())
	{
		// String a = ;
		espS.println(Serial.readString());
		while (true)
		{
			if (espS.available())
			{
				// String a = ClearRequest(espS.readString());

				Serial.println(espS.readString());
				break;
			}
		}
	}

	//if (digitalRead(RegisterEmployeeButtonPin) == HIGH)
	//{
	//	//light led to indicate process
	//	digitalWrite(LedYellowPin, HIGH);
	//	//waiting pass the card.
	//	do
	//	{
	//		Serial.println("passe o cartao...");
	//	}
	//	while (!IsCardUp());
	//	String cardID = ReadCard();
	//	Serial.println("ID is: " + cardID);
	//	//blink led green and whistle buzzer for indicate sucess in process.
	//	Whistle(1);
	//	//turn off yellow led.
	//	digitalWrite(LedYellowPin, LOW);
	//	Serial.println("saiu");
	//}
	//if (IsCardUp())
	//{
	//	CheckCardExists(ReadCard());
	//}
}

String ClearRequest(String str)
{
	String ret;
	for (int i = 0; i < (sizeof(str) / sizeof(String)); i++)
	{
		str.replace("?", "");
		str.replace("⸮", "");
		str.replace("$esp:", "");
		str.replace("\n", "");
	}
	return ret;
}

//keep employee card id
String EmployeeID;
//if the id exists or not, it will be saved here
bool isChecked;

bool IsCardUp()
{
	return ReadRFID.PICC_IsNewCardPresent() && ReadRFID.PICC_ReadCardSerial();
}

void CheckCardExists(String cardID)
{
	//initializing EmployeeID with empty for avoid bugs.
	EmployeeID = cardID;
	//light led to indicate process
	digitalWrite(LedYellowPin, HIGH);
	//checking if the id is from an employee.
	// String id = cardID == "8bcbafd" ? "4" : "29";
	// espS.println("$ad:" + id);
	String teste = "$ad:" + cardID;
	Serial.println(teste);
	espS.println(teste);
	while (true)
	{
		delay(100);
		String request;
		if (espS.available())
		{
			String re = espS.readString();
			Serial.println(re);
			request = ClearRequest(re);
			Serial.println(request);
			if (request == "true")
			{
				isChecked = true;
			}
			else //if(ClearRequest(request).startsWith("<html>"))
			{
				isChecked = false;
			}
			break;
		}
	}
	digitalWrite(LedYellowPin, LOW);
	//if true, granted acess
	if (isChecked == true)
	{
		WhistleAllowed();
	}
	//else, i'm sorry.
	else
	{
		WhistleDenied();
	}
	//waiting 2 secs for next check.
	delay(2000);
}

String ReadCard()
{
	String card = "";
	//reading id from card and saving to variable.
	for (byte i = 0; i < ReadRFID.uid.size; i++)
	{
		card.concat(String(ReadRFID.uid.uidByte[i], HEX));
	}
	return card;
}

void WhistleAllowed()
{
	//send message of acess allowed
	Serial.println("Allowed: " + EmployeeID);
	//whistle buzzer.
	Whistle(1);
}

void WhistleDenied()
{
	//send message of acess denied
	Serial.println("Denied: " + EmployeeID);
	//whistle buzzer.
	Whistle(2);
}

void Whistle(const int count)
{
	for (int i = 0; i < count; i++)
	{
		bool re = count == 1;
		short int led = re ? LedGreenPin : LedRedPin;
		//checking if count send is allowed or denied, and apply frequency.
		tone(BuzzerPin, re ? 1500 : 500);
		//and let there be light
		digitalWrite(led, HIGH);
		//await .2 secs for turn off.
		delay(200);
		//turn off buzzer.
		noTone(BuzzerPin);
		//turn off led
		digitalWrite(led, LOW);
		//...
		delay(100);
	}
}
