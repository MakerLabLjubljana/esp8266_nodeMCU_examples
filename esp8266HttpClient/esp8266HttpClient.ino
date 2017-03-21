#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>


ESP8266WiFiMulti WiFiMulti;

void setup() {
    Serial.begin(115200);
    Serial.println("Serial ready!");
    WiFiMulti.addAP("ssid", "pass");
}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {
        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        // configure target server and url
        http.begin("https://www.trola.si/hajdrihova/", "9f 8e 70 c1 01 5a f7 33 1c 19 bf d0 3e 9c 3c ee eb 5b b9 6c"); //HTTPS
        //http.begin("http://www.tafr.si/trola/?station=hajdrihova"); //HTTP

        http.addHeader("Accept","application/json",false,true);
        
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been sent and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println(payload);

                DynamicJsonBuffer jsonBuffer(512);
                JsonObject& root = jsonBuffer.parseObject(payload);
          
                if (!root.success()){
                  Serial.println("parseObject() failed");
                }else{
                  int number = root["stations"][0]["number"];
                  String stationName = root["stations"][0]["name"].asString();
                  Serial.println(String(number)+" "+stationName);
                }
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }

    delay(10000);
}

