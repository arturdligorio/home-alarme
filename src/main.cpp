#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#include "./pages.h"

#define ESP12_LED 2

#define SENSOR_PIN 10

#define DFPLAYER_RX 5
#define DFPLAYER_TX 16

#define LED_PIN 4
#define LED_COUNT 30

#define COR_AMARELA 155, 155, 0
#define COR_VERDE 0, 155, 0
#define COR_VERMELHA 155, 0, 0
#define COR_LARANJA 155, 70, 0

#define SMTP_PORT 2525
#define SMTP_SERVER "mail.smtp2go.com"
#define SMTP_USER "home.alarme.iot@gmail.com"
#define SMTP_PASS "homeiot1020"

#define TRIGGER_IFTTT "alarme_disparado"
#define KEY_IFTTT "i0PQ4IxUb9hQPgfonA9YcXN-DcMwDdhFnJTY2tLMbKH"
#define HOST_IFTTT "maker.ifttt.com"
#define PORT_IFTTT 443
#define FINGERPRINTS_IFTTT "AA 75 CB 41 2E D5 F9 97 FF 5D A0 8B 7D AC 12 21 08 4B 00 8C"

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

SoftwareSerial mySoftwareSerial(5, 16); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

ESP8266WebServer server(80);

HTTPClient http;

const char *ssid_c = "ESP8266 AP";
const char *pass_c = "10203040";

const char *DNS_WIFI = "espiot";

boolean connectedNewWifi = false;
boolean timeOutNewConnectionWifi = true;
String stateConnectWifi = "desconectado";
boolean sistemaAutonoLigado = true;

boolean sensorMovimento = false;

void inicializaDFPlayer()
{

  mySoftwareSerial.begin(9600);
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(30);
  myDFPlayer.EQ(0);
}

int getNumberMuisicDFPlayer()
{
  return myDFPlayer.readFileCounts(DFPLAYER_DEVICE_SD);
}

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

void ascendeLedByColor(int r, int g, int b)
{
  int i = LED_COUNT;
  for (i = 0; i < LED_COUNT; i++)
  {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();
  }
}

void piscaLedInicio()
{

  //cor amarela
  ascendeLedByColor(COR_AMARELA);
  delay(250);
  pixels.clear();
  delay(250);
  ascendeLedByColor(COR_AMARELA);
  delay(250);
  pixels.clear();
  delay(250);
  ascendeLedByColor(COR_AMARELA);
  delay(250);
  pixels.clear();
  delay(250);
}

String sendNotifyIFTTT()
{

  String response = "error";

  if ((WiFi.status() == WL_CONNECTED))
  {

    String url = "https://maker.ifttt.com/trigger/";
    url = url + TRIGGER_IFTTT;
    url = url + "/with/key/";
    url = url + KEY_IFTTT;

    http.begin(url, FINGERPRINTS_IFTTT);
    int httpCode = http.GET();

    if (httpCode > 0)
    {
      response = http.getString();
      Serial.println(httpCode);
      Serial.println(response);

      return response;
    }
    else
    {
      Serial.println("Error on HTTP request");
      delay(1000);
      sendNotifyIFTTT();
    }

    http.end();
  }

  return response;
}

void startWifiAP()
{

  digitalWrite(ESP12_LED, HIGH);

  WiFi.softAP(ssid_c, pass_c);
  Serial.print("Access Point \"");
  Serial.print(ssid_c);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());

  piscaLedInterno();
  piscaLedInterno();
  piscaLedInterno();
  piscaLedInterno();
}

