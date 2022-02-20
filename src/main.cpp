#include <Arduino.h>

#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#include <DHTNew.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <FS.h>
#include "LittleFS.h"

//настройки таймера задержки
unsigned long lastTime = 0;
unsigned long timerDelay = 5000; // 5 seconds (5000)

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
String valveState = "OPEN";               // статус клапана для отабражения в html
int max_temp = 26;                        // уставка для макс температуры
int min_temp = 25;                        // уставка для мин температуры
const char* max_temp_file = "/max.cfg";   // файл для хранения настроек
const char* min_temp_file = "/min.cfg";   // файл для хранения настроек
const char* input_max_temp = "max_temp";  // name в html форме
const char* input_min_temp = "min_temp";  // name в html форме


// объявление функции
void notFound(AsyncWebServerRequest *request);
String getTemperature();
String getHumidity();
String getValveState();
String processor(const String& var);
String get_max_temp();
String get_min_temp();
String readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, const char * message);


void setup() {

  Serial.begin(115200);

  // Старт датчика DHT22 ----------------------------------------------
  pinMode(DHT_PIN, INPUT);         // D5 пин в режиме входа 
  pinMode (DHT_VCC, OUTPUT);       // D6 пин в режиме выхода 
  digitalWrite (DHT_VCC, HIGH);    // подать напряжение 3,3V на D6 пин
  dht.begin();
  // -------------------------------------------------------------------

  // Работа файловой системы -----------------------------------------
  if(!LittleFS.begin()){
    Serial.println("Error while mounting LittleFS");
    return;
  }
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

  server.on("/get_valve_state", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getValveState().c_str());
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;

    if (request->hasParam(input_max_temp)) {
      inputMessage = request->getParam(input_max_temp)->value();
      writeFile(LittleFS, max_temp_file, inputMessage.c_str());
      delay(15);
      max_temp = get_max_temp().toInt();
      Serial.println(max_temp);
    };

    if (request->hasParam(input_min_temp)) {
      inputMessage = request->getParam(input_min_temp)->value();
      writeFile(LittleFS, min_temp_file, inputMessage.c_str());
      delay(15);
      min_temp = get_min_temp().toInt();
      Serial.println(min_temp);
    };

  });
  server.onNotFound(notFound);
  server.begin();
}
 

void loop() {

}


// Функции для http сервера ----------------
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String get_max_temp() {
  String value = readFile(LittleFS, max_temp_file);
  int max = value.toInt();
  return String(max);
};

String get_min_temp() {
  String value = readFile(LittleFS, min_temp_file);
  int min = value.toInt();
  return String(min);
};

String getTemperature() {
  delay(dht.getMinimumSamplingPeriod());
  float temperature = dht.readTemperature();
  return String(temperature, 1);
};

String getHumidity() {
  delay(dht.getMinimumSamplingPeriod());
  float humidity = dht.readHumidity();
  return String(humidity, 1);
};

String getValveState() {
  return valveState;
};

String processor(const String& var) {
  if(var == "STATE"){
    return getValveState();
   }
  else if (var == "TEMPERATURE"){
    return getTemperature();
  }
  else if (var == "HUMIDITY"){
    return getHumidity();
  }
  else if (var == "MAX_TEMP"){
    return get_max_temp();
  }
  else if (var == "MIN_TEMP"){
    return get_min_temp();
  }
  return String();
}
// -----------------------------------------

// записать настройки в пзу память 
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
};

// считать настройки из пзу 
String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  return fileContent;
};