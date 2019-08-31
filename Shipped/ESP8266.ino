#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "YOUR SSID";
const char* password = "YOUT KEY";

void setup()
{//$ad:https://jsonplaceholder.typicode.com/posts?id=1
	Serial.begin(115200);
	delay(5000);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(1000);
		Serial.println("Connecting to WiFi..");
	}

	Serial.println("Connected");
}

void loop()
{
	if (Serial.available())
	{
		String uriSerial = Serial.readString();
		if (uriSerial.startsWith("$ad:"))
		{
			Serial.println(uriSerial);
			uriSerial = ClearString(uriSerial);
		}

		if ((WiFi.status() == WL_CONNECTED))
		{
			HTTPClient http;
			http.begin("http://logikoz.tk/validar.php?id=" + uriSerial);

			if (http.GET() > 0)
			{
				Serial.println("$esp:" + http.getString());
			}
			//freeing up resources
			http.end();
		}
	}
}

String ClearString(String str)
{
	String ok = str;
	for (int i = 0; i < (sizeof(str) / sizeof(String)); i++)
	{
		ok.replace("\n", "");
		ok.replace(" ", "");
		ok.replace("?", "");
		ok.replace("⸮", "");
		ok.replace("$ad:", "");
	}
	return ok;
}
