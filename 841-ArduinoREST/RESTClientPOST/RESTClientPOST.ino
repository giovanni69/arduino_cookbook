#include "WiFiS3.h"
#include <ArduinoJson.h>
#include "arduino_secrets.h" 

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
char server[] = "192.168.1.17";  
const int port = 5150;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(1); }

  while (status != WL_CONNECTED) {
    Serial.print(".");
    status = WiFi.begin(ssid, pass);
    delay(3000);
  }

  if (client.connect(server, port)) {
    Serial.print("connesso");
    //POST
    sendPost();
  }

}

void loop() {
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    delay(3000);
    Serial.println("disconnecting from Wifi.");
    WiFi.disconnect();

    // do nothing forevermore:
    while (true);
  }
}

void sendPost() {

  const char* postData = "{\"id\": 10, \"name\": \"Italy\", \"capital\": \"Roma\", \"area\": 123123} ";

  client.println("POST /country HTTP/1.1");
  client.print("Host: ");client.println(server);
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");client.println(strlen(postData));
  client.println();
  client.print(postData);
  delay(200);

  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
}