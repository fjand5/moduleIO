#ifndef SYSTIME
#define SYSTIME
#define MAX_OF_TASK 8
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "sysDefine.h"
#include "sysFunc.h"
#include "FS.h"
namespace TIME {
	WiFiUDP ntpUDP;
	NTPClient timeClient(ntpUDP,"europe.pool.ntp.org",7*3600);

	bool gotTime = false;
	String getTimeByString(){
	  return timeClient.getFormattedTime();
	  }
	void initTime() {

	  timeClient.begin();
	  gotTime = timeClient.update();

	}
	int getTimeInMin(){
	  return timeClient.getHours()*60 + timeClient.getMinutes();
	  }

	void loop()
	{
	  if (gotTime)
	    return;
	  gotTime = timeClient.update();
	}
}
#endif
