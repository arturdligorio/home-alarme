#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>

#include "./pages.h"

#define ESP12_LED 2

#define LED_PIN 6
#define LED_COUNT 60
#define BRIGHTNESS 50

ESP8266WebServer server(80);

const char *ssid_c = "ESP8266 Access Point";

const char *DNS_WIFI = "espiot";

void iniciaComponentes()
{
}

void piscaLedInterno()
{
  digitalWrite(ESP12_LED, LOW);
  delay(200);
  digitalWrite(ESP12_LED, HIGH);
  delay(200);
}

void startWifiAP()
{

  digitalWrite(ESP12_LED, HIGH);

  WiFi.softAP(ssid_c);
  Serial.print("Access Point \"");
  Serial.print(ssid_c);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());

  piscaLedInterno();
  piscaLedInterno();
  piscaLedInterno();
  piscaLedInterno();
  
  if (MDNS.begin(DNS_WIFI))
  {
    Serial.print("DNS na rede com o nome de:\t");
    Serial.println(DNS_WIFI);
  }

  MDNS.addService("http", "tcp", 80);
}

boolean connectWiFI(String ssid, String password)
{
  digitalWrite(ESP12_LED, HIGH);
  int timeOut = 0;

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("*");
    timeOut += 1;

    if (timeOut >= 30)
    {
      Serial.println("\nTimeOut para conexão WIFI.");

      digitalWrite(ESP12_LED, LOW);
      delay(3000);
      digitalWrite(ESP12_LED, HIGH);
      return false;
    }
  }

  piscaLedInterno();
  piscaLedInterno();
  piscaLedInterno();

  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.print("The IP Address of ESP8266 Module is: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin(DNS_WIFI))
  {
    Serial.print("DNS na rede com o nome de:\t");
    Serial.println(DNS_WIFI);
  }

  MDNS.addService("http", "tcp", 80);
  return true;
}

void handleRoot()
{
  server.send(200, "text/html", "<h1>Funcionou</h1>");
}

void configWifi()
{

  String s = WIFI_CONNECT_PAGE;
  server.send(200, "text/html", s);
}

void connectWifiHandle(){

  String ssid_connect = server.arg("ssid_wifi");
  String password_connect = server.arg("pass_wifi");
  String menssagem = "";

  if (connectWiFI(ssid_connect, password_connect))
  {
    IPAddress ip = WiFi.localIP();
    String ipString = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    menssagem = ipString;

  }else{

    menssagem = "0.0.0.0";
  }
  
  Serial.println(menssagem);
  server.send(200, "text/plain", menssagem);

}

void handleNotFound()
{

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup()
{
  // if(SPIFFS.begin() ? Serial.println("SPIFFS Initialize....ok") : Serial.println("SPIFFS Initialization...failed"));

  // if(SPIFFS.format() ? Serial.println("File System Formated") : Serial.println("File System Formatting Error"));

  pinMode(ESP12_LED, OUTPUT);

  Serial.begin(9600);
  Serial.println();

  //connectWiFI(ssid, password);

  startWifiAP();

  server.on("/wifi", configWifi);

  server.on("/config/wifi", connectWifiHandle);

  server.on("/inline", []() {
    server.send(200, "text/plain", "<h1>this works as well</h1>");
  });

  server.onNotFound(handleNotFound);
  server.begin();
}

void loop()
{
  MDNS.update();
  server.handleClient();

  delay(1000);
  digitalWrite(ESP12_LED, LOW);

  delay(1000);
  digitalWrite(ESP12_LED, HIGH);
}