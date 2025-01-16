#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "nome da rede";
const char* password = "senha da rede";

void rootHandler() {
  String html = R"(
    <!DOCTYPE html>
    <html lang="pt-br">
      <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <title>Controle ESP32</title>
        <script src="https://cdn.tailwindcss.com"></script>
      </head>
      <body class="flex flex-col h-screen">
        <header class="bg-blue-500 p-4 text-center text-white">
          <h1 class="text-2xl font-bold">Controle ESP32</h1>
        </header>
        <main class="bg-zinc-200 flex-grow">
          <div class="flex justify-center items-center h-full">
            <div class="bg-white p-4 rounded-lg shadow-md w-72 h-80 flex flex-col justify-center items-center">
              <h2 class="text-2xl font-bold mb-3">Built-in LED <span id="status" class="text-green-500">(ON)</span></h2>
              <button id="onButton" class="bg-blue-500 text-white px-4 py-2 w-32 m-1 rounded-md">Ligar</button>
              <button id="offButton" class="bg-red-500 text-white px-4 py-2 w-32 m-1 rounded-md">Desligar</button>
            </div>
          </div>
        </main>
      </body>
      <script>
        const status = document.getElementById('status');
        const offButton = document.getElementById('offButton');
        const onButton = document.getElementById('onButton');
        offButton.addEventListener('click', () => {
          status.textContent = '(OFF)';
          fetch('/off');
        });
        onButton.addEventListener('click', () => {
          status.textContent = '(ON)';
          fetch('/on');
        });
      </script>
    </html>
  )";
  server.send(200, "text/html", html)
}

void ledHandler(bool state) {
  digitalWrite(LED_BUILTIN, state);
  server.send(200, "text/plain", state ? "LED ligado" : "LED desligado");
}

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