#include <SPI.h>
#include <MFRC522.h>

//pins
const unsigned short int SdaPin = 10;
const unsigned short int RtsPin = 9;

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
      AccessAllowed();
    }
    //else, i'm sorry.
    else
    {
      AccessDenied();
    }
    //awaiting
    delay(3000);
  }
  else
  {
    return;
  }
}

void AccessAllowed()
{
  //send message of acess allowed 
  Serial.println("Allowed: " + EmployeeID);
}

void AccessDenied()
{
  //send message of acess denied 
  Serial.println("Denide: " + EmployeeID);
}
