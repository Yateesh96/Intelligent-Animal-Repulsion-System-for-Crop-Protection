#include "PubSubClient.h"	//	Connect	and	publish	to	the	MQTT	broker
#include "WiFi.h"	//	Enables	the	ESP32	to	connect	to	the	local	network	(via	WiFi)
#define	OUTPUT_1 2	//	LED	1	is	connected	to	pin	2
const char *SSID="m11";
const char *PWD="12345679";
String messageTemp;

void connectToWiFi()
{
	Serial.print("Connecting to ");
    WiFi.begin(SSID,PWD);
	Serial.println(SSID);
	while(WiFi.status()!=WL_CONNECTED)
	{
		Serial.print(".");
		delay(500);
	}
	Serial.print("!Connected!");
}
//	MQTT	client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

char *mqttServer="broker.hivemq.com";
char *mqus="samplebird";
char *mqpa="password";
int	mqttPort=1883;
const char* topicOutput1="yat/1";

void setupMQTT()
{   
	Serial.println("inside setup mqtt");
	mqttClient.setServer(mqttServer,mqttPort);
	//	set	the	callback function
	mqttClient.setCallback(callback);
}
void callback(char*	topic,byte* payload,unsigned int length)
{  
	Serial.print("Message arrived on topic:	");
	Serial.print(topic);
	Serial.print(".	Message: ");
	for	(int i=0;i<length;i++)
	{
		Serial.print((char)payload[i]);
		messageTemp+=(char)payload[i];
	}
	Serial.println();
	//	 If	a	message	is	received	on	 the	 topic	esp/output1,	check	if	 the	message	is	either	 "true"	or	"false".
	//	Changes	the	output	state	according	to	the	message
    if(String(topic)=="yat/1")
	{     
		Serial.println("Changning output to ");
		Serial.println(messageTemp);
	}
}
boolean	mqttConnect()
{
	Serial.print("Connecting	to	");
	Serial.print(mqttServer);
	//	Connect	to	MQTT	Broker	without	username	and	password
	boolean	status=mqttClient.connect(mqttServer);
	if(status==false)
	{
		Serial.println("	fail");
		ESP.restart();
		return	false;
	}
    Serial.println("	success");
	mqttClient.subscribe(topicOutput1);
	return	mqttClient.connected();
}
void setup()
{
	Serial.begin(115200);
	//pinMode(DHT,INPUT);
	pinMode(OUTPUT_1,OUTPUT);
	//pinMode(OUTPUT_2,OUTPUT);
	connectToWiFi();
	setupMQTT();
	if( mqttConnect() )
    {
      Serial.println("mqtt connect succeeded");
    }
    else
    {
      Serial.println("mqtt connect failed");
    }
}
void loop()
{
	if	(mqttClient.connected())
	{
		Serial.println("MQTT Success");
    	//Serial.println(messageTemp);
		mqttClient.loop();
		delay(5000); 
    }
    else
    {
       Serial.println(" MQTT fails");
       delay(5000);
    }
}