#include <SPI.h>
#include <MFRC522.h>

//pins
const unsigned short int SdaPin = 10;
const unsigned short int RtsPin = 9;

//array containing allowed employees 
String RegistredEmployeeID[] = { "cardsIDHere" };
//create a object of type MFRC522
MFRC522 ReadRFID(SdaPin, RtsPin);

void setup()
{
	Serial.begin(9600);
	SPI.begin();
	ReadRFID.PCD_Init();
}

void loop()
{
	ReadCard();
}

String EmployeeID;
bool isChecked = false;

void ReadCard()
{
	EmployeeID = "";

	if (ReadRFID.PICC_IsNewCardPresent() && ReadRFID.PICC_ReadCardSerial())
	{
		for (byte i = 0; i < ReadRFID.uid.size; i++)
		{
			EmployeeID.concat(String(ReadRFID.uid.uidByte[i], HEX));
		}

		for (int i = 0; i < (sizeof(RegistredEmployeeID) / sizeof(String)); i++)
		{
			isChecked = EmployeeID.equalsIgnoreCase(RegistredEmployeeID[i]);
		}

		if (isChecked == true)
		{
			AccessAllowed();
		}
		else
		{
			AccessDenied();
		}
		delay(3000);
	}
	else
	{
		return;
	}
}

void AccessAllowed()
{
	Serial.println("Allowed: " + EmployeeID);
}

void AccessDenied()
{
	Serial.println("Denide: " + EmployeeID);
}
