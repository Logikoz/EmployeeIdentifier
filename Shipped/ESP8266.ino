#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "rede";
const char* password = "senha";

void setup()
{
	Serial.begin(9600);
	delay(5000);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(1000);
		Serial.println("Connecting to WiFi..");
	}

	Serial.println("Connected to the WiFi network");

}

void loop()
{
	if ((WiFi.status() == WL_CONNECTED))
	{
		HTTPClient http;

		http.begin("http://jsonplaceholder.typicode.com/todos/1");
		int httpCode = http.GET();

		if (httpCode > 0)
		{
			String payload = http.getString();
			Serial.println(payload);
		}
		else
		{
			Serial.println("Error on HTTP request");
		}

		http.end();
	}
}