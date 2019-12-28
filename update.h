#ifndef UPDATESYS 
#define UPDATESYS
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
namespace UPDATE{
	bool updateFirmware(){
		WiFiClient client;
		if(ESPhttpUpdate.update(client, LINK_FIRMWARE) != HTTP_UPDATE_OK){
		  return false;
		};
		return true;
	}
}
#endif