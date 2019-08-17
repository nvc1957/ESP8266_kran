/* Created by NVC*/


#include <ESP8266WiFi.h>                      // Подключаем библиотеку ESP8266WiFi

const char* ssid = "Keenetic-0064";     // Название Вашей WiFi сети
const char* password = "5NSwXHrx";// Пароль от Вашей WiFi сети

int value = HIGH;
int KL   =  0;
int connectAP = 0;
int connectAP_Memory =0;
int kolichestvo_perepodklucheniy_k_roytery = 0;
int kolichestvo_perepodklucheniy_k_roytery_chasov = 0;
int timer2 = 0;
int timer3 = 0;

int kolichestvo_minut_posle_zapuska_poliva = 0;
int kolichestvo_minut_posle_otklucheniya_poliva = 0;
int kolichestvo_chasov_posle_zapuska_poliva = 0;
int kolichestvo_chasov_posle_otklucheniya_poliva = 0;

int perecluchatel = 3;
long int timer = 0;
bool Kluch = true;

#define RELAY 2                               // Пин к которому подключен датчик
WiFiServer server(80);                        // Указываем порт Web-сервера

void setup() {
  
  Serial.begin(115200);                       // Скорость передачи 115200
  pinMode(RELAY, OUTPUT);                     // Указываем вывод RELAY как выход
  digitalWrite(RELAY, HIGH);                   // Устанавливаем RELAY в LOW (0В)
  Serial.println();                           // Печать пустой строки
  Serial.print("Connecting to ");             // Печать "Подключение к:"
  Serial.println(ssid);                       // Печать "Название Вашей WiFi сети"
  Serial.println("Setting soft-AP_ESPs_01_41 ... ");
  Serial.println("PASS  ---   1111111111");
  boolean result = WiFi.softAP("ESPs_01_41", "1111111111");
  if(result == true)
  {
    Serial.println("Ready");
  }
  else
  {
    Serial.println("Failed!");
  }

  WiFi.begin(ssid, password);                 // Подключение к WiFi Сети

  while (WiFi.status() != WL_CONNECTED)       // Проверка подключения к WiFi сети
  {
    delay(500);                               // Пауза 500 мкс
    Serial.print(".");                        // Печать "."
  }
  Serial.println("");                         // Печать пустой строки
  Serial.println("WiFi connected");           // Печать "WiFi connected"

  server.begin();                             // Запуск сервера
  Serial.println("Server started");           // Печать "Server starte"
  Serial.print("Use this URL to connect: ");  // Печать "Use this URL to connect:"
  Serial.print(WiFi.localIP());               // Печать выданого IP адресса
  Serial.println("/");                        // Печать "/"
}

