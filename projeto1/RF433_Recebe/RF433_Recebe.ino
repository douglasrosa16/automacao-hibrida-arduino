//Projeto1: ESP8266 com 2 botões envia para Arduino UNO com 2 LEDS
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

#include <RH_ASK.h>     //INCLUSÃO DE BIBLIOTECA
#include <SPI.h>        //INCLUSÃO DE BIBLIOTECA
RH_ASK driver;          //CRIA O DRIVER PARA COMUNICAÇÃO

const int LED1 = 2; //PINO DIGITAL UTILIZADO PELO LED
const int LED2 = 3; //PINO DIGITAL UTILIZADO PELO LED
const int LED3 = 4; //PINO DIGITAL UTILIZADO PELO LED

String recebido = "";   //VARIÁVEL DO TIPO STRING
String campos[10];      //Variaveis para campos recebidos (ATÉ 10 CAMPOS)
int i = 0;

/// dados da placa Filha 
String placaMae = "PLACA_MAE_118";
String placaFilha = "FILHA_118_01"; //(Trocar a cada placa)
String todas = "TODAS_118";

void setup()
{
  //Define os pinos como Saída
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  driver.init(); //INICIALIZA A COMUNICAÇÃO RF DO DRIVER RF
  Serial.begin(9600); //Seta o serial monitor
}

void loop(){
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
    Serial.print(recebido);
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
//infcampos[5]: (37.11)           => Indica o valor da variável acima

  if (infcampos[0] == placaMae)//se a placa mãe que enviou os dados é a placa mãe cadastrada nesta placa filha então executa, senão não faz nada.
  {
    if ( (infcampos[1] == placaFilha) || (infcampos[1] == todas) ) //Se a placa mãe enviou para esta placa filha ou para todas as placas filhas
    {
      Serial.print(" ");
      //Serial.println(infcampos[1]);
      Serial.println(infcampos[3]);
      if (infcampos[4] == "LED")//se o tipo de variavel ou componente for um LED
      {
        if (infcampos[1] == todas) {
          if (infcampos[3] == "0") {
            digitalWrite(LED1,LOW);
            digitalWrite(LED2,LOW);
            digitalWrite(LED3,LOW);            
          }else{
            digitalWrite(LED1,HIGH);
            digitalWrite(LED2,HIGH);
            digitalWrite(LED3,HIGH);
          }
        }else{
          if (infcampos[3] == "0") {
            digitalWrite(infcampos[2].toInt(),LOW);
          }else{
            digitalWrite(infcampos[2].toInt(),HIGH);
          }
        }
      }

    }    
  }
}
