/*
 Projeto: Automação Híbrida no campo através de WiFi e Rádio Lora
 Autores: Gilson de Freitas, Douglas Oliveira, Ayron Strensk
 Contato: 66-9-9998-6062
 Rondonópolis - MT
 Canal do Youtube: CurseAgora -> https://www.youtube.com/playlist?list=PLBN3lYxRDbp2teC_Bl79ajtkqRolPgDqt
*/

//LORA
#include <SoftwareSerial.h>          // Inclusão da biblioteca para comunicação Serial entre Arduino Lora
SoftwareSerial loraSerial(2, 3);     // Configuração dos pinos Arduino Uno(2       como TX e 3       como RX)
//SoftwareSerial loraSerial(D2, D1); // Configuração dos pinos ESP8266-12E(4 ou D2 como TX e 5 ou D1 como RX)
//Obs1.: Arduino Uno Receptor(Pino 2 no pino TX do Lora) ==e== (Pino 3 no pino RX do Lora) 
//Obs2.: ESP8266-12E Receptor(Pino 4 no pino TX do Lora) ==e== (Pino 5 no pino RX do Lora) 

const int LED1           = 4;    //Pino Digital para o LED 1
const int LED2           = 5;    //Pino Digital para o LED 2
const int LED3           = 6;    //Pino Digital para o LED 3 
const int LIGA           = HIGH; //Constante a ser usada para Ligar    LED ou RELÉS
const int DESLIGA        = LOW;  //Constante a ser usada para Desligar LED ou RELÉS
String    TEXTO_RECEBIDO = "";   //Variável que vai receber a Mensagem do módulo Lora

void setup()                     //Função que executa apenas uma vez (ao ligar a placa)
{
  pinMode(LED1, OUTPUT);         //Configura o pino LED1 como saída
  pinMode(LED2, OUTPUT);         //Configura o pino LED2 como saída
  pinMode(LED3, OUTPUT);         //Configura o pino LED3 como saída

  Serial.begin(9600);            //Inicializa o Monitor Serial (para quando estiver testando no computador)
  Serial.println("Início");      //Imprime "Início" no Monitor Serial (para quando estiver testando no computador)
  loraSerial.begin(9600);        //Inicializa o Módulo Lora na frequência 9600MHZ
}

void loop()                                                                   //Função infinita, reinicia ao terminar
{
  if (loraSerial.available() > 1)                                             //Se o receptor Lora recebeu alguma informação
  {
    TEXTO_RECEBIDO = loraSerial.readString();                                 // Atribui o texto recebido do Lora, para a variável TEXTO_RECEBIDO
    Serial.println(TEXTO_RECEBIDO);                                           // Imprime no Monitor Serial o valor da variável TEXTO_RECEBIDO

    if (TEXTO_RECEBIDO.indexOf("01:0|") > 0) { digitalWrite(LED1,DESLIGA); }; // Se o valor de 01 for 0, então desliga o LED1
    if (TEXTO_RECEBIDO.indexOf("01:1|") > 0) { digitalWrite(LED1,LIGA   ); }; // Se o valor de 01 for 1, então    liga o LED1

    if (TEXTO_RECEBIDO.indexOf("02:0|") > 0) { digitalWrite(LED2,DESLIGA); }; // Se o valor de 02 for 0, então desliga o LED2
    if (TEXTO_RECEBIDO.indexOf("02:1|") > 0) { digitalWrite(LED2,LIGA   ); }; // Se o valor de 02 for 1, então    liga o LED2

    if (TEXTO_RECEBIDO.indexOf("03:0|") > 0) { digitalWrite(LED3,DESLIGA); }; // Se o valor de 03 for 0, então desliga o LED3
    if (TEXTO_RECEBIDO.indexOf("03:1|") > 0) { digitalWrite(LED3,LIGA   ); }; // Se o valor de 03 for 1, então    liga o LED3
  }
  delay(100);                                                                 // delay de 1 décimo de segundos
}
