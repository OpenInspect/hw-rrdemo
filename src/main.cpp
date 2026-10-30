#include <WiFi.h>
#include <WebServer.h>

#define WIFI_SSID "utexas-iot"
#define WIFI_PASS "REDACTED"

// A4988 pins: {STEP, DIR, EN}
const int PINS[2][3] = {
  {4, 5, 6},  // motor 1
  {7, 1, 12}, // motor 2
};
const uint32_t STEP_US = 1000; // 1 kHz
long pos[2] = {0, 0};
WebServer server(80);

const char* PAGE = R"rawliter(<html><head><title>Steppers</title>
<style>body{font-family:sans-serif}table{border-collapse:collapse;margin:1em}
td,th{border:1px solid #888;padding:0.8em;font-size:1.5em}</style></head><body>
<h1>Stepper Test</h1><table>
<tr><th>Motor 1</th><td><a href="/s?m=0&n=-100">-100</a></td><td><a href="/s?m=0&n=-10">-10</a></td><td><a href="/s?m=0&n=-1">-1</a></td><td><a href="/s?m=0&n=1">+1</a></td><td><a href="/s?m=0&n=10">+10</a></td><td><a href="/s?m=0&n=100">+100</a></td></tr>
<tr><th>Motor 2</th><td><a href="/s?m=1&n=-100">-100</a></td><td><a href="/s?m=1&n=-10">-10</a></td><td><a href="/s?m=1&n=-1">-1</a></td><td><a href="/s?m=1&n=1">+1</a></td><td><a href="/s?m=1&n=10">+10</a></td><td><a href="/s?m=1&n=100">+100</a></td></tr>
</table></body></html>)rawliter";

void step(int m, long n) {
  digitalWrite(PINS[m][1], n >= 0);
  for (long i = 0; i < abs(n); i++) {
    digitalWrite(PINS[m][0], HIGH);
    delayMicroseconds(STEP_US / 2);
    digitalWrite(PINS[m][0], LOW);
    delayMicroseconds(STEP_US / 2);
  }
  pos[m] += n;
}

void setup() {
  Serial.begin(115200);
  for (int m = 0; m < 2; m++) {
    pinMode(PINS[m][0], OUTPUT);
    pinMode(PINS[m][1], OUTPUT);
    pinMode(PINS[m][2], OUTPUT);
    digitalWrite(PINS[m][2], LOW); // A4988 EN is active low
  }
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, [] { server.send(200, "text/html", PAGE); });
  server.on("/s", HTTP_GET, [] {
    int m = server.arg("m").toInt();
    long n = server.arg("n").toInt();
    if (m >= 0 && m < 2) step(m, n);
    server.sendHeader("Location", "/", true);
    server.send(302);
  });
  server.begin();
}

void loop() { server.handleClient(); }
