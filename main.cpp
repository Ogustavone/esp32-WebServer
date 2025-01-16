// #include <Arduino.h>
// #include <WiFi.h>
// #include <WebServer.h>

const char* ssid = "nome da rede";
const char* password = "senha da rede";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // define o pino do led como saída
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);  // Serial com baudrate 115200 (esp32)
  WiFi.mode(WIFI_STA);

  // Conexão wifi
  WiFi.begin( ssid, password );
  
  Serial.println("Conectando a rede WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Conectado: " + String(ssid));

  // Webserver
  WebServer server(80);

  server.on("/", rootHandler);
  server.on("/led-on", HTTP_GET, [](){ ledHandler(true); });
  server.on("/led-off", HTTP_GET, [](){ ledHandler(false); });

  server.begin();
  Serial.println("Servidor iniciado, acesse em http://" + String(WiFi.localIP()));
}

void loop() {
  server.handleClient();
}