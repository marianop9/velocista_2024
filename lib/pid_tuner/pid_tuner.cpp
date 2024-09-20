#include "pid_tuner.h"

#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

AsyncWebServer server(80);
PIDController pidController;

void pid_tuner_set_endpoints();

void pid_tuner_init(const char *ssid, const char *pwd) {
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(ssid, pwd);

    if (!SPIFFS.begin(true, "/storage")) {
        ESP_LOGE("FileSystem", "Failed to init SPIFFS.");
        return;
    }

    if (WiFi.waitForConnectResult() == WL_CONNECTED) {
        ESP_LOGI("WiFi", "WiFi connected");
        ESP_LOGI("WiFi", "%s", WiFi.localIP().toString().c_str());
    } else {
        ESP_LOGE("WiFi", "WiFi connection failed.");
        return;
    }

    pid_tuner_set_endpoints();

    // temitas de CORS para probar la pagina desde la pc :)
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    server.begin();
}

void pid_tuner_set_endpoints() {
    server.on("/greet", [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Heello world!");
    });

    server.on("/fs", [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/greeting.txt", "text/plain");
    });

    server.on("/", [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    server.on("/script.js", [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/script.js", "text/javascript");
    });

    // server.on("/style.css", [](AsyncWebServerRequest *request) {
    //     request->send(SPIFFS, "/style.css", "text/css");
    // });

    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (!request->hasParam("param")) return;

        AsyncWebParameter *p = request->getParam("param");

        if (p == nullptr) {
            ESP_LOGI("param", "param was null");
            return;
        }

        ESP_LOGI("get-param", "got param: %s=%s", p->name().c_str(),
                 p->value().c_str());

        // String pvalue = p->value();
        PIDParam paramType = PIDParamFromString(p->value().c_str());

        int value = pidController.getParam(paramType);

        ESP_LOGI("get-param", "got value %d", value);

        request->send(200, "text/plain", String(value));
    });

    server.on("/set", HTTP_POST, [](AsyncWebServerRequest *request) {
        // retrieve POST paramater
        if (!request->hasParam("param")) {
            request->send(422, "text/plain", "expected param in querystring");
            return;
        }
        if (!request->hasParam("value")) {
            request->send(422, "text/plain",
                          "expected param value in querystring");
            return;
        }

        AsyncWebParameter *param = request->getParam("param");
        AsyncWebParameter *value = request->getParam("value");

        PIDParam paramType = PIDParamFromString(param->value().c_str());
        if (paramType == PIDParam::InvalidParam) {
            request->send(422, "text/plain", "received invalid PID param");
            return;
        }

        int paramValue = value->value().toInt();

        pidController.setParam(paramType, paramValue);

        request->send(200, "text/plain", String(paramValue));
    });
}