void connectWiFI(String ssid, String password)
{
  connectedNewWifi = false;
  boolean conectou = true;
  int timeOut = 0;

  WiFi.disconnect(true);
  delay(200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("*");
    timeOut += 1;
    delay(500);
    stateConnectWifi = "conectando";
    if (timeOut >= 30)
    {
      Serial.println("\nTimeOut para conexão WIFI.");
      timeOutNewConnectionWifi = true;
      connectedNewWifi = false;
      stateConnectWifi = "timeout";
      conectou = false;
      break;
    }
  }

  if (conectou)
  {
    digitalWrite(ESP12_LED, HIGH);
    timeOutNewConnectionWifi = false;
    connectedNewWifi = true;
    stateConnectWifi = "conectado";
    piscaLedInterno();
    piscaLedInterno();
    piscaLedInterno();

    Serial.println("\nWiFi connection Successful");
    Serial.print("The IP Address of ESP8266 Module is: ");
    Serial.println(WiFi.localIP());
  }
}

void handlePageHome()
{
  server.send(200, "text/html", String(HOME_PAGE));
}

void handleStatus()
{
  String s = "on";
  server.send(200, "text/html", s);
}

void handlePageConfigWifi()
{
  server.send(200, "text/html", String(WIFI_CONNECT_PAGE));
}

void handleConnectWifi()
{

  String ssid_connect = server.arg("ssid_wifi");
  String password_connect = server.arg("pass_wifi");

  Serial.println("\nSSID: " + ssid_connect + "\tPass: " + password_connect);

  server.send(200, "text/plain", "ok");
  connectWiFI(ssid_connect, password_connect);
}

void handleStatusConnectWifi()
{
  String mensagem = "";
  String ipString = "0.0.0.0";

  if (WiFi.status() == WL_CONNECTED)
  {
    IPAddress ip = WiFi.localIP();
    ipString = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
  }

  Serial.println("\nLocal ip: " + ipString);

  if (timeOutNewConnectionWifi && !connectedNewWifi && stateConnectWifi != "conectado")
  {

    mensagem = stateConnectWifi;
    Serial.println(mensagem);
    server.send(200, "text/plain", mensagem);
  }
  else if (connectedNewWifi && stateConnectWifi == "conectado")
  {
    mensagem = ipString;
    Serial.println(mensagem);
    server.send(200, "text/plain", mensagem);
  }
}

void handleStateMovimento()
{
  if (sensorMovimento)
    server.send(200, "text/plain", "true");
  else
    server.send(200, "text/plain", "false");
}

void handlePageMonitor()
{
  server.send(200, "text/html", String(MOVIMENTO_MONITORING_PAGE));
}

void handleSendNotyfi()
{

  String resp = sendNotifyIFTTT();

  server.send(200, "text/html", resp);
}

void handleNotFound()
{

  String message = "File Not Found\n\n";

  server.send(404, "text/plain", message);
}

void leSensorMovimento()
{
  if (sistemaAutonoLigado)
  {
    int acionamento = digitalRead(SENSOR_PIN);
    if (acionamento == LOW)
    {
      sensorMovimento = false;
      ascendeLedByColor(COR_VERDE);
    }
    else
    {
      sensorMovimento = true;
      ascendeLedByColor(COR_VERMELHA);
    }
  }
  else
  {

    sensorMovimento = false;
    ascendeLedByColor(COR_LARANJA);
  }
}

void setup()
{

  ESP.eraseConfig();
  ESP.resetFreeContStack();


  pinMode(ESP12_LED, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  inicializaDFPlayer();
  Serial.begin(9600);
  Serial.println();

  WiFi.disconnect(true);
  startWifiAP();

  server.on("/wifi", HTTP_GET, handlePageConfigWifi);
  server.on("/home", HTTP_GET, handlePageHome);
  server.on("/monitor", HTTP_GET, handlePageMonitor);
  server.on("/status", HTTP_GET, handleStatus);
  server.on("/config/wifi", HTTP_POST, handleConnectWifi);
  server.on("/status/wifi/state", HTTP_GET, handleStatusConnectWifi);
  server.on("/state/movimento", HTTP_GET, handleStateMovimento);

  server.on("/send/notyfi", HTTP_POST, handleSendNotyfi);

  server.onNotFound(handleNotFound);
  server.begin();
  pixels.begin();
}

void loop()
{
  server.handleClient();
  leSensorMovimento();
  delay(500);
}