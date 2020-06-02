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

#define DFPLAYER_RX D5
#define DFPLAYER_TX D6
#define TEMPO_MIN_ALARME 60000

#define LED_PIN 4
#define LED_COUNT 30

#define NO_COLOR 0, 0, 0
#define COR_AMARELA 155, 155, 0
#define COR_VERDE 0, 155, 0
#define COR_VERMELHA 155, 0, 0
#define COR_LARANJA 155, 70, 0
#define COR_ROSA 204, 10, 152

#define COR_ON 0, 155, 0
#define COR_OFF 155, 0, 0

const char *trigger_ifttt = "alarme_disparado";
const char *key_ifttt = "i0PQ4IxUb9hQPgfonA9YcXN-DcMwDdhFnJTY2tLMbKH";
const char *host_ifttt = "maker.ifttt.com";
int port_ifttt = 443;
const char *fingerprints_ifttt = "AA 75 CB 41 2E D5 F9 97 FF 5D A0 8B 7D AC 12 21 08 4B 00 8C";

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

SoftwareSerial mySoftwareSerial(DFPLAYER_RX, DFPLAYER_TX); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

ESP8266WebServer server(80);

HTTPClient http;

const char *ssid_c = "ESP8266 AP";
const char *pass_c = "10203040";

long time_repro_init = 0;
long time_alarme_disparado = 0;
int numero_musicas = 0;
boolean connectedNewWifi = false;
boolean timeOutNewConnectionWifi = true;
String stateConnectWifi = "desconectado";
boolean sistemaAutonoLigado = true;
boolean desligouManualmenteAlarme = false;
boolean sensorMovimento = false;

