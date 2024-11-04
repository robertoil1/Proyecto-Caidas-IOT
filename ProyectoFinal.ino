#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <math.h>
#include <stdio.h>

// Credenciales WiFi
const char* ssid = "LoremIpsum";
const char* password = "woim9701";

// Configuración de ThingsBoard
const char* mqtt_server = "iot.ceisufro.cl";
const int mqtt_port = 1883;
const char* access_token = "bket293EjRcPjaZp9NT6";
const char* device_ID = "0bc94150-9abd-11ef-8d2d-4d85475624ac";

WiFiClient espClient;
PubSubClient client(espClient);

int val_x = 0, val_y = 0, val_z = 0;
double b;
const int portX = 34;
const int portZ = 35;
const int portY = 32;

const int portButton = 23;
int buttonState = 0;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop hasta que estemos conectados al servidor MQTT
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    if (client.connect(device_ID, access_token, "")) {
      Serial.println("conectado");
    } else {
      Serial.print("Error, rc=");
      Serial.print(client.state());
      Serial.println(" intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

// Esta función envía datos a ThingsBoard
void sendAccelDataToThingsBoard() {
  val_x = 0;
  val_y = 0;
  val_z = 0;
  
  for (int i = 0; i < 10; i++) {
    val_x += analogRead(portX);
    val_y += analogRead(portY);
    val_z += analogRead(portZ);
  }

  val_x = val_x / 10;
  val_y = val_y / 10;
  val_z = val_z / 10;

  b = (double)(abs(val_x - 320)) / (abs(val_z - 320));
  float a = atan(b) / 3.14 * 180;

  Serial.print("X: ");
  Serial.print(val_x);
  Serial.print(" Y: ");
  Serial.print(val_y);
  Serial.print(" Z: ");
  Serial.println(val_z);
  Serial.print(" Botón: ");
  Serial.println(buttonState);

  // Construir un mensaje JSON
  String payload = "{";
  payload += "\"val_x\":"; payload += val_x; payload += ",";
  payload += "\"val_y\":"; payload += val_y; payload += ",";
  payload += "\"val_z\":"; payload += val_z; payload += ",";
  payload += "\"buttonState\":"; payload += (buttonState == LOW ? 1 : 0);
  payload += "}";

  // Enviar el mensaje JSON a ThingsBoard
  if (client.connected()) {
    client.publish("v1/devices/me/telemetry", payload.c_str());
  } else {
    reconnect();
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  reconnect();
  pinMode(portButton, INPUT_PULLUP);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  buttonState = digitalRead(portButton);
  sendAccelDataToThingsBoard();
  delay(1000);
}
