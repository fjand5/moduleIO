
#ifndef JSON2CMD 
#define JSON2CMD
#include "sysFunc.h"
#include "mqttConnection.h"
#include "ioMng.h"
#include "update.h"
StaticJsonDocument<512> doc;

bool json2cmd(String topic, String content)
{
  DeserializationError err = deserializeJson(doc, content);
  if (err)
  {
#ifdef DEBUG
    Serial.println(String("ERR JSON"));
#endif
    return false;
  }
  if (doc["cmd"].as<String>() == "FMM")
  {
    // format memory
     return SPIFFS.format();
     
  }else if (doc["cmd"].as<String>() == "RST") // reset
  {
    ESP.reset();
  } else if (doc["cmd"].as<String>() == "SEP") // set pin
  {
    // IOMNG::setPin(doc["pin"].as<String>(), doc["state"].as<String>());
    return false;
  } else if (doc["cmd"].as<String>() == "GEP") // get pin
  {
    
    // MQTT::sendData(IOMNG::getPin(doc["pin"].as<String>()));
    return false;
  } else if (doc["cmd"].as<String>() == "GUT") // get up time
  {
    MQTT::sendData(String("{\"GUT\":")+(millis()/1000)+"}");
    return false;
  }  
  else if (doc["cmd"].as<String>() == "GTM") // get  time
  {
    MQTT::sendData(TIME::getTimeByString());
    return false;
  } 
  else if (doc["cmd"].as<String>() == "CLT") // set task
  {
    // return IOMNG::clearTask();
  }
  else if (doc["cmd"].as<String>() == "UPD") // update
  {
    return UPDATE::updateFirmware();
  }


}
#endif 
