/*
Pipeline alarm for arduino esp8266 board
Turn an alarm on when a pipeline is red
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your project.

const char* ssid = "{Your SSID}";
const char* password = "{Your Wifi Password}";
const char* mqtt_server = "{Your MQTT host}";
const char*  topic_name = "{Your topic name}";
const String  clientId = "{Your client ID}";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Last pipeline status [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED and Relay if an 1 was received
  if ((char)payload[0] == '1') {
    digitalWrite(13, LOW);   // Turn the LED on 
    digitalWrite(12, HIGH);   // Turn the Relay on
  } else {
    digitalWrite(13, HIGH);  // Turn the LED off by making the voltage HIGH
    digitalWrite(12, LOW);  // Turn the Relay off
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe(topic_name);
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
  pinMode(13, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(12, OUTPUT);     // Initialize the Relay pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
      // ... and resubscribe
    client.subscribe(topic_name);
  }
}
