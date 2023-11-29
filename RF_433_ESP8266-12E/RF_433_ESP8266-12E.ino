//RF433, RF315 Arduino UNO, Nano -> Receptor(GND, 5V, 11(data) ), Transmissor (GND, 3.5V a 12V, 12(data) )
//RF433 Esp8266-12E -> Receptor (GND, 5V, D4)
//RF433 Esp8266-12E -> Transmissor (GND, Vin, D2)
//RF433 SuperHeterodyne Receptor (3V a 5V)
//RF433 SuperHeterodyne Transmissor (2V a 3.6V)

/*
 Autor: Gilson de Freitas
 Contato: 66-9-9998-6062
 Rondonópolis - MT
 Link do Curso de Programação em Arduino: https://www.hotmart.com/product/arduino-referencia-de-programacao/J7636476D
 Link do Curso Crie teu site em uma semana: https://www.hotmart.com/product/curso-criar-e-publicar-site-do-zero-em-1-semana/A7631617J
 Link do Livro Arduino e Firebase: https://produto.mercadolivre.com.br/MLB-1296636864-ebook-arduino-e-firebasedashboard-codigos-fontes-imediato-_JM?quantity=1
*/

#include <RH_ASK.h> //INCLUSÃO DE BIBLIOTECA
#include <SPI.h>    //INCLUSÃO DE BIBLIOTECA
 
//RH_ASK driver; //CRIA O DRIVER PARA COMUNICAÇÃO ARDUINO UNO E NANO: Transmissor no pino 12, Receptor no pino 11
RH_ASK driver(2000, 2, 4, 5); //CRIA O DRIVER PARA COMUNICAÇÃO ESP8266-12E:
//(Velocidade = 2000, Receptor no pino GPIO2 (D4), Transmissor no pino GPIO4 (D2), 

//WIFI
/*
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WiFiAP.h>
  const char* ssid = "Nome_da_sua_Rede_Wifi";
  const char* password = "Senha_da_sua_Wifi";
  WiFiServer server(80);
*/  
//WIFI

//AP
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  ESP8266WebServer server;
//AP

/*
byte LED1 = 5;
byte LED2 = 4;
byte LED3 = 0;
byte LED4 = 14;
*/


//---------------------------------------------------------------
// Cria a Página HTML
char MAIN_page[] PROGMEM = R"=====(
<html>
  <head>
    <meta http-equiv='refresh' content='3'>
    <style>
      button { font-size:50px; }  
      table, th, td { border: 2px solid black; padding: 5px; font-size:50px;}
      table { border-spacing: 15px; }
    </style>
  </head>
  
  <body>
  <table style="width:100%">
  <tr>
    <td>Componente</td>
    <td>Ligar</td>
    <td>Desligar</td>
    <td>Status</td>
  </tr>
  <tr>
    <td>LED 1</td>
    <td><a href="led1on" target="myIframe1"> <button> ON </button> </a></td>
    <td><a href="led1of" target="myIframe1"> <button> OF </button> </a></td>
    <td><iframe name="myIframe1" frameborder="0" scrolling="0" width="100px" height="30px"></iframe></td>
  </tr>	
  <tr>
    <td>LED 2</td>
    <td><a href="led2on" target="myIframe2"> <button> ON </button> </a></td>
    <td><a href="led2of" target="myIframe2"> <button> OF </button> </a></td>
    <td><iframe name="myIframe2" frameborder="0" scrolling="0" width="100px" height="30px"></iframe></td>
  </tr>	
  <tr>
    <td>LED 3</td>
    <td><a href="led3on" target="myIframe3"> <button> ON </button> </a></td>
    <td><a href="led3of" target="myIframe3"> <button> OF </button> </a></td>
    <td><iframe name="myIframe3" frameborder="0" scrolling="0" width="100px" height="30px"></iframe></td>
  </tr>
  <tr>
    <td>LED TODAS</td>
    <td><a href="ledtodason" target="myIframe4"> <button> ON </button> </a></td>
    <td><a href="ledtodasof" target="myIframe4"> <button> OF </button> </a></td>
    <td><iframe name="myIframe4" frameborder="0" scrolling="0" width="100px" height="30px"></iframe></td>
  </tr>