int getNumberMuisicDFPlayer()
{
  int num_dfp = 0;
  Serial.println("[DEBUG] GET COUNT MUSICAS\n");

  num_dfp = myDFPlayer.readFileCounts(DFPLAYER_DEVICE_SD);

  //num_dfp = 5;

  Serial.print("\nNumero de musicas..: ");
  Serial.println(num_dfp);
  return num_dfp;
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

void ledPercorreFita(int r, int g, int b)
{
  int i = LED_COUNT;
  int apaga = 0;
  int tamanho_led_aceso = 7;

  pixels.clear();
  ascendeLedByColor(NO_COLOR);

  for (i = 0; i < LED_COUNT; i++)
  {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();

    apaga = i - tamanho_led_aceso;
    if (apaga >= 0)
      pixels.setPixelColor(apaga, pixels.Color(NO_COLOR));

    //TODO: ajustar o delay para percorre na corrente
    delay(40);
  }

  pixels.clear();
  ascendeLedByColor(NO_COLOR);

  int ret = LED_COUNT;
  for (ret = LED_COUNT; ret >= 0; ret--)
  {
    pixels.setPixelColor(ret, pixels.Color(r, g, b));
    pixels.show();

    apaga = ret + tamanho_led_aceso;

    if (apaga <= LED_COUNT)
      pixels.setPixelColor(apaga, pixels.Color(NO_COLOR));

    //TODO: ajustar o delay para percorre na corrente
    delay(40);
  }
  pixels.clear();
  ascendeLedByColor(NO_COLOR);
}

String sendNotifyIFTTT()
{

  String response = "error";
  Serial.println("[DEBUG] Enviando notificação via IFTTT.");

  if ((WiFi.status() == WL_CONNECTED))
  {

    String url = "http://maker.ifttt.com/trigger/";
    url = url + trigger_ifttt;
    url = url + "/with/key/";
    url = url + key_ifttt;

    int httpCode = 0;
    http.begin(url);
    httpCode = http.GET();

    Serial.println("[DEBUG] Notificação enviada via IFTTT.");

    if (httpCode > 0)
    {
      response = http.getString();
      Serial.print("[INFO] Notificação enviada: ");
      Serial.print(httpCode);
      Serial.print(" : ");
      Serial.println(response);
    }
    else
    {
      Serial.println("[ERROR] Erro ao enviar notificação IFTTT.");
    }

    http.end();
    return response;
  }

  Serial.println("[DEBUG] Não Conectado no WIFI.");

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

  String hp = HOME_PAGE;

  if (sistemaAutonoLigado)
  {
    hp.replace("#184a07", "green");
  }
  else
  {
    hp.replace("#184a07", "red");
  }
  server.send(200, "text/html", hp);
}

void handleStatus()
{
  String resp = "on";

  if (sistemaAutonoLigado)
  {
    resp = "on";
  }
  else
  {

    resp = "off";
  }

  server.send(200, "text/html", resp);
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

void handleTurnOnOff()
{

  String resp = "";

  if (sistemaAutonoLigado)
  {
    sistemaAutonoLigado = false;
    ledPercorreFita(COR_OFF);
    resp = "off";
  }
  else
  {

    sistemaAutonoLigado = true;
    ledPercorreFita(COR_ON);
    resp = "on";
  }

  server.send(200, "text/html", resp);
}

void handleStopAlarme()
{

  String response = "";
  if (sensorMovimento)
  {
    ascendeLedByColor(NO_COLOR);
    myDFPlayer.stop();
    desligouManualmenteAlarme = true;
    response = "parou";
    sensorMovimento = false;
  }
  else
  {

    response = "alarme not active";
  }

  server.send(200, "text/plain", response);
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
    long now_low = millis();
    long limit_low = TEMPO_MIN_ALARME;

    if (acionamento == LOW)
    {
      if (now_low - time_alarme_disparado >= limit_low)
      {
        sensorMovimento = false;
        ascendeLedByColor(NO_COLOR);
        myDFPlayer.stop();
      }
    }
    else
    {
      long limit = TEMPO_MIN_ALARME;
      long now = millis();

      if ((now - time_repro_init >= limit) || desligouManualmenteAlarme || time_repro_init == limit)
      {
        int p_num = 0;
        sensorMovimento = true;
        ascendeLedByColor(COR_VERMELHA);

        p_num = rand() % numero_musicas + 1;
        if (p_num > numero_musicas)
        {
          p_num = numero_musicas;
        }

        Serial.print("[INFO] Reproduzindo a musica: ");
        Serial.println(p_num);
        myDFPlayer.play(p_num);
        time_repro_init = millis();
        time_alarme_disparado = millis();
        desligouManualmenteAlarme = false;

        sendNotifyIFTTT();
      }
    }
  }
  else
  {

    sensorMovimento = false;
    ascendeLedByColor(NO_COLOR);
  }
}

void setup()
{

  ESP.eraseConfig();
  ESP.resetFreeContStack();

  Serial.begin(9600);

  Serial.println("");
  Serial.println("Iniciando DFplayer");

  mySoftwareSerial.begin(9600);
  myDFPlayer.begin(mySoftwareSerial);
  numero_musicas = getNumberMuisicDFPlayer();
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(30);
  myDFPlayer.EQ(0);
  myDFPlayer.stop();

  pinMode(ESP12_LED, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  ascendeLedByColor(NO_COLOR);
  ledPercorreFita(COR_ON);

  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect(true);
  startWifiAP();

  server.on("/", HTTP_GET, handlePageHome);
  server.on("/home", HTTP_GET, handlePageHome);
  server.on("/monitor", HTTP_GET, handlePageMonitor);
  server.on("/wifi", HTTP_GET, handlePageConfigWifi);

  server.on("/onoff", HTTP_POST, handleTurnOnOff);

  server.on("/status", HTTP_GET, handleStatus);
  server.on("/config/wifi", HTTP_POST, handleConnectWifi);
  server.on("/status/wifi/state", HTTP_GET, handleStatusConnectWifi);
  server.on("/state/movimento", HTTP_GET, handleStateMovimento);
  server.on("/send/notyfi", HTTP_POST, handleSendNotyfi);
  server.on("/stop/alarme", HTTP_POST, handleStopAlarme);

  server.onNotFound(handleNotFound);
  server.begin();
  pixels.begin();
  time_repro_init = TEMPO_MIN_ALARME;
}

void loop()
{
  server.handleClient();
  leSensorMovimento();
  delay(250);
}