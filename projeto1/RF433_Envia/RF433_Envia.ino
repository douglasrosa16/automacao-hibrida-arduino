//Projeto1: ESP8266-12E com 2 botões, envia para Arduino UNO com 2 LEDS

//RF433, RF315 Arduino UNO, Nano -> Receptor(GND, 5V, 11(data) ), Transmissor (GND, 3.3V a 12V, 12(data) )
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

#include <RH_ASK.h> //INCLUSÃO DE BIBLIOTECA
#include <SPI.h> //INCLUSÃO DE BIBLIOTECA

//RH_ASK driver; //CRIA O DRIVER PARA COMUNICAÇÃO ARDUINO UNO E NANO: Transmissor no pino 12, Receptor no pino 11
RH_ASK driver(2000, 2, 4, 5); //CRIA O DRIVER PARA COMUNICAÇÃO ESP8266-12E: Transmissor no pino GPIO 4 (D2), Receptor no pino GPIO 2 (D4)

//Variaveis
int bt1 = D6;
int bt2 = D7;
int bt2 = D8;
int bt1s = 0;
int bt2s = 0;
int bt3s = 0;
//Variaveis

int tempo = 5000;
void enviaRF(char *msg)
{  
  driver.send((uint8_t *)msg, strlen(msg)); //ENVIA AS INFORMAÇÕES PARA O RECEPTOR (PALAVRA: led)
  driver.waitPacketSent(); //AGUARDA O ENVIO DAS INFORMAÇÕES    
  delay(500);
  
  driver.send((uint8_t *)msg, strlen(msg)); //ENVIA AS INFORMAÇÕES PARA O RECEPTOR (PALAVRA: led)
  driver.waitPacketSent(); //AGUARDA O ENVIO DAS INFORMAÇÕES  
}

void setup()
{
  //Define os botões como Entrada
  pinMode(bt1, INPUT);
  pinMode(bt2, INPUT);
  pinMode(bt3, INPUT);
  driver.init(); //INICIALIZA A COMUNICAÇÃO RF DO DRIVER
  Serial.begin(115200); //Serial monitor ESP8266
}

void loop()
{
  //Caso o PINO esteja pressionado
  if (digitalRead(bt1) == HIGH) {
    delay(200);
    
    //Se estiver pressionado fica aguardando
    while (digitalRead(bt1) == HIGH) {
      delay(50);
    }
    
    bt1s = !bt1s;// inverte o sinal (Ligado / Desligado)
    
    if (bt1s == HIGH) {
      enviaRF("|PLACA_MAE_118|FILHA_118_01|2|1|LED|37.11|");// Placa mãe, transmite o comando para a placa Filha 01 ligar o Led 1    
    } else {
      enviaRF("|PLACA_MAE_118|FILHA_118_01|2|0|LED|37.11|");// Placa mãe, transmite o comando para a placa Filha 01 desligar o Led 1	    
    }
    Serial.println(bt1s); //Imprime o status do Botão
  }

  if (digitalRead(bt2) == HIGH) {
    delay(200);
    
    while (digitalRead(bt2) == HIGH) {
      delay(50);
    }
    
    bt2s = !bt2s;// inverte o sinal
    
    if (bt2s == HIGH) {
       enviaRF("|PLACA_MAE_118|FILHA_118_01|3|1|LED|37.11|");// Placa mãe, transmite o comando para a placa Filha 01 ligar o Led 2	   
    } else {
       enviaRF("|PLACA_MAE_118|FILHA_118_01|3|0|LED|37.11|");// Placa mãe, transmite o comando para a placa Filha 01 desligar o Led 2       
    }
    Serial.println(bt2s);
  }

  if (digitalRead(bt3) == HIGH) {
    delay(200);
    
    while (digitalRead(bt3) == HIGH) {
      delay(50);
    }
    
    bt3s = !bt3s;// inverte o sinal
    
    if (bt3s == HIGH) {
       enviaRF("|PLACA_MAE_118|FILHA_118_01|4|1|LED|37.11|");// Placa mãe, transmite o comando para a placa Filha 01 ligar o Led 3	   
    } else {
       enviaRF("|PLACA_MAE_118|FILHA_118_01|4|0|LED|37.11|");// Placa mãe, transmite o comando para a placa Filha 01 desligar o Led 3      
    }
    Serial.println(bt3s);
  }

}
