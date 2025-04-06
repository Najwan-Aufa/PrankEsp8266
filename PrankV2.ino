#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <LittleFS.h>  // Gunakan LittleFS

const char *ssid = "Najwan";
const char *password = "12345678";  // Kosongkan jika tidak ingin password

ESP8266WebServer server(80);
DNSServer dnsServer;

// Fungsi untuk menangani halaman utama
void handleRoot() {
  File file = LittleFS.open("/index.html", "r");
  if (!file) {
    server.send(404, "text/plain", "File tidak ditemukan di LittleFS");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}

// Fungsi untuk menangani file audio
void handleAudio() {
  File file = LittleFS.open("/laughx.mp3", "r");
  if (!file) {
    server.send(404, "text/plain", "File audio tidak ditemukan");
    return;
  }
  server.streamFile(file, "audio/mpeg");
  file.close();
}

// Fungsi untuk menangani redirect
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

  // Mulai LittleFS
  if (!LittleFS.begin()) {
    Serial.println("Gagal memulai LittleFS");
    return;
  }
  Serial.println("LittleFS berhasil dimulai");

  // Mulai WiFi AP
  WiFi.softAP(ssid, password);
  Serial.println("WiFi AP aktif.");

  // Tampilkan IP
  Serial.print("IP ESP8266: ");
  Serial.println(WiFi.softAPIP());

  // Mengatur DNS Server agar mengarahkan ke ESP8266
  dnsServer.start(53, "*", WiFi.softAPIP());

  // Mengatur server HTTP
  server.on("/", handleRoot);
  server.on("/laughx.mp3", handleAudio);  // Endpoint untuk file audio
  server.onNotFound(handleRedirect);
  server.begin();
  Serial.println("Server HTTP dimulai!");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
