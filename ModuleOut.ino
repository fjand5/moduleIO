#define LED_BUILTIN 2
#include "Wifi.h"
// #include "ioMng.h"
#include "mqttConnection.h"
#include "json2cmd.h"
#include "time.h"

void setup() {

Serial.begin(115200);
pinMode(LED_BUILTIN, OUTPUT); 

WIFI::init(); 
ioMng inoutManager;

TIME::initTime();
MQTT::init();
MQTT::setCallback([](String topic, String data){
json2cmd(topic, data);

	});


}

void loop() {

	delay(50);
	WIFI::loop();
	MQTT::loop();
	TIME::loop();
}
/*
{"WIFI":{"ssid":"HT3","sspass":"longtranhhodau"},"IOMNG":{"pinArr":[{"pin":"D0","mode":"OUTPUT","state":"LOW"},{"pin":"D1","mode":"OUTPUT","state":"HIGH"},{"pin":"D2","mode":"OUTPUT","state":"HIGH"},{"pin":"D3","mode":"OUTPUT","state":"HIGH"}],"taskArr":[]},"MQTT":{"luat":{"adr":"mqtt.ngoinhaiot.com","port":1234,"user":"luat","pass":"12345679","rTopic":"luat/esp/rx","tTopic":"luat/esp/tx"}}}
*/
