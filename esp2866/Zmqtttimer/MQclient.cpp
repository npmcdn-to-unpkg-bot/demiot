#include "MQclient.h"
#include <Arduino.h>
#include <PubSubClient.h>

char itopic[16];
char ipayload[80];
bool NEW_MAIL=0;

Console::Console(char* devid, PubSubClient& client ){
  cdevid = devid;
  cclient = client;
}

void Console::log(char* dd){
	char log[20];
	strcpy(log,cdevid);
	strcat(log,"/log");
  if (cclient.connected()){
    cclient.publish(log, dd, true);
  }		
}

MQclient::MQclient(char* devid){
	cdevid = devid;
}

void MQclient::reconn(PubSubClient& client) {
  Serial.print("Attempting remo MQTT connection...");
  if (client.connect(cdevid)) {
    Serial.println("connected");
    char ccmd[20];
    strcpy(ccmd, cdevid);
  	strcat(ccmd,"/cmd");
    client.subscribe(ccmd);
    return;
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    delay(5000);
    Serial.println(" try again in 5 seconds");
  }
}

void handleCallback(char* topic, byte* payload, unsigned int length){
  for (int i=0;i<strlen(topic);i++) {
    itopic[i] = topic[i];
  }
  itopic[strlen(topic)] = '\0';  
  for (int i=0;i<length;i++) {
    ipayload[i] = (char)payload[i];
  }
  ipayload[length] = '\0';
  NEW_MAIL = 1;
}