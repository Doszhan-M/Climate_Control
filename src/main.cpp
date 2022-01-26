#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

//настройки точки доступа
const char *ssid = "actuator";            // имя точки доступа
const char *password = "64(Me4J6#C!gZfj"; // пароль точки доступа

const char *url = "http://192.168.4.2:8081/zeroconf/switch";

// ----------------------- ПИНЫ ---------------------------
#define DHT_TYPE DHT22            //тип датчика DHT22
#define DHT_VCC D6                //питание датчика DHT22
#define DHT_PIN D5                // дата линия датчика DHT22

int max_temp = 26;
int min_temp = 25;

bool valve_is_opened = true;

DHT dht(DHT_PIN, DHT_TYPE);       // обьявляем объект dht, указывать тип и дата пин 
WiFiClient client;                // объявить объект класса
HTTPClient http;                  // объявить объект класса

//настройки таймера задержки
unsigned long lastTime = 0;
unsigned long timerDelay = 5000; // 5 seconds (5000)

void setup()
{
  Serial.begin(115200);            // Запуск последовательной связи

  WiFi.softAP(ssid, password);     // WiFi.softAP используется для запуска режима AP NodeMCU.
  Serial.print("Access Point:");   // Выводим информацию через последовательный монитор
  Serial.println(ssid);            // Сообщаем пользователю имя WiFi, установленное NodeMCU
  Serial.print("IP-адрес:");       // И IP-адрес NodeMCU
  Serial.println(WiFi.softAPIP()); // IP-адрес NodeMCU можно получить, вызвав WiFi.softAPIP ()

  pinMode(DHT_PIN, INPUT);         // D5 пин в режиме входа 
  pinMode (DHT_VCC, OUTPUT);       // D6 пин в режиме выхода 
  digitalWrite (DHT_VCC, HIGH);    // подать напряжение 3,3V на D6 пин
  dht.begin();                     // инициализация DHT22 3

  http.begin(client, url);         // инициализация http клиента

  delay(5000);                       // для стабильности 
}

void close_valve () {
  // отправить запрос на включение реле
  http.addHeader("Content-Type", "application/json");                                  // header
  String RequestData = "{\"deviceid\":\"1000b91ec6\",\"data\":{\"switch\": \"on\"}}"; // payload
  int ResponseStatusCode = http.POST(RequestData);                                     // post запрос
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
  http.addHeader("Content-Type", "application/json");                                  // header
  String RequestData = "{\"deviceid\":\"1000b91ec6\",\"data\":{\"switch\": \"off\"}}"; // payload
  int ResponseStatusCode = http.POST(RequestData);                                     // post запрос
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

    http.end(); // Free resources
    lastTime = millis();
  }
}


