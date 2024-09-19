#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

#include "pid_tuner.h"

AsyncWebServer server(80);

void pid_tuner_init(const char *ssid, const char *pwd)
{
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(ssid, pwd);

    if (!SPIFFS.begin(true, "/storage"))
    {
        ESP_LOGE("FileSystem", "Failed to init SPIFFS.");
        return;
    }

    if (WiFi.waitForConnectResult() == WL_CONNECTED)
    {
        ESP_LOGI("WiFi", "WiFi connected");
        ESP_LOGI("WiFi", "%s", WiFi.localIP().toString().c_str());
    } else 
    {
        ESP_LOGE("WiFi", "WiFi connection failed.");
        return;
    }

    server.on("/", [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Heello world!");
    });

    server.on("/fs", [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/greeting.txt", "text/plain");
    });

    server.begin();
}