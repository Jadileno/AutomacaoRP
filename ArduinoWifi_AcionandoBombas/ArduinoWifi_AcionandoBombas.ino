#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


WiFiClient client;

const char* ssid = "NURVAS";
const char* password = "SOS@123123";
WiFiServer server(80);

String readString;
 int pino_rele1 = 3;  
int pino_rele2 = 4;
boolean ligado = true;
boolean ligado_2 = true;
 
void setup()
{
    Serial.begin(115200);
    delay(10);
    connectWiFi();
  
    pinMode(pino_rele1, OUTPUT);
    pinMode(pino_rele2, OUTPUT);
 
    //Desliga os dois reles
    digitalWrite(pino_rele1, LOW);
    digitalWrite(pino_rele2, LOW);
  
  Serial.println("RoboticParts"); 
 
}
 
void loop()
{
  WiFiClient client = server.available();
  if (client) {
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        if (readString.length() < 100) {
          readString += c;
        }
        if (c == '\n')
        {
          //Controle do rele1
          Serial.println(readString);
          //Liga o Rele 1
          if (readString.indexOf("?ligar") > 0)
          {
            digitalWrite(pino_rele1, HIGH);
            Serial.println("Rele 1 Ligado");
            ligado = false;
          }
          else
          {
            //Desliga o Rele 1
            if (readString.indexOf("?desligar") > 0)
            {
              digitalWrite(pino_rele1, LOW);
              Serial.println("Rele 1 Desligado");
              ligado = true;
            }
          }
 
          //Controle do rele2
          Serial.println(readString);
          //Liga o Rele 2
          if (readString.indexOf("?2_ligar") > 0)
          {
            digitalWrite(pino_rele2, HIGH);
            Serial.println("Rele 2 Ligado");
            ligado_2 = false;
          }
          else
          {
            //Desliga o Rele 2
            if (readString.indexOf("?2_desligar") > 0)
            {
              digitalWrite(pino_rele2, LOW);
              Serial.println("Rele 2 Desligado");
              ligado_2 = true;
            }
          }
          readString = "";
 
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<!doctype html>");
          client.println("<html lang = 'pt-BR'>");
          client.println("<html>");
          client.println("<head>");
          client.println("<title> RoboticParts Automacao</title>");
          //client.println("<meta http-equiv='Content-Type' content='text/html; charset=ISO-8859-1'>");
          //client.println("<meta name='viewport' content='width=720, initial-scale=0.5' />");

          client.println("<link rel='stylesheet' type='text/css' href='https://github.com/Jadileno/Acionamento_Bomba_WiFi/blob/master/ProjetoBombaAgua_ArduinoWiFi/ccs/automacao.css' />");
          client.println("<script type='text/javascript' src='https://github.com/Jadileno/Acionamento_Bomba_WiFi/blob/master/ProjetoBombaAgua_ArduinoWiFi/js/automacao.js'></script>");

          client.println("</head>");
          client.println("<body>");
          client.println("<div id='wrapper'><img alt='Testes.png' src='/Jadileno/Acionamento_Bomba_WiFi/blob/master/ProjetoBombaAgua_ArduinoWiFi/logo/Testes.png?raw=true'/><br/>");
          client.println();
          client.println();
          client.println("<div id='div1'>Motor 1</div>");
          client.println("<div id='div2'>Motor 2</div>");
          client.println("<div id='div2'>SensorPressao</div>");
          client.print("<div id='rele'></div><div id='estado' style='visibility: hidden;'>");
          client.print(ligado);
          client.println("</div>");
          client.println("<div id='botao'></div>");
          client.println("<div id='botao_2'></div>");
          client.print("<div id='rele_2'></div><div id='estado_2' style='visibility: hidden;'>");
          client.print(ligado_2);
          client.println("</div>");
          client.println("</div>");
          client.println("<script>AlteraRele1()</script>");
          client.println("<script>AlteraRele2()</script>");
          client.println("</div>");
          client.println("</body>");
          client.println("</head>");
 
          delay(1);
          client.stop();
        }
      }
    }
  }
}

void connectWiFi() {
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  // Inicia o servidor (SERVER)
  server.begin();
  Serial.println("Server started");
  // Print the IP address
  Serial.print("Use este URL to conectar:");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

