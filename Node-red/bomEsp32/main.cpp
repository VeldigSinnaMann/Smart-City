#include <IRremote.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <Wire.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "NTNU-IOT";
const char* password = "";

const long RECV_PIN = 32;
IRrecv irrecv(RECV_PIN);
unsigned long irNum = 0;  // inkommende signal

// Add your MQTT Broker IP address, example:
const char* mqtt_server = "10.25.17.1";  // rpi ip adresse

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// variablelr til å sende
char carID[8] = "0001";  // idNummer bil nummer. må bruke char[] format

// LED Pin
const int ledPin = 4;

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
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  IrSender.begin(DISABLE_LED_FEEDBACK);
  // default settings

  // setter opp mqtt og wifi
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(ledPin, OUTPUT);
}

int readIR(unsigned long irCode) {
  if (irCode == 0xE916FF00)  // knappeverdien til "1" på ir kontrolleren
  {
    return 0001;
  } else if (irCode == 0xE619FF00)  // knappeverdien til "2" på ir kontrolleren
  {
    return 0002;
  } else if (irCode == 0xF20DFF00) {  // 3
    return 0003;
  } else if (irCode == 0xf30cff00) {
    return 0004;
  } else if (irCode == 0xe718ff00) {
    return 0005;
  } else {
    return 404;
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (IrReceiver.decode()) {
    unsigned long irNum = IrReceiver.decodedIRData.decodedRawData;
    Serial.println(irNum, HEX);
    double readCar = readIR(irNum);
    Serial.println(readCar);
    if (readCar < 100) {
      char carString[8];
      dtostrf(readCar, 1, 2, carString);
      client.publish("esp32/bom", carString);
    }
    IrReceiver.resume();
  }
}