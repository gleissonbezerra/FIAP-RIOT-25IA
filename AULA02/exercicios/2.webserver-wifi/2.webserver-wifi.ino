#include <ESP8266WiFi.h>
 
const char* ssid = "";//id da sua rede
const char* password = "";//senha do seu wifi
 
int ledPin = 2; // GPIO2 
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
   
  // Inicia conexão
  Serial.println();
  Serial.println();
  Serial.print("Conectando a rede ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
   
  // Iniciar o serviço http na port 80
  server.begin();
  Serial.println("Servidor iniciado");
 
  // Mostra o encereço IP address na serial
  Serial.print("Utilize esta URL para conectar: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
    
}
 
void loop() {
  
  // Verifica se existe um novo cliente se contando, caso contrario reinicia o loop
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  // Espera o cliente enviar algo
  Serial.println("Novo Cliente ");
  while(!client.available()){
    delay(1);
  }
   
  // Le a primeira linha da requisição
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
   
  // Variável de controle do led
  int value = LOW;
  
  // Verifica se foi pedida uma URL específica
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  } 
  if (request.indexOf("/LED=OFF") != -1){
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
 
  // Retorna a resposta HTTP
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
   
  client.print("Led status: ");
   
  if(value == HIGH) {
    client.print("On");  
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("Clique <a href=\"/LED=ON\">aqui</a> para ligar o LED<br>");
  client.println("Clique <a href=\"/LED=OFF\">aqui</a> desligar o LED<br>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Cliente desconectedado!!!");
  Serial.println("");
 
}
