# Home Climate Control
![picture](https://doszhan-m.github.io/img/home_climate_control.png)

## Приложение для управления температурой в квартире на модуле esp8266 NodeMCU V3

### На модуле ESP8266 поднимается web сервер, который может обрабатывать запросы из браузера. Адаптация html страницы сделана только для мобильных устройств. Чтобы получить доступ к странице необходимо подключить ESP8266 к домашнему роутеру и в настройках роутера задать статичный ip адрес. Так же модуль поднимает свою точку доступа, можно зайти через нее. Реле от Sonoff должно подключаться к точке доступа от ESP8266. Главный функционал устройства - поддерживать температуру в заданном промежутке. Так же можно управлять 3-х позиционной кнопкой на самом устройстве: открыть - авто - закрыть. 


Примененные компоненты: 
1. ESP8266
2. DHT22
3. DS1307
4. Sonoff Basic R3 (DIY Mode)
5. Кнопка 3-х позиционная
6. Электрические термоприводы для радиаторов отопления

Чтобы изменить настройки для подключения wifi, можно отправить запрос по адресу:
http://your_local_ip/wifi_settings?ssid=TestWifi&password=SecretPass\
где, TestWifi название точки доступа, SecretPass - пароль

## Схема подключения:
![picture](https://doszhan-m.github.io/img/home_climate_schema.png)
