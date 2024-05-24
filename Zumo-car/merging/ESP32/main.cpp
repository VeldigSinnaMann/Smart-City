#include <Wire.h>
#include <PubSubClient.h>
#include <WiFi.h>

const char *ssid = "NTNU-IOT";
const char *password = "";
const char *mqtt_server = "10.25.17.1"; // rpi ip adresse
const char* mqtt_topic_ratt = "zumo/kontrollerInput";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

char licencePlate[] = "0001";

void setup_wifi()
{
    delay(10);
    // kolbe pÃ¥ wifi
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    { // viser prikker til kobling er god
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("ESP8266Client"))
        {
            Serial.println("connected");
            // Subscribe
            client.subscribe("esp32/output");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

union FloatConverter
{
    float value;
    uint8_t bytes[sizeof(float)];
};

void setup()
{
    Wire.begin(4);                // Setter opp ESP32 som slave
    Wire.onReceive(receiveEvent); // Register event for mottak av data
    Serial.begin(9600);           // Starter serial for utskrift
    setup_wifi();
    client.setServer(mqtt_server, 1883);
}

void receiveEvent(int howMany)
{
    if (howMany == sizeof(float))
    {
        FloatConverter converter;
        for (int i = 0; i < sizeof(float); i++)
        {
            converter.bytes[i] = Wire.read();
        }

        // Bruker den konverterte flyttallsverdien
        Serial.print("Received float: ");
        Serial.println(converter.value, 3);


        char sendValue[20];
        dtostrf(converter.value, 1, 3, sendValue);
        strcat(sendValue, ", "); // Legg til komma 
        strcat(sendValue, licencePlate);
        client.publish("esp32/kjoreMonster", sendValue);
    }
}

void loop()
{
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();
}
