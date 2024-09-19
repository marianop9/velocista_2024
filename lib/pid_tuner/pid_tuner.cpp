#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "freertos/FreeRTOS.h"

#include "pid_tuner.h"

AsyncWebServer server(80);
PIDController pidController;

void pid_tuner_set_endpoints();

PIDParam PIDParamFromString(const String &str)
{
    ESP_LOGI("PID param from string", "got param %s", str.c_str());
    if (str.equals("kp"))
        return Proportional;
    else if (str.equals("ki"))
        return Integral;
    else
        return Derivative;
}


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
    }
    else
    {
        ESP_LOGE("WiFi", "WiFi connection failed.");
        return;
    }

    pid_tuner_set_endpoints();

    server.begin();
}

void pid_tuner_set_endpoints()
{
    server.on("/", [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Heello world!"); });

    server.on("/fs", [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/greeting.txt", "text/plain"); });

    server.on("/index.html", [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html", "text/html"); });

    server.on("/style.css", [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/style.css", "text/css"); });

    server.on("/script.js", [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/script.js", "text/javascript"); });

    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        if (request->hasParam("param"))
        {
            AsyncWebParameter *p = request->getParam("param");

            // String pvalue = p->value();
            PIDParam param = PIDParamFromString(p->value());

            int value = pidController.getParam(param);

            request->send(200, "text/plain", String(value));
        } });
}

void PIDController::setParam(PIDParam param, int value)
{
    taskENTER_CRITICAL(&mux);
    switch (param)
    {
    case Proportional:
        kp = value;
        break;
    case Integral:
        ki = value;
        break;
    case Derivative:
        kd = value;
        break;
    default:
        break;
    }
    taskEXIT_CRITICAL(&mux);
}

int PIDController::getParam(PIDParam param)
{
    taskENTER_CRITICAL(&mux);
    switch (param)
    {
    case Proportional:
        return kp;
    case Integral:
        return ki;
    case Derivative:
        return kd;
    default:
        return -1;
    }
    taskEXIT_CRITICAL(&mux);
}

void PIDController::update(int error)
{
    P = error;
    I = I + error;
    D = error - lastError;

    lastError = error;

    adjdustment = P * kp + I * ki + D * kd; // calculate the correction
}