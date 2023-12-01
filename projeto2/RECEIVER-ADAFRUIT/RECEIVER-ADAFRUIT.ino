/*
 Autores: Gilson de Freitas, Douglas Oliveira, Ayron Strensk
 Contato: 66-9-9998-6062
 Rondonópolis - MT
 Curso 1(Programaçã): https://hotmart.com/pt-br/marketplace/produtos/arduino-referencia-de-programacao/J7636476D?sck=HOTMART_SITE
 Curso 2(RF433 MHZ ): https://hotmart.com/pt-br/marketplace/produtos/curso-de-arduino-radio-frequencia-433mhz/V21307818G?sck=HOTMART_SITE
*/

#include "config.h"                       // Inclusão do arquivo config.h (este arquivo config.h, deve estar junto a este na mesma pasta)
AdafruitIO_Feed *LED1 = io.feed("LED1");  // LED1 é o nome do feed(nome da variável criada exatamente na API da Adafruit)
AdafruitIO_Feed *LED2 = io.feed("LED2");  // LED2 é o nome do feed(nome da variável criada exatamente na API da Adafruit)
AdafruitIO_Feed *LED3 = io.feed("LED3");  // LED3 é o nome do feed(nome da variável criada exatamente na API da Adafruit)

//LORA - TRANSMISSOR
#include <SoftwareSerial.h>               // Inclusão da biblioteca para comunicação Serial entre Arduino Lora
//SoftwareSerial loraSerial(2, 3);        // Configuração dos pinos Arduino Uno(2       como TX e 3       como RX)
SoftwareSerial loraSerial(D2, D1);        // Configuração dos pinos ESP8266-12E(4 ou D2 como TX e 5 ou D1 como RX)
//Obs1.: Arduino Uno Receptor(Pino 2 no pino TX do Lora) ==e== (Pino 3 no pino RX do Lora) 
//Obs2.: ESP8266-12E Receptor(Pino 4 no pino TX do Lora) ==e== (Pino 5 no pino RX do Lora) 

String valor = "";                        // Variável que vai receber o valor dos Feeds da API da Adafruit

void setup()                              // Função que executa apenas uma vez (ao ligar a placa)
{
  Serial.begin(115200);                   // Inicializa o Monitor Serial (para quando estiver testando no computador)
  loraSerial.begin(9600);                 // Inicializa o Módulo Lora na frequência 9600MHZ
  while(! Serial);                        // Aguarda o serial monitor abrir

  Serial.println("Início");               // Imprime "Início" no Monitor Serial (para quando estiver testando no computador)
  Serial.print("Conectando na ADAFRUIT"); // Imprime "Conectando na ADAFRUIT" no Monitor Serial (para quando estiver testando no computador)

  io.connect();                           // Conecta na ADAFRUI

  //Criação dos nomes das funções que buscam os valores das variáveis, quando forem alteradas na API da Adafruit
  LED1->onMessage(handleLED1);            // Cria a função handleLED1 referente ao Feed LED1 da API
  LED2->onMessage(handleLED2);            // Cria a função handleLED2 referente ao Feed LED2 da API
  LED3->onMessage(handleLED3);            // Cria a função handleLED3 referente ao Feed LED3 da API

  while(io.status() < AIO_CONNECTED) {    // Aguardando a conexão com a API Adafruit
    Serial.print(".");                    // Imprime "." até que esta placa se conecte com a API Adafruit
    delay(1000);                          // Delay de 1 segundo
  }

  Serial.println(io.statusText());        // Imprime "Conectado" assim que conectar na API Adafruit
  LED1->get();                            // Força a busca de dados do Feed LED1
  LED2->get();                            // Força a busca de dados do Feed LED2
  LED3->get();                            // Força a busca de dados do Feed LED3
}

void loop()                               // Função infinita, reinicia ao terminar
{
  io.run();                               // Mantem executando a conexão e busca de alteração de dados na API Adafruit

  // Exemplo de envio dados coletados na placa para a API Adafruit
  //LED1->save(0);                        // Envia o valor 0 para o Feed LED1
  //LED2->save(0);                        // Envia o valor 0 para o Feed LED2
  //LED3->save(0);                        // Envia o valor 0 para o Feed LED3
}

void handleLED1(AdafruitIO_Data *data)    // Função de recebimento de cada FEED(variavel) ADAFRUIT
{
  Serial.print(data->feedName());         // Impressão do nome do Feed no Monitor Serial para testes no computador
  Serial.print(": ");                     // Imprime ": " no Monitor Serial para testes no computador
  Serial.println(data->value());          // Imprime o valor recebido do Feed da API no Monitor Serial para testes no computador

  valor = String(data->value());          // Atribuir o valor recebido do feed para a variável valor
  loraSerial.print("|01:" + valor + "|"); // Envia "|01:0|" ou "|01:1|" para a Lora
  delay(100);                             // Aguarda 1 décimo de segundo
}

void handleLED2(AdafruitIO_Data *data)    // Função de recebimento de cada FEED(variavel) ADAFRUIT
{
  Serial.print(data->feedName());         // Impressão do nome do Feed no Monitor Serial para testes no computador
  Serial.print(": ");                     // Imprime ": " no Monitor Serial para testes no computador
  Serial.println(data->value());          // Imprime o valor recebido do Feed da API no Monitor Serial para testes no computador

  valor = String(data->value());          // Atribuir o valor recebido do feed para a variável valor
  loraSerial.print("|02:" + valor + "|"); // Envia "|02:0|" ou "|02:1|" para a Lora
  delay(100);                             // Aguarda 1 décimo de segundo
}

void handleLED3(AdafruitIO_Data *data)    // Função de recebimento de cada FEED(variavel) ADAFRUIT
{
  Serial.print(data->feedName());         // Impressão do nome do Feed no Monitor Serial para testes no computador
  Serial.print(": ");                     // Imprime ": " no Monitor Serial para testes no computador
  Serial.println(data->value());          // Imprime o valor recebido do Feed da API no Monitor Serial para testes no computador

  valor = String(data->value());          // Atribuir o valor recebido do feed para a variável valor
  loraSerial.print("|03:" + valor + "|"); // Envia "|03:0|" ou "|03:1|" para a Lora
  delay(100);                             // Aguarda 1 décimo de segundo
}
