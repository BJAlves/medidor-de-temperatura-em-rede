#include <SPI.h>
#include <Ethernet.h>
#include "stdlib.h"
#include "stdio.h"

const int LM35 = A0; 
int ledPin = 7;
int ledPin2 = 6;
int ledPin3 = 5;
int flag=0;
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0F, 0xBA, 0x15 };
IPAddress ip(172,16,12,246);
IPAddress myDns(172,16,0, 7);
IPAddress gateway(172, 16, 12, 1);
IPAddress subnet(255, 255, 252, 0);
 
/* É criada uma instância de server, chamada EthernetServer, que fica esperando pelo cliente na porta 6000 */

EthernetServer server(6000);


void setup() {

  /* LEDS sao configurados como saídas */
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  Serial.begin(9600);
  
  while (!Serial) {
    ;
  }


  /* Começa a conexão ethernet */
  Serial.println("Trying to get an IP address using DHCP");
 
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    /* Inicializa o dispositivo ethernet sem usar DHCP */
    Ethernet.begin(mac, ip, myDns, gateway, subnet);
  }
  /* Printa o endereço IP local */
  Serial.print("My IP address: ");
  ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    /* Printa os valores de cada byte do endereço IP */
    Serial.print(ip[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();
  /* Começa a escutar os clientes */
  server.begin();

}
  
void loop() {
  char tempA[30];
  String temp_str;
  String tr, mr;
  float temp, salvaTr;
  

  
  /* Espera por um novo cliente */
  EthernetClient client = server.available();
  
   

  /* Se houver cliente */
  if (client) {

       /* Quando flag vale 0, lê a requsição do cliente: "Qual a temperatura atual?" */
       if(flag == 0)
       {
         mr = client.readString();
         Serial.print("Pedido do cliente: ");
         Serial.println(mr);
         
         flag = 1;
         
       }

       /* Quando flag vale 2 o servidor lê do cliente a temperatura que ele deseja para o ar condicionado */
       if(flag == 2)
       {
         tr = client.readString();
         Serial.print("Temperatura requisitada: ");
         Serial.println(tr);
         salvaTr = StrToFloat(tr);
         /* A conversão de String para float é salva em salvaTr e esta variável é passada como parâmetro
          Para o procedimento acendeLeds */
         acendeLeds(salvaTr);
        
         
       }
       
     /* Quando flag vale 1 lê a temperatura do sensor LM35 e manda a mesma para o cliente */
      if(flag == 1)
       {
         temp = medirTemperatura(LM35);
          Serial.println(temp);
          dtostrf(temp, 4, 2, tempA);
          /* Função dtostrf: Converte Float em char, pois a temperatura é recebida no cliente em um vetor de char.
           4 indica o tamanho maximo do texto convertido. 2 indica o número de casas depois da vírgula.
           tempA é o vetor de char no qual a conversão para char será salva. */
          
          for(int i=0; i<sizeof(tempA); i++)
          {
            temp_str += tempA[i];
            /* Percorre todo o vetor de char, concatenando caracter por caracter a string temp_str */
          }

          /* Acende o LED que corresponde a faixa de temperatura apropriada */
          acendeLeds(temp);
          
          client.println(temp_str);
          /* manda a string temp_str para o cliente */
         
         flag = 2;
       }

      
      Ethernet.maintain(); /* Renovação da concessão de endereço para dispositivos de rede a partir do servidor DHCP */
  }
}


float medirTemperatura(int st)
{
 float temperatura = (float(analogRead(st))*5/(1023))/0.01;
 /* Converte a leitura analógica para a temperatura correspondente em º C */
 
  return temperatura;
}

float StrToFloat(String str) /* Converte float para String */
{
     char carray[str.length() + 1];  /* cria um vetor de char chamado carray com uma casa a mais do que o total de casas da String fornecida como parâmetro */
     str.toCharArray(carray, sizeof(carray)); /* Converte a String str fornecida para carray, com o mesmo tamanho deste. */
    return atof(carray); /* O uso da função atof, transforma a String que havia em carray em float. */
}

/* Procedimento que faz a correspondência entre as faixas de temperatura e os LEDS */
void acendeLeds(float t)
{
  if(t <= 25)
  {
    digitalWrite(ledPin, HIGH);   
    digitalWrite(ledPin2, LOW);   
    digitalWrite(ledPin3, LOW);   
    delay(2000);               
  }
  
  if(t > 25 && t <=27)
  {
    digitalWrite(ledPin, LOW);   
    digitalWrite(ledPin2, HIGH);   
    digitalWrite(ledPin3, LOW);   
    delay(2000);    
  
  }
  
  if(t > 27 && t <=40)
  {
    digitalWrite(ledPin, LOW);   
    digitalWrite(ledPin2, LOW);   
    digitalWrite(ledPin3, HIGH);   
    delay(2000);    
  }
  
}
