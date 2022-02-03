#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <WiFiClient.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"


// настройки точки доступа
const char *ssid = "actuator";            // имя точки доступа
const char *password = "64(Me4J6#C!gZfj"; // пароль точки доступа
 
// настройки wifi подключения
const char* wifi_ssid = "ASUS_ROUTER";
const char* wifi_password = "aSus2020";

const char *url = "http://192.168.4.2:8081/zeroconf/switch";

// ----------------------- ПИНЫ ---------------------------
#define DHT_TYPE DHT22            //тип датчика DHT22
#define DHT_VCC D6                //питание датчика DHT22
#define DHT_PIN D5                // дата линия датчика DHT22

int max_temp = 26;
int min_temp = 25;

bool valve_is_opened = true;
String valveState = "OPEN";

DHT dht(DHT_PIN, DHT_TYPE);       // обьявляем объект dht, указывать тип и дата пин 
WiFiClient client;                // объявить объект класса wifi
HTTPClient restclient;            // объявить объект класса rest клиента
AsyncWebServer server(80);        // объявить объект класса http сервера

// html данные для отрисовки страницы
const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";



//настройки таймера задержки
unsigned long lastTime = 0;
unsigned long timerDelay = 5000; // 5 seconds (5000)

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String getTemperature() {
  float temperature = dht.readTemperature();
  Serial.println(temperature);
  return String(temperature);
}

String processor(const String& var) {
  if(var == "STATE"){
    return valveState;
   }
  else if (var == "TEMPERATURE"){
    return getTemperature();
  }
  return String();
}

void setup()
{
  Serial.begin(115200);            // Запуск последовательной связи

  if(!LittleFS.begin()){
    Serial.println("Error while mounting LittleFS");
    return;
  }
  
  WiFi.softAP(ssid, password);     // WiFi.softAP используется для запуска режима AP NodeMCU.
  Serial.print("Access Point:");   // Выводим информацию через последовательный монитор
  Serial.println(ssid);            // Сообщаем пользователю имя WiFi, установленное NodeMCU
  Serial.print("IP-адрес:");       // И IP-адрес NodeMCU
  Serial.println(WiFi.softAPIP()); // IP-адрес NodeMCU можно получить, вызвав WiFi.softAPIP ()

  pinMode(DHT_PIN, INPUT);         // D5 пин в режиме входа 
  pinMode (DHT_VCC, OUTPUT);       // D6 пин в режиме выхода 
  digitalWrite (DHT_VCC, HIGH);    // подать напряжение 3,3V на D6 пин
  dht.begin();                     // инициализация DHT22 3

  WiFi.begin(wifi_ssid, wifi_password);  // подключение к wifi сети
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

  restclient.begin(client, url);         // инициализация http клиента

  //SERVER BLOCK
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/style.css", "text/css");
  });

  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "index.js", "text/js");
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getTemperature().c_str());
  });
  // server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
  //   String inputMessage;
  //   String inputParam;
  //   // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
  //   if (request->hasParam(PARAM_INPUT_1)) {
  //     inputMessage = request->getParam(PARAM_INPUT_1)->value();
  //     inputParam = PARAM_INPUT_1;
  //   }
  //   // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
  //   else if (request->hasParam(PARAM_INPUT_2)) {
  //     inputMessage = request->getParam(PARAM_INPUT_2)->value();
  //     inputParam = PARAM_INPUT_2;
  //   }
  //   // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
  //   else if (request->hasParam(PARAM_INPUT_3)) {
  //     inputMessage = request->getParam(PARAM_INPUT_3)->value();
  //     inputParam = PARAM_INPUT_3;
  //   }
  //   else {
  //     inputMessage = "No message sent";
  //     inputParam = "none";
  //   }
  //   Serial.println(inputMessage);
  //   request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
  //                                    + inputParam + ") with value: " + inputMessage +
  //                                    "<br><a href=\"/\">Return to Home Page</a>");
  // });
  server.onNotFound(notFound);
  server.begin();
  //END SERVER BLOCK

  delay(1000);                           // для стабильности 
}

void close_valve () {
  // отправить запрос на включение реле
  restclient.addHeader("Content-Type", "application/json");                                  // header
  String RequestData = "{\"deviceid\":\"1000b91ec6\",\"data\":{\"switch\": \"on\"}}"; // payload
  int ResponseStatusCode = restclient.POST(RequestData);                                     // post запрос
  if (ResponseStatusCode == 200)
    {
      Serial.print("Relay has been switched on! Status: ");
      Serial.println(ResponseStatusCode);
      Serial.println("Valve has been closed: ");
      valve_is_opened = false;
    }
    else
    {
      Serial.print("Error! HTTP Response code: ");
      Serial.println(ResponseStatusCode);
    }
}

void open_valve () {
  // отправить запрос на выключение реле
  restclient.addHeader("Content-Type", "application/json");                                  // header
  String RequestData = "{\"deviceid\":\"1000b91ec6\",\"data\":{\"switch\": \"off\"}}"; // payload
  int ResponseStatusCode = restclient.POST(RequestData);                                     // post запрос
  if (ResponseStatusCode == 200)
    {
      Serial.print("Relay has been switched off! Status: ");
      Serial.println(ResponseStatusCode);
      Serial.println("Valve has been opened: ");
      valve_is_opened = true;
    }
    else
    {
      Serial.print("Error! HTTP Response code: ");
      Serial.println(ResponseStatusCode);
    }
}

void loop()
{
  if ((millis() - lastTime) > timerDelay)                                              // задержка
  {
    float temperature = dht.readTemperature();                                         // считать температуру
    Serial.print("Temperature: "); Serial.println(temperature);

    if (temperature > max_temp && valve_is_opened) {
        close_valve();
    } 
    if (temperature < min_temp && !valve_is_opened) {
        open_valve();
    }

    restclient.end(); // Free resources
    lastTime = millis();
  }
}