<hr>
</body>
</html>
)=====";

void handleRoot() { String s = MAIN_page; server.send(200, "text/html", s); }

void handleled1on(){     /*digitalWrite(LED1,HIGH);*/ server.send(200, "text/html", "ON"); enviaRF("|PLACA_MAE_118|FILHA_118_01|2|1|LED|37.11|"); }
void handleled1of(){     /*digitalWrite(LED1,LOW);*/  server.send(200, "text/html", "OF"); enviaRF("|PLACA_MAE_118|FILHA_118_01|2|0|LED|37.11|"); }

void handleled2on(){     /*digitalWrite(LED2,HIGH);*/ server.send(200, "text/html", "ON"); enviaRF("|PLACA_MAE_118|FILHA_118_01|3|1|LED|37.11|"); }
void handleled2of(){     /*digitalWrite(LED2,LOW);*/  server.send(200, "text/html", "OF"); enviaRF("|PLACA_MAE_118|FILHA_118_01|3|0|LED|37.11|"); }

void handleled3on(){     /*digitalWrite(LED3,HIGH);*/ server.send(200, "text/html", "ON"); enviaRF("|PLACA_MAE_118|FILHA_118_01|4|1|LED|37.11|"); }
void handleled3of(){     /*digitalWrite(LED3,LOW);*/  server.send(200, "text/html", "OF"); enviaRF("|PLACA_MAE_118|FILHA_118_01|4|0|LED|37.11|"); }

void handleledtodason(){ /*digitalWrite(LED4,HIGH);*/ server.send(200, "text/html", "ON"); enviaRF("|PLACA_MAE_118|TODAS_118|5|1|LED|37.11|"); }
void handleledtodasof(){ /*digitalWrite(LED4,LOW);*/  server.send(200, "text/html", "OF"); enviaRF("|PLACA_MAE_118|TODAS_118|5|0|LED|37.11|"); }

void enviaRF(char *msg)
{  
  driver.send((uint8_t *)msg, strlen(msg)); //ENVIA AS INFORMAÇÕES PARA O RECEPTOR (PALAVRA: led)
  driver.waitPacketSent(); //AGUARDA O ENVIO DAS INFORMAÇÕES  
}

void setup(){
//  pinMode(LED1,OUTPUT);
//  pinMode(LED2,OUTPUT);
//  pinMode(LED3,OUTPUT);
//  pinMode(LED4,OUTPUT);

  driver.init(); //INICIALIZA A COMUNICAÇÃO RF DO DRIVER
  Serial.begin(115200);  
  Serial.println();

  WiFi.mode(WIFI_AP);                     //Funcionamento somente como Ponto de Acesso
  //WiFi.softAP(ssid, password);          //WIFI: Ponto de Acesso pela sua Wifi
  WiFi.softAP("ESP8266", "12345678");     //Nome da Rede e senha que a Esp8266 vai gerar

  //http://192.168.4.1                    // conectar na placa com este IP
  //IPAddress myIP = WiFi.softAPIP();     // lista o IP da placa. Serve para quando voce precisa conectar na placa mas não sabe o IP
  //Serial.print("Meu IP:"+String(myIP)); // Mosta o IP no Monitor Serial
  
  //chamada das funções
  server.on("/", handleRoot);                                             //Chama a página inicial (Página HTML)
  server.on("/led1on", handleled1on); server.on("/led1of", handleled1of); //Chama os handles led1on e led1of
  server.on("/led2on", handleled2on); server.on("/led2of", handleled2of); //Chama os handles led2on e led2of
  server.on("/led3on", handleled3on); server.on("/led3of", handleled3of); //Chama os handles led3on e led3of
  server.on("/ledtodason", handleledtodason); server.on("/ledtodasof", handleledtodasof); //Chama os handles ledtodason e ledtodasof

  server.begin(); //Inicia o servidor para a chamada das funções
}

void loop()
{
  server.handleClient();          //Habilita a requisição para clientes (conexões através de celulares para acesso e controle)
  //Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum()); // mostra o número de dispositivos conectados
  delay(10);
}