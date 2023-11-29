//RF433, RF315 Arduino UNO, Nano -> Receptor(GND, 5V, 11(data) ), Transmissor (GND, 3.5V a 12V, 12(data) )
//RF433 Esp8266-12E -> Receptor (GND, 5V, D4)
//RF433 Esp8266-12E -> Transmissor (GND, Vin, D2)
//RF433 SuperHeterodyne Receptor (3V a 5V)
//RF433 SuperHeterodyne Transmissor (2V a 3.6V)

/*
 Autor: Gilson de Freitas
 Contato: 66-9-9998-6062
 Rondonópolis - MT
 Canal do Youtube: CurseAgora -> https://www.youtube.com/playlist?list=PLBN3lYxRDbp2teC_Bl79ajtkqRolPgDqt
*/

//https://circuits4you.com/2018/11/20/web-server-on-esp32-how-to-update-and-display-sensor-values/
/*
 * ESP32 AJAX Demo
 * Updates and Gets data from webpage without page refresh
 * https://circuits4you.com
 */
#include <RH_ASK.h> //INCLUSÃO DE BIBLIOTECA
#include <SPI.h> //INCLUSÃO DE BIBLIOTECA
 
//RH_ASK driver; //CRIA O DRIVER PARA COMUNICAÇÃO ARDUINO UNO E NANO: Transmissor no pino 12, Receptor no pino 11
RH_ASK driver(2000, 2, 4, 5); //CRIA O DRIVER PARA COMUNICAÇÃO ESP8266-12E:
//(Velocidade = 2000, Receptor no pino GPIO2 (D4), Transmissor no pino GPIO4 (D2), 
 
//#include <WiFi.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "index.h"  //Web page header file

ESP8266WebServer server(80);

String recebido = "";   //VARIÁVEL DO TIPO STRING
String campos[10];      //Variaveis para campos recebidos (ATÉ 10 CAMPOS)
int i = 0;
int valorpino = 0;
int led01, led02, led03;

/// dados da placa MAE
String placaMae = "PLACA_MAE_118";

//Enter your SSID and PASSWORD
const char* ssid = "VIVOFIBRA-E5C0";
const char* password = "6CBC8179FE";

//DEFINIÇÃO DE IP FIXO PARA O NODEMCU ESP8266-12E
//IPAddress ip(192,168,15,8); //COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR. EX: 192.168.1.110 **** ISSO VARIA, NO MEU CASO É: 192.168.0.175
//IPAddress gateway(192,168,15,1); //GATEWAY DE CONEXÃO (ALTERE PARA O GATEWAY DO SEU ROTEADOR)
//IPAddress subnet(255,255,255,0); //MASCARA DE REDE

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot()
{
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleContagem() {
  valorpino++;
  String VarContagem = String(valorpino);
  server.send(200, "text/plane", VarContagem); //Send ADC value only to client ajax request
}
 
void handleVariavel01()
{
  String Variavel01;
  if (led01 == 1) { Variavel01 = "On"; } else { Variavel01 = "Off"; };
  server.send(200, "text/plane", Variavel01);
}
 
void handleVariavel02()
{
  String Variavel02;
  if (led02 == 1) { Variavel02 = "On"; } else { Variavel02 = "Off"; };
  server.send(200, "text/plane", Variavel02);
}
 
void handleVariavel03()
{
  String Variavel03;
  if (led03 == 1) { Variavel03 = "On"; } else { Variavel03 = "Off"; }
  server.send(200, "text/plane", Variavel03);
}

//===============================================================
// Setup
//===============================================================
void setup(void)
{
  Serial.begin(115200);
  driver.init(); //INICIALIZA A COMUNICAÇÃO RF DO DRIVER RF
/*
//ESP8266 As access point
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);
*/

//ES8266 connects to your wifi -----------------------------------
  WiFi.mode(WIFI_STA); //Connectto your wifi
  WiFi.begin(ssid, password);
  //WiFi.config(ip, gateway, subnet);

  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      Serial.print(".");
    }
    
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
//----------------------------------------------------------------
 
  server.on("/", handleRoot);                   //Executar a handleRoot para Mostrar na página
  server.on("/LerContagem", handleContagem);//To get update of ADC Value only
  server.on("/LerVariavel01", handleVariavel01);//Executarar a handleVariavel01 para atualizar somente o valor da Variavel01
  server.on("/LerVariavel02", handleVariavel02);//Executarar a handleVariavel02 para atualizar somente o valor da Variavel02
  server.on("/LerVariavel03", handleVariavel03);//Executarar a handleVariavel03 para atualizar somente o valor da Variavel03
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

void loop(void)
{
  server.handleClient();
  delay(10);
    
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN]; //LÊ A MENSAGEM RECEBIDA POR RF433
  uint8_t buflen = sizeof(buf);        //CRIA O COMPRIMENTO DO BUFFER PARA O TAMANHO DE buf

  if(driver.recv(buf, &buflen)) //SE O DRIVER RECEBEU buf(INTEIRO) E buflen (COMPRIMENTO DE DADOS), FAZ
  {
    recebido = ""; //VARIÁVEL RECEBE VAZIO
    i = 0;
    while (i < buflen)//Enquanto i for menor que a quantidade total do buflen, percorre cada caracter ao incrementar 1 na variavel i
    {
      recebido += (char)buf[i]; //a variavel recebido, agrupa cada letra lida da variavel buf para formar todo o texto recebido
      i++;
    }
  }

  if (recebido.length() > 0)
  {
    //Serial.print(recebido); Serial.print(" - ");
    i = 0;
    while (i < 10)
    {
      int separador1 = recebido.indexOf('|');
      int separador2 = recebido.indexOf('|', separador1 + 1);
      String txt = recebido.substring(separador1+1, separador2);
      recebido.remove(separador1+1,separador2);

      campos[i] = txt;
      i++;
    }
    executacomando(campos);
    recebido = "";
  }
}

