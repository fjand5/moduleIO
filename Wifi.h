#ifndef WIFI
#define WIFI
#define MAX_CHAR_SIZE_WIFI 32
#define DEFAULT_APID "NNIOT"
#define DEFAULT_APPASS "12345679"
#include "sysDefine.h"
#include "sysFunc.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "html/index.h"
#include "html/config.h"
#include "FS.h"
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266HTTPClient.h>

namespace WIFI {
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

struct wifiInfoStr
{
  char apid[MAX_CHAR_SIZE_WIFI] = {NULL};
  char appass[MAX_CHAR_SIZE_WIFI] = {NULL};
  char ssid[MAX_CHAR_SIZE_WIFI] = {NULL};
  char sspass[MAX_CHAR_SIZE_WIFI] = {NULL};
#ifdef DEBUG
  void printMe() {
    LOG(String("apid: ") + apid);
    LOG(String("appass: ") + appass);
    LOG(String("ssid: ") + ssid);
    LOG(String("sspass: ") + sspass);
  };
#endif
} wifiInfo;



void updateInfo()
{
  JsonObject wifiObj = getObj("WIFI", false);
  if (wifiObj.isNull()) {
    strcpy(wifiInfo.apid, DEFAULT_APID);
    strcpy(wifiInfo.appass, DEFAULT_APPASS);
  } else {
    if (wifiObj.containsKey("apid") && wifiObj.containsKey("appass")) {
      strcpy(wifiInfo.apid, wifiObj["apid"].as<const char *>());
      strcpy(wifiInfo.appass, wifiObj["appass"].as<const char *>());
    }
    if (wifiObj.containsKey("ssid") && wifiObj.containsKey("sspass")) {
      strcpy(wifiInfo.ssid, wifiObj["ssid"].as<const char *>());
      strcpy(wifiInfo.sspass, wifiObj["sspass"].as<const char *>());
    }

  }



}

void init() {

  updateInfo();

  server.on("/", [] {
    LOG("come");
    if (server.hasArg("ssid") && server.hasArg("sspass"))
    {

      JsonObject mainObj = getObj("");
      mainObj["WIFI"]["ssid"] = server.arg("ssid");
      mainObj["WIFI"]["sspass"] = server.arg("sspass");
      setObj(mainObj);

      server.send(200, "text/plain; charset=utf-8", "Đã thiết lập xong");



    }
    else
    {
      server.send(200, "text/html", String(__DATE__) + " : " + __TIME__ + index_page);
    }

  });
  server.on("/config", HTTP_GET, []() {
    SPIFFS.begin();
    String tmp;
    File file = SPIFFS.open(CONFIG_FILE, "r");
    tmp = String(config_page_befor) + file.readString() +  config_page_after;
    server.send(200, "text/html", tmp.c_str());
    file.close();

  });
  server.on("/config", HTTP_POST, []() {
    server.send(200, "text/plain", "");

  }, [] {

    HTTPUpload& upload = server.upload();
    static File fsUploadFile;
    if (upload.status == UPLOAD_FILE_START) {
      fsUploadFile = SPIFFS.open(CONFIG_FILE, "w");

    } else if (upload.status == UPLOAD_FILE_WRITE) {

      if (fsUploadFile) {
        fsUploadFile.write(upload.buf, upload.currentSize);
#ifdef DEBUG
        char* buf = (char*)calloc(upload.currentSize + 1, sizeof( char ));
        memcpy(buf, &upload.buf, upload.currentSize);
        String tmp = String(buf);
        Serial.print(tmp);
#endif
      }

    } else if (upload.status == UPLOAD_FILE_END) {
      if (fsUploadFile) {
        fsUploadFile.close();
      }
    }
  });


  server.on("/listWifi", [] {
    String list;
    int n = WiFi.scanNetworks();
    if (n == 0) {
      list = "no networks found";
    } else {
      for (int i = 0; i < n; ++i) {
        list += String(WiFi.SSID(i)) + "|" + WiFi.BSSIDstr(i);

      }
    }
    server.send(200, "text/html", "list");

  });
  server.begin();

  WiFi.disconnect();
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(wifiInfo.apid, wifiInfo.appass);
  if (strlen(wifiInfo.ssid) != 0 && strlen(wifiInfo.sspass) != 0)
    WiFi.begin(wifiInfo.ssid, wifiInfo.sspass);
  else
    WiFi.begin("NA", "NA");

  while (WiFi.status() != WL_CONNECTED )
  {
    delay(100);
    Serial.print(".");
    server.handleClient();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  httpUpdater.setup(&server);

  if (system_get_rst_info()->reason == REASON_EXT_SYS_RST)
  {

    while (millis() < 10000)
    {
      LOG(String("da ket noi ") + millis());
      server.handleClient();
      delay(500);
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(100);
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(100);
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(100);
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(100);

    }

  }
 
}

void loop()
{
  server.handleClient();

}
}
#endif