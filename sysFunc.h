#ifndef SYSFUNC
#define SYSFUNC
#include <ArduinoJson.h>
#include "FS.h"
#include "sysDefine.h"


StaticJsonDocument<4048> JsonDoc;
bool isLoadConfigFile = false;
JsonObject getObj(String name, bool check = true) {

  JsonObject object;
  if(!isLoadConfigFile)
  {
    SPIFFS.begin();
    File file = SPIFFS.open(CONFIG_FILE, "r");
    DeserializationError err =deserializeJson(JsonDoc, file.readString());
    file.close();
    if(err){
          return object;
    }
  }
  
  isLoadConfigFile= true;

  if (name != NULL) {
    object = JsonDoc[name.c_str()].as<JsonObject>();
  } else if (JsonDoc.isNull()) {
    object = JsonDoc.to<JsonObject>();
  } else {
    object = JsonDoc.as<JsonObject>();
  }
 
  return object;
}
bool  setObj(JsonObject obj) {
  if (!isLoadConfigFile)
    return false;
  if(!SPIFFS.begin())
    return false;
  File file = SPIFFS.open(CONFIG_FILE, "w");
  serializeJson(obj, file);
  file.close();

  return true;
}
int mapPin(String pin){
  if(pin == "D0")
    return D0;
  if(pin == "D1")
    return D1;
  if(pin == "D2")
    return D2;
  if(pin == "D3")
    return D3;
  if(pin == "D4")
    return D4;
  if(pin == "D5")
    return D5;
  if(pin == "D6")
    return D6;
  if(pin == "D7")
    return D7;
  if(pin == "D8")
    return D8;
}
int mapMode(String mode){
  if(mode == "INPUT")
    return INPUT;
  if(mode == "OUTPUT")
    return OUTPUT;
  if(mode == "INPUT_PULLUP")
    return INPUT_PULLUP;

}
int mapState(String state){
  if(state == "HIGH")
    return HIGH;
  if(state == "LOW")
    return LOW;

}
#endif
