#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <LittleFS.h>  

const char *ssid = "Najwan";
const char *password = "12345678";  

ESP8266WebServer server(80);
DNSServer dnsServer;


void handleRoot() {
  File file = LittleFS.open("/index.html", "r");
  if (!file) {
    server.send(404, "text/plain", "File tidak ditemukan di LittleFS");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}


void handleAudio() {
  File file = LittleFS.open("/laughx.mp3", "r");
  if (!file) {
    server.send(404, "text/plain", "File audio tidak ditemukan");
    return;
  }
  server.streamFile(file, "audio/mpeg");
  file.close();
}


void handleRedirect() {
  server.sendHeader("Location", "http://192.168.4.1/", true);
  server.send(302, "text/plain", "Redirecting...");
}

void setup() {

  if (!LittleFS.begin()) {
    Serial.println("Gagal memulai LittleFS. Pastikan data sudah diupload!");
    return;
  } else {
    Serial.println("LittleFS berhasil dimulai");
  }


  Serial.begin(115200);
  delay(1000);


  if (!LittleFS.begin()) {
    Serial.println("Gagal memulai LittleFS");
    return;
  }
  Serial.println("LittleFS berhasil dimulai");


  WiFi.softAP(ssid, password);
  Serial.println("WiFi AP aktif.");


  Serial.print("IP ESP8266: ");
  Serial.println(WiFi.softAPIP());


  dnsServer.start(53, "*", WiFi.softAPIP());


  server.on("/", handleRoot);
  server.on("/laughx.mp3", handleAudio);  
  server.onNotFound(handleRedirect);
  server.begin();
  Serial.println("Server HTTP dimulai!");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