void executacomando(String infcampos[]){
//|PLACA_MAE_118|FILHA_118_01|3|1|LED|37.11|

//infcampos[0]: ("PLACA_MAE_118") => Indica se o conteúdo vem da placa mãe correta
//infcampos[1]: ("FILHA_118_01")  => Indica a informação é par esta placa filha
//infcampos[2]: (0 A 99)          => Indica o número do pino
//infcampos[3]: (0 OU 1)          => Indica o valor do pino 0:LOW, 1:HIGH
//infcampos[4]: (LED)             => Indica o nome de alguma variável para controle ou Tipo de variável
//infcampos[5]: (37.11)           => Indica o valor da variáel acima

  if (infcampos[0] == placaMae)//se a placa mãe que enviou os dados é a placa mãe cadastrada nesta placa filha então executa, senão não faz nada.
  {
    Serial.println(infcampos[1]);// imprime o nome da placa filha

    if (infcampos[1] == "FILHA_118") //Se a placa filha é conhecida por esta placa mãe e pediu para ligar ou desligar todos os leds
    {      
      if (infcampos[4] == "LED")//se o tipo de variavel ou componente for um LED
      {
        led01 = infcampos[3].toInt();
        led02 = infcampos[3].toInt();
        led03 = infcampos[3].toInt();        
      }
    }

    if (infcampos[1] == "FILHA_118_01") //Se a placa filha é conhecida por esta placa mãe
    {      
      if (infcampos[4] == "LED")//se o tipo de variavel ou componente for um LED
      {
        led01 = infcampos[3].toInt();
      }
    }

    if (infcampos[1] == "FILHA_118_02") //Se a placa filha é conhecida por esta placa mãe
    {      
      if (infcampos[4] == "LED")//se o tipo de variavel ou componente for um LED
      {
        led02 = infcampos[3].toInt();
      }
    }

    if (infcampos[1] == "FILHA_118_03") //Se a placa filha é conhecida por esta placa mãe
    {      
      if (infcampos[4] == "LED")//se o tipo de variavel ou componente for um LED
      {
        led03 = infcampos[3].toInt();
      }
    }
    
    Serial.print("led01: ");Serial.println(led01);
    Serial.print("led02: ");Serial.println(led02);
    Serial.print("led03: ");Serial.println(led03);
    Serial.println("==========================================================");
  }
}