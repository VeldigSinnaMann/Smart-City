//dette er sender delen av kontroller modulen. Det oppleves at det er et delay på systemet på ca 0.2s fra sendt til 
// mottat singal. denne modulen sender potValue. 

#include <PubSubClient.h>
#include <WiFi.h>
#include <Wire.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "NTNU-IOT";
const char* password = "";

// Add your MQTT Broker IP address, example:
const char* mqtt_server = "10.25.18.202";  // rpi ip adresse

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// variablelr til å sende
//float temperature = 0;
//float pressure = 0;
int potValue;

// pins
const int potPin = 32;

void setup_wifi() {
  delay(10);
  // kolbe på wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {  // viser prikker til kobling er god
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the
  // message is either "on" or "off". Changes the output state according to the
  // message
  /*
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if (messageTemp == "on") {
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    } else if (messageTemp == "off") {
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
  */
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  // default settings
  // sjekker at sensor er riktig koblet. trengs ikke dersom en ikke bruker bmp
  // sensor
  unsigned status;

  // setter opp mqtt og wifi
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // sender melding hvert 5. sek
  long now = millis();
  if (now - lastMsg > 100) {
    lastMsg = now;
  /*
  // eskempel på en til mqtt utsending
    // Temperature in Celsius
    //temperature = bmp.readTemperature() / 100;
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.println(tempString);
    client.publish("esp32/temperature", tempString);
*/
    potValue = analogRead(potPin);
    char potString[8];
    Serial.println(potValue);
    dtostrf(potValue, 1, 2, potString);
    client.publish("esp32/kontrollerOutput", potString);
  }
}
