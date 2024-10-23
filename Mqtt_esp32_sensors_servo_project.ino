#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
//Defining pins for water level sensor
#define POWER 33
#define SIGNAL 32

//Variables for the Motion  sensor
const int PIN_TO_SENSOR = 19; // GPIO19 pin connected to OUTPUT pin of sensor
int pinStateCurrent   = LOW;  // current state of pin
int pinStatePrevious  = LOW;  // previous state of pin

//Variables for water level sensor
int valueSensor=0;
int level=0;

//Servo Variables
Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
int servoPin = 2;


// Update these with values suitable for your network.
const char* ssid = "Wifi-Name";
const char* password = NULL;
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
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

  //Print the received message
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Read the sensors if 1 is received
  if ((char)payload[0] == '1') {

    //read the Water Level Sensor
    level=waterSensor();
    Serial.print("Water Level:");
    Serial.println(level);
    delay(1000);

    //read the Motion Detection Sensor
    pinStatePrevious = pinStateCurrent; // store old state
    pinStateCurrent = digitalRead(PIN_TO_SENSOR); 

    //if motion is detected
    if (pinStatePrevious == LOW && pinStateCurrent == HIGH) {   // pin state change: LOW -> HIGH
      Serial.println("Motion detected!");

      //Open the water valve(Servo motor) based on how much water is already in the bowl
      if(level < 1200){
        pos=map(level,0,1200,180,0);
        myservo.write(pos);
        delay(4000);
        pos=0;
        myservo.write(pos);
      }

    }
    else if (pinStatePrevious == HIGH && pinStateCurrent == LOW) {   // pin state change: HIGH -> LOW
      Serial.println("Motion stopped!");
    }
  }
}

//function that hels with reading the data from sensor
int waterSensor()
{
  digitalWrite(POWER,HIGH);
  delay(10);
  valueSensor=analogRead(SIGNAL);
  delay(10);
  digitalWrite(POWER,LOW);
  return valueSensor;
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopicDaniel", "hello world");
      // ... and resubscribe
      client.subscribe("inTopicDaniel");
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
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  //Initialize pins for Water Level Sensor
  pinMode(POWER,OUTPUT);
  digitalWrite(POWER,LOW);

  //Initialize the motion sensor
  pinMode(PIN_TO_SENSOR, INPUT);
  Serial.println("Started Motion Sensor!"); // set ESP32 pin to input mode to read value from OUTPUT pin of sensor

  //Initialize servo
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 500, 2400); // attaches the servo on pin 18 to the servo object
  myservo.write(pos);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    //Sending messages for testing purposes
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopicDaniel", msg);
  }


}