void loop() {
  connectAP = WiFi.softAPgetStationNum();
  if( connectAP >> 0 && KL==0){
  Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  connectAP_Memory = connectAP;
  KL=1;
  }
 if(connectAP != connectAP_Memory && KL==1){
   Serial.printf("Stations DISconnected = %d\n", WiFi.softAPgetStationNum());
  KL=0;
 }
  
  timer = millis();
  if (((timer - timer2) % 60000 == 0 || (timer - timer2) % 60000 == 1 || (timer - timer2) % 60000 == 2 || (timer - timer2) % 60000 == 3) && perecluchatel == 2 && Kluch == true) {
    kolichestvo_minut_posle_zapuska_poliva++;
    Serial.println(kolichestvo_minut_posle_zapuska_poliva);
    Serial.println(timer);
    Kluch = false;
    if (kolichestvo_minut_posle_zapuska_poliva == 60) {
      kolichestvo_minut_posle_zapuska_poliva = 0;
      kolichestvo_chasov_posle_zapuska_poliva++;
    }
  }
  if (((timer - timer3) % 60000 == 0 || (timer - timer3) % 60000 == 1 || (timer - timer3) % 60000 == 2 || (timer - timer3) % 60000 == 3) && perecluchatel == 1 && Kluch == true) {
    kolichestvo_minut_posle_otklucheniya_poliva++;
    Serial.println(kolichestvo_minut_posle_otklucheniya_poliva);
    Serial.println(timer);
    Kluch = false;
    if (kolichestvo_minut_posle_otklucheniya_poliva == 60) {
      kolichestvo_minut_posle_otklucheniya_poliva = 0;
      kolichestvo_chasov_posle_otklucheniya_poliva++;
    }
  }
  if (((timer - timer3) % 60000 == 7 || (timer - timer3) % 60000 == 8 || (timer - timer3) % 60000 == 9 || (timer - timer3) % 60000 == 10) && perecluchatel == 1 && Kluch == false) {
    Kluch = true;
  }
  if (((timer - timer2) % 60000 == 7 || (timer - timer2) % 60000 == 8 || (timer - timer2) % 60000 == 9 || (timer - timer2) % 60000 == 10) && perecluchatel == 2 && Kluch == false) {
    Kluch = true;
  }
  if (timer % 60000 == 0 || timer % 60000 == 1 || timer % 60000 == 2 || timer % 60000 == 3) {
    kolichestvo_perepodklucheniy_k_roytery++;
    if (kolichestvo_perepodklucheniy_k_roytery == 60) {
      kolichestvo_perepodklucheniy_k_roytery = 0;
      kolichestvo_perepodklucheniy_k_roytery_chasov++;
    }
    WiFi.begin(ssid, password);                 // Подключение к WiFi Сети

    while (WiFi.status() != WL_CONNECTED)       // Проверка подключения к WiFi сети
    {
      delay(500);                               // Пауза 500 мкс
      Serial.print(".");                        // Печать "."
    }
    Serial.println("");                         // Печать пустой строки
    Serial.println("WiFi connected");           // Печать "WiFi connected"

    server.begin();                             // Запуск сервера
    Serial.println("Server started");           // Печать "Server starte"
    Serial.print("Use this URL to connect: ");  // Печать "Use this URL to connect:"
    Serial.print(WiFi.localIP());               // Печать выданого IP адресса
    Serial.println("/");                        // Печать "/"
  }
  WiFiClient client = server.available();    // Получаем данные, посылаемые клиентом
  if (!client)
  {
    return;
  }
  Serial.println("new client");               // Отправка "new client"
  while (!client.available())                 // Пока есть соединение с клиентом
  {
    delay(1);                                 // пауза 1 мс
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();


  if (request.indexOf("/RELAY=ON") != -1)
  {
    Serial.println("RELAY=ON");
    digitalWrite(RELAY, LOW);
    value = LOW;
    if (perecluchatel == 1 || perecluchatel == 3) {
      kolichestvo_minut_posle_zapuska_poliva = 0;
      kolichestvo_chasov_posle_zapuska_poliva = 0;
      timer2 = millis();
      perecluchatel = 2;
    }
  }
  if (request.indexOf("/RELAY=OFF") != -1)
  {
    Serial.println("RELAY=OFF");
    digitalWrite(RELAY, HIGH);
    value = HIGH;
    if (perecluchatel == 2 || perecluchatel == 3) {
      kolichestvo_minut_posle_otklucheniya_poliva = 0;
      kolichestvo_chasov_posle_otklucheniya_poliva = 0;
      timer3 = millis();
      perecluchatel = 1;
    }

  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><title>Kran1</title></head>");
  client.println("<style>  body{width:100vw;height:100vh;background-color: #696969;} #container { font-size: 25px;color:#FFFFFF;  } #container2 { font-size: 25px;color:#FFFFFF;  } #container3 { font-size: 25px;color:#FFFFFF;  } #container4 { font-size: 25px;color:#FFFFFF;}.onoffswitch {position: relative; width: 200px; -webkit-user-select:none; -moz-user-select:none; -ms-user-select: none;}.onoffswitch-checkbox {display: none;}.onoffswitch-label {    display: block; overflow: hidden; cursor: pointer;    border: 2px solid #999999; border-radius: 50px;}.onoffswitch-inner {    display: block; width: 200%; margin-left: -100%; transition: margin 0.3s ease-in 0s;}.onoffswitch-inner:before, .onoffswitch-inner:after { display: block; float: left; width: 50%; height:75px; padding: 0; line-height: 75px;font-size: 40px; color: white; font-family: Trebuchet, Arial, sans-serif; font-weight: bold; box-sizing: border-box;}.onoffswitch-inner:before {    content: \"ON\"; padding-right: 90px;background-color: #FF0000; color: #FFFFFF;}.onoffswitch-inner:after {    content: \"OFF\";    padding-right: 20px;  background-color: #00FF33; color: #FFF7F7;    text-align: right;}.onoffswitch-switch { display: block; width: 70px; margin: 2.5px;background: #FFFFFF;position: absolute; top: 0; bottom: 0;right: 121px;border: 2px solid #999999; border-radius: 50px;transition: all 0.3s ease-in 0s; }.onoffswitch-checkbox:checked + .onoffswitch-label .onoffswitch-inner {margin-left: 0;}.onoffswitch-checkbox:checked + .onoffswitch-label .onoffswitch-switch {right: 0px; } #blink { margin-bottom: 5px; font-size: 40px;color:#FFFFFF; -webkit-animation: blink 2s linear infinite;  animation: blink 2s linear infinite;}@-webkit-keyframes blink {  50% { color: rgb(255, 255, 255); }  51% { color: rgba(255, 255, 255, 0); }  100% { color: rgba(255, 255, 255, 0); }}@keyframes blink {  50% { color: rgb(255, 255, 255); }  51% { color: rgba(255, 255, 255, 0); } 100% { color: rgba(255, 255, 255, 0); }</style>");
  client.println("<center>");
  client.print("<p style=\"font-size: 30px; color: #0000FF; font-style: italic;\">Watering is:</p> ");

  if (value == HIGH)
  {
    client.println("<p style =\"font-size: 40px;color:#00FF00;margin-bottom: 5px;\">OFF</p>");
    client.println("<br>");
    client.println("<div class=\"onoffswitch\">");
    client.println("<input type=\"checkbox\" name=\"onoffswitch\" class=\"onoffswitch-checkbox\" id=\"myonoffswitch\"/>");
    client.println("<label class=\"onoffswitch-label\" for=\"myonoffswitch\">");
    client.println("<span class=\"onoffswitch-inner\"></span>");
    client.println("<span class=\"onoffswitch-switch\"></span>");
    client.println("</label>");
    client.println("</div>");
    client.println("<div id=\"container3\">");
    client.print("<p>Time from stop watering: ");
    client.print(kolichestvo_chasov_posle_otklucheniya_poliva);
    client.print(" hours, ");
    client.print(kolichestvo_minut_posle_otklucheniya_poliva);
    client.println(" minutes.</p>");
    client.println("</div>");
    if (perecluchatel == 1) {

      client.println("<div id=\"container4\">");
      client.print("<p>Last watering duration: ");
      client.print(kolichestvo_chasov_posle_zapuska_poliva);
      client.print(" hours, ");
      client.print(kolichestvo_minut_posle_zapuska_poliva);
      client.println(" minutes.</p>");
      client.println("</div>");
    }
  }
  else
  {
    client.println("<p style =\"font-size: 40px;color:#FF0000;margin-bottom: 5px;\">ON</p>");
    client.println("<br>");
    client.println("<div class=\"onoffswitch\">");
    client.println("<input type=\"checkbox\" name=\"onoffswitch\" class=\"onoffswitch-checkbox\" id=\"myonoffswitch\" checked/>");
    client.println("<label class=\"onoffswitch-label\" for=\"myonoffswitch\">");
    client.println("<span class=\"onoffswitch-inner\"></span>");
    client.println("<span class=\"onoffswitch-switch\"></span>");
    client.println("</label>");
    client.println("</div>");
    client.println("<div id=\"container2\">");
    client.print("<p>Time from start watering: ");
    client.print(kolichestvo_chasov_posle_zapuska_poliva);
    client.print(" hours, ");
    client.print(kolichestvo_minut_posle_zapuska_poliva);
    client.println(" minutes.</p>");
    client.println("</div>");
  }
  client.println("<div id=\"container\">");
  client.print("<p>Time from start working device: ");
  client.print(kolichestvo_perepodklucheniy_k_roytery_chasov);
  client.print(" hours, ");
  client.print(kolichestvo_perepodklucheniy_k_roytery);
  client.println(" minutes.</p>");
  client.println("</div>");

  client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js\"></script>");
  client.println("<script>");
  client.println("var c = document.querySelector('#myonoffswitch');c.onclick = function() {if (c.checked) {location.href = \"/RELAY=ON\";}else {location.href = \"/RELAY=OFF\";}}");
  client.println("</script>");
  client.println("</center>");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
