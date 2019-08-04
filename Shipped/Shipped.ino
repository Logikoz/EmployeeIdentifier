#include <SPI.h>
#include <MFRC522.h>

//pins
const unsigned short int SdaPin = 10;
const unsigned short int RtsPin = 9;
const unsigned short int LedGreenPin = 6;
const unsigned short int LedRedPin = 7;
const unsigned short int BuzzerPin = 8;
//number of times the buzzer will beep.
const unsigned short int CountBuzzerAllowed = 1;
const unsigned short int CountBuzzerDenied = 2;

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
  pinMode(BuzzerPin, OUTPUT);
}

void loop()
{
  ReadCard();
}
//keep employee card id
String EmployeeID;
//if the id exists or not, it will be saved here
bool isChecked;

void ReadCard()
{
  //initializing EmployeeID with empty for avoid bugs.
  EmployeeID = "";
  //checking if there is a card on top of the reader
  if (ReadRFID.PICC_IsNewCardPresent() && ReadRFID.PICC_ReadCardSerial())
  {
    //reading id from card and saving to variable.
    for (byte i = 0; i < ReadRFID.uid.size; i++)
    {
      EmployeeID.concat(String(ReadRFID.uid.uidByte[i], HEX));
    }
    //checking if the id is from an employee.
    for (int i = 0; i < (sizeof(RegistredEmployeeID) / sizeof(String)); i++)
    {
      isChecked = EmployeeID.equalsIgnoreCase(RegistredEmployeeID[i]);
    }
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
    delay(3000);
  }
  else
  {
    return;
  }
}

void WhistleAllowed()
{
  //send message of acess allowed 
  Serial.println("Allowed: " + EmployeeID);
  //whistle buzzer.
  Whistle(CountBuzzerAllowed);
}

void WhistleDenied()
{
  //send message of acess denied 
  Serial.println("Denide: " + EmployeeID);
  //whistle buzzer.
  Whistle(CountBuzzerDenied);
}

void Whistle(const int count)
{
  for (int i = 0; i < count; i++)
  {
    //checking if count send is allowed or denied, and apply frequency.
    tone(BuzzerPin, count == 1 ? 1500 : 500);
    //and let there be light
    digitalWrite(LedGreenPin, HIGH);
    //await .2 secs for turn off.
    delay(200);
    //turn off buzzer.
    noTone(BuzzerPin);
    //turn off led
    digitalWrite(LedGreenPin, LOW);
    //...
    delay(100);
  }
}
