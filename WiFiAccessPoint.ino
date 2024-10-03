#include <WiFi.h>
#include <WiFiAP.h>

// Configuración del punto de acceso
const char *ssid = "Camion01";
const char *password = "CampusTD";

// Configuración de la red
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("Configuring access point...");

  // Configurar IP estática
  if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
    Serial.println("Failed to configure AP");
    while (1);
  }

  // Iniciar el punto de acceso
  if (!WiFi.softAP(ssid, password)) {
    Serial.println("Failed to create access point");
    while (1);
  }

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();  // Escuchar clientes entrantes

  if (client) {  // Si hay un cliente
    Serial.println("New Client Connected.");  // Mensaje en el Serial Monitor
    String currentLine = "";  // String para almacenar datos entrantes del cliente
    while (client.connected()) {  // Mientras el cliente esté conectado
      if (client.available()) {  // Si hay bytes para leer del cliente
        char c = client.read();  // Leer un byte
        Serial.write(c);  // Imprimirlo en el monitor serial
        if (c == '\n') {  // Si el byte es un carácter de nueva línea

          if (currentLine.length() == 0) {
            // Encabezados HTTP
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // Contenido de la respuesta HTTP
            client.println("<h1>Conectado</h1>");
            client.print("<p>SSID: ");
            client.print(ssid);
            client.println("</p>");
            client.print("<p>IP Address: ");
            client.print(local_IP);
            client.println("</p>");
            client.print("<p>Gateway: ");
            client.print(gateway);
            client.println("</p>");
            client.print("<p>Subnet: ");
            client.print(subnet);
            client.println("</p>");
            client.print("<p>MAC Address: ");
            client.print(WiFi.softAPmacAddress());
            client.println("</p>");
            client.print("<p>Channel: ");
            client.print(WiFi.channel());
            client.println("</p>");
            client.print("<p>RSSI: ");
            client.print(WiFi.RSSI());
            client.println(" dBm</p>");
            client.print("<p>Connected Devices: ");
            client.print(WiFi.softAPgetStationNum());
            client.println("</p>");
            client.print("<p>Uptime: ");
            client.print(millis() / 1000);
            client.println(" seconds</p>");
            client.print("<p>Hostname: ");
            client.print(WiFi.getHostname());
            client.println("</p>");
            client.print("<p>DNS IP: ");
            client.print(WiFi.dnsIP());
            client.println("</p>");
            client.print("<p>BSSID: ");
            client.print(WiFi.BSSIDstr());
            client.println("</p>");
            client.print("<p>WiFi Mode: ");
            client.print(WiFi.getMode());
            client.println("</p>");
            client.print("<p>WiFi Status: ");
            client.print(WiFi.status());
            client.println("</p>");

            // La respuesta HTTP termina con otra línea en blanco:
            client.println();
            break;
          } else {  // Si se recibió una nueva línea, limpiar currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // Si se recibió algo que no es un retorno de carro
          currentLine += c;  // Añadirlo al final de currentLine
        }
      }
    }
    // Cerrar la conexión
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
