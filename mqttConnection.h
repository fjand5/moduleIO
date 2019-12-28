#ifndef MQTTCONNECTION
#define MQTTCONNECTION
#define MAX_CHAR_SIZE_MQTT 64
#define MAX_MQTT_CONNECTION 4
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "sysFunc.h"


class mqttConnection : public PubSubClient
{
  private:
    char _name[MAX_CHAR_SIZE_MQTT];
    char _pass[MAX_CHAR_SIZE_MQTT];
    char _rTopic[MAX_CHAR_SIZE_MQTT];
    char _tTopic[MAX_CHAR_SIZE_MQTT];
    int _timeOut = 100;
    WiFiClient client;
    void (*onDataCome) (String topic, String data) = NULL;

  public:
    mqttConnection(const char * server, uint16_t  port, const char * name, const char * pass): PubSubClient(client)	{
      strcpy(_name, name);
      strcpy(_pass, pass);
      client.setTimeout(_timeOut);
      setServer(server, port);
      setCallback([ = ](char* topic, byte * payload, unsigned int length) {
        String content = "";
        for (int i = 0; i < length; i++) {
          content += (char)payload[i];
        }
        if (onDataCome != NULL) {
          onDataCome(String(topic), content);
        }
      });
    };
    void reconnect() {
      if (!client.connected()) {
        randomSeed(micros());
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        if (connect(clientId.c_str(), _name, _pass)) {
          subscribe(_rTopic);
#ifdef DEBUG
          Serial.println(String("RECONNECT SUCCESS !!!"));
#endif
        } else {
          if (_timeOut < 500)
            _timeOut += 50;
          client.setTimeout(_timeOut);
#ifdef DEBUG
          Serial.println(String("RECONNECT FAILED !!! ") + _name + " " + _pass + " " + state());
#endif
        }
      }
    }
    void setTopic(const char * rTopic, const char * tTopic) {
      strcpy(_rTopic, rTopic);
      strcpy(_tTopic, tTopic);
    }
    void setOnDataCome(void (*func) (String topic, String data)) {
      onDataCome = func;
    }
    int sendData(String data) {
      if (_tTopic == "")
        return 0;
      return publish(_tTopic, data.c_str(), data.length());
    }
    void handler() {
      if (!connected()) {
        reconnect();
      }
      loop();
    }

    ~mqttConnection() {};

};

namespace MQTT {
mqttConnection * arrMqtt[MAX_MQTT_CONNECTION] = {NULL};
JsonObject mqttObj;
bool noObj = true;
void init() {
  mqttObj = getObj("MQTT");
  if (mqttObj.isNull())
  {
    noObj = true;
    return;
  }
  noObj = false;
  if (mqttObj.size() > MAX_MQTT_CONNECTION)
    return;

  int index = 0;
  for (JsonPair kv : mqttObj) {

    JsonObject tmp = kv.value().as<JsonObject>();

    arrMqtt[index] = new mqttConnection(
      tmp["adr"].as<char *>(),
      tmp["port"].as<uint16_t>(),
      tmp["user"].as<char *>(),
      tmp["pass"].as<char *>()

    );
    if (tmp.containsKey("rTopic") && tmp.containsKey("tTopic")) {

      arrMqtt[index]->setTopic(
        tmp["rTopic"].as<char *>(),
        tmp["tTopic"].as<char *>()
      );
    }

    index++;

  }

}
void loop() {
  static int index = 0;
  if (noObj)
    return;
  arrMqtt[index++ % min((int)mqttObj.size(), MAX_MQTT_CONNECTION)]->handler();
}

void setCallback(void (*func) (String topic, String data)) {
  for (int i = 0; i < min((int)mqttObj.size(), MAX_MQTT_CONNECTION); ++i)
  {
    arrMqtt[i]->setOnDataCome(func);
  }
}
void sendData(String data) {
  for (int i = 0; i < min((int)mqttObj.size(), MAX_MQTT_CONNECTION); ++i)
  {
    arrMqtt[i]->sendData(data);
  }

}

}
#endif
