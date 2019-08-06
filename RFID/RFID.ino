#include <SPI.h>
#include <MFRC522.h>

//Analog pins
const unsigned short int RegisterEmployeeButtonPin = 5;
//Digital pins
const unsigned short int SdaPin = 10;
const unsigned short int RtsPin = 9;
const unsigned short int LedGreenPin = 6;
const unsigned short int LedRedPin = 7;
const unsigned short int LedYellowPin = 4;
const unsigned short int BuzzerPin = 8;

//card admin for add or remove employee
String CardAdminID = "8bcbafd";
//array containing allowed employees 
String RegistredEmployeeID[] = { "34611b" };
//create a object of type MFRC522
MFRC522 ReadRFID(SdaPin, RtsPin);

void setup()
{
	// Initialize serial communications with the PC
	Serial.begin(9600);
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
	if (digitalRead(RegisterEmployeeButtonPin) == HIGH)
	{
		//light led to indicate process
		digitalWrite(LedYellowPin, HIGH);
		//waiting pass the card.
		do
		{
			Serial.println("passe o cartao...");
		}
		while (!IsCardUp());
		String cardID = ReadCard();
		Serial.println("ID is: " + cardID);
		//blink led green and whistle buzzer for indicate sucess in process.
		Whistle(1);
		//turn off yellow led.
		digitalWrite(LedYellowPin, LOW);
		Serial.println("saiu");
	}

	if (IsCardUp())
	{
		CheckCardExists();
	}
}
//keep employee card id
String EmployeeID;
//if the id exists or not, it will be saved here
bool isChecked;

bool IsCardUp()
{
	return ReadRFID.PICC_IsNewCardPresent() && ReadRFID.PICC_ReadCardSerial();
}

void CheckCardExists()
{
	//initializing EmployeeID with empty for avoid bugs.
	EmployeeID = "";
	//light led to indicate process
	digitalWrite(LedYellowPin, HIGH);
	//checking if the id is from an employee.
	for (int i = 0; i < (sizeof(RegistredEmployeeID) / sizeof(String)); i++)
	{
		isChecked = (EmployeeID = ReadCard()).equals(RegistredEmployeeID[i]);
	}
	delay(300);
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
	//waiting 3 secs for next check.
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
	Serial.println("Denide: " + EmployeeID);
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
