#include <Arduino.h>

#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#include <DHTNew.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <FS.h>
#include "LittleFS.h"


// Конфигурация WIFI подключения
const char* wifi_ssid = "ASUS_ROUTER";
const char* wifi_password = "aSus2020";

// настройки точки доступа
const char *ssid = "actuator";            
const char *password = "64(Me4J6#C!gZfj"; 

// ----------------------- ПИНЫ ---------------------------
#define DHT_VCC D6                // питание датчика DHT22
#define DHT_PIN D5                // дата линия датчика DHT22


DHT dht(DHT_PIN, DHT_MODEL_DHT22);  // обьявляем объект класса dht
AsyncWebServer server(80);          // объявить объект класса http сервера


// Переменные
String valveState = "OPEN";


//настройки таймера задержки
unsigned long lastTime = 0;
unsigned long timerDelay = 5000; // 5 seconds (5000)


// Функции для http сервера ----------------
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String getTemperature() {
  delay(dht.getMinimumSamplingPeriod());
  float temperature = dht.readTemperature();
  return String(temperature, 1);
}

String getHumidity() {
  delay(dht.getMinimumSamplingPeriod());
  float temperature = dht.readHumidity();
  return String(temperature, 1);
}

String processor(const String& var) {
  if(var == "STATE"){
    return valveState;
   }
  else if (var == "TEMPERATURE"){
    return getTemperature();
  }
  else if (var == "HUMIDITY"){
    return getHumidity();
  }
  return String();
}
// -----------------------------------------


void setup() {
  Serial.begin(115200);

  // Старт датчика DHT22 ----------------------------------------------
  pinMode(DHT_PIN, INPUT);         // D5 пин в режиме входа 
  pinMode (DHT_VCC, OUTPUT);       // D6 пин в режиме выхода 
  digitalWrite (DHT_VCC, HIGH);    // подать напряжение 3,3V на D6 пин
  dht.begin();
  // -------------------------------------------------------------------

  // Подключение к wifi сети --------------------------------------------
  WiFi.begin(wifi_ssid, wifi_password);  
  Serial.println(".");
  Serial.println("WIFI connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  // -------------------------------------------------------------------


  // Точка доступа для sonoff --------------------------------------------
  WiFi.softAP(ssid, password);     // WiFi.softAP используется для запуска режима AP NodeMCU.
  Serial.print("Access Point:");   // Выводим информацию через последовательный монитор
  Serial.println(ssid);            // Сообщаем пользователю имя WiFi, установленное NodeMCU
  Serial.print("IP-адрес:");       // И IP-адрес NodeMCU
  Serial.println(WiFi.softAPIP()); // IP-адрес NodeMCU можно получить, вызвав WiFi.softAPIP ()
  // -------------------------------------------------------------------


  // Проверка файловой системы -----------------------------------------
  if(!LittleFS.begin()){
    Serial.println("Error while mounting LittleFS");
    return;
  }
  // -------------------------------------------------------------------

  // Эндпойнты web сервера ----------------------------------------------
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/style.css", "text/css");
  });

  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "index.js", "text/js");
  });

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "favicon.ico", "image/ico");
  });

  server.on("/get_temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getTemperature().c_str());
  });

  server.on("/get_humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getHumidity().c_str());
  });

  server.onNotFound(notFound);
  server.begin();
}
 

void loop() {

}