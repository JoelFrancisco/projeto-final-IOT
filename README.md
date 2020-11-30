# Projeto final acerca da disciplina de IOT do curso técnico de informática SATC

<br>
<br>

# SUMÁRIO
-[SOBRE O PROJETO](https://github.com/JoelFrancisco/projeto-final-IOT#sobre-o-projeto)<br>
-[ESQUEMÁTICO](https://github.com/JoelFrancisco/projeto-final-IOT#esquem%C3%A1tico)<br>
-[BIBLIOTECAS NECESSÁRIAS](https://github.com/JoelFrancisco/projeto-final-IOT#bibliotecas-necess%C3%A1rias)<br>
-[CÓDIGO](https://github.com/JoelFrancisco/projeto-final-IOT#c%C3%B3digo)<br>
-[APP NO BLYNK](https://github.com/JoelFrancisco/projeto-final-IOT#app-no-blynk)

# SOBRE O PROJETO

O projeto consiste em um sistema simples com Arduino UNO, ESP-01 e Blynk APP para a exibição de informações dos sensores no APP assim como atuar como base para um sistema básico de segurança.

## Arduino UNO

Possívelmente a microcontrolador mais conhecido pelos makers, dado a seu custo e facilidade de uso.

## ESP-01

Módulo derivado do chip ESP 8266, vem ficando cada vez mais conhecido principalmente no universo de IOR por seu preço e conexão com internet, nesse projeto em especifíco é utilizado como Shield do Arduino UNO, para conecta-lo a internet.

## Blynk APP

Plataforma para facilitar o universo do IOT, possuindo versões pagas e uma gratuita(a qual foi a utilizada nesse projeto). Será utilizado para a comunicação entre um dispositivo android e o ESP-01.

Leia a documentação: [aqui](https://blynk.io/en/getting-started)

**Será necessário instalar o APP Blynk disponível para Android e IOS, procure por Blynk na sua loja de aplicativos ou veja o link na documentação.**

<br>

# ESQUEMÁTICO

![Esquema](/img/esquema/esquema.png)
### Recursos necessários para realização do projeto:

* Arduino Uno
* ESP-01
* ESP-01 Adapter(o esquemático não está utilizando o adaptador)
* DHT11
* HC-SR04
* Buzzer
* LED RGB
* LED
* Botão
* 1 resistor 460ohms
* 1 resistor 150ohms
* 1 resistor 180ohms
* 2 resistor 390ohms
* 26 jumpers
* Protoboard

<br>

# BIBLIOTECAS NECESSÁRIAS
Para realização do projeto será necessária a instalação de algumas bibliotecas na IDE do Arduino.

## Como instalar bibliotecas por ZIP
Algumas bibliotecas que serão utilizadas serão instaladas por meio de um arquivo compactado em ZIP, proveniente do código fonte das bibliotecas presentes no Github, pórem, inicialmente é necessário conhecer como fazer essa instalação.
<br>

**Sketch > Incluir Biblioteca > Adicionar biblioteca .ZIP**

![instalação Libs por ZIP](/img/instalacao_libs/instalacao_libs_zip1.png)
![instalação Libs por ZIP 2](/img/instalacao_libs/instalacao_libs_zip2.png)

## Biblioteca para o sensor HC-SR04
-[Utilize esse link](https://github.com/filipeflop/Ultrasonic)

## Biblioteca para o sensor DHT11
-[Utilize esse link](https://github.com/adafruit/DHT-sensor-library)

### OBS: é necessário incluir um header adicional para o funcionamento da biblioteca.
<br>
Para isso, será preciso ir para o diretório no qual são instaladas as bibliotecas no Arduino, para descobrir esse diretório é necessário.

### Achar diretório onde ficam as Libs
**Arquivo > Preferências**
![instalação Libs por ZIP 3](/img/instalacao_libs/instalacao_libs_zip3.png)
![instalação Libs por ZIP 4](/img/instalacao_libs/instalacao_libs_zip4.png)
**libraries**
![instalação Libs por ZIP 5](/img/instalacao_libs/instalacao_libs_zip5.png)
**DHT-sensor-library-master**<br>
![instalação Libs por ZIP 6](/img/instalacao_libs/instalacao_libs_zip6.png)
![instalação Libs por ZIP 7](/img/instalacao_libs/instalacao_libs_zip7.png)<br>
Crie um arquivo com "Adafruit_Sensor.h" e abra-o em um editor de texto, praticamente qualquer editor pode ser utilizado para isso, eu estarei utilizando o vim.
![instalação Libs por ZIP 8](/img/instalacao_libs/instalacao_libs_zip8.png)<br>
-[Após isso utilize esse link e copie esse código para o arquivo criado](https://github.com/adafruit/Adafruit_Sensor/blob/master/Adafruit_Sensor.h)
![instalação Libs por ZIP 9](/img/instalacao_libs/instalacao_libs_zip9.png)

Com isso, está pronto para o funcionamento a lib do DHT.

## Biblioteca para o Blynk APP
Essa biblioteca será instalada de maneira diferente das anteriores<br>
**Sketch > Incluir Biblioteca > Gerenciar Bibliotecas...**
![instalação Libs por ZIP 10](/img/instalacao_libs/instalacao_libs_zip10.png)
Procure por Blynk e instale
![instalação Libs por ZIP 11](/img/instalacao_libs/instalacao_libs_zip11.png)

<br>
<br>

# CÓDIGO

## Definições iniciais do Blynk APP
```c++
// Blynk setup
#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = "Seu token deve ser colado aqui";

char ssid[] = "Coloque o nome da sua rede WiFi";
char pass[] = "Coloque a senha da sua rede WiFi";

#include <SoftwareSerial.h>
SoftwareSerial EspSerial(3, 4); // RX, TX

#define ESP8266_BAUD 9600  

ESP8266 wifi(&EspSerial);

BlynkTimer timer;
```
<br>
Inicialmente temos a definição das informações de conexão do Blynk APP no monitor serial. Juntamente com a inclusão dos headers necessários para lidar com o ESP-01 como Shield do Arduino UNO.

```c++
#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
```
Após isso temos o Token e as informações da rede WiFi.

```c++
char auth[] = "Seu token deve ser colado aqui";

char ssid[] = "Coloque o nome da sua rede WiFi";
char pass[] = "Coloque a senha da sua rede WiFi";
```

Seguido das configurações do Esp para com o Arduino
OBS. Caso o ESP não responda tente mudar o valor no ESP8266_BAUD.

```c++
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(3, 4); // RX, TX

#define ESP8266_BAUD 9600  

ESP8266 wifi(&EspSerial);
```

Após temos a definição de um BlynkTimer que será utilizado posteriormente

```c++
BlynkTimer timer;
```

<br>

## Definições iniciais do sensor HC-SR04

```c++
//HC-SR04 setup
#include <Ultrasonic.h>

#define echo A4
#define trig A5

Ultrasonic ultrasonic(trig, echo);

// Conseguindo valor do sensor HC-SR04 em cm
float sensor_hcsr04(){ 
  float valor;
  long micro = ultrasonic.timing();
  valor = ultrasonic.convert(micro, Ultrasonic::CM);
  delay(1000);
  return valor;
}
```

Inicialmente é incluída a biblioteca do sensor e definido os pinos.

```c++
#include <Ultrasonic.h>

#define echo A4
#define trig A5
```

Depois é instânciado um objeto do sensor ultrassônico, que herda da classe base vindo da bibliotecas

```c++
Ultrasonic ultrasonic(trig, echo);
```

É criada uma função que retorna o valor do sensor HC-SR04

```c++
float sensor_hcsr04(){ 
  float valor_distancia;
  long micro = ultrasonic.timing();
  valor_distancia = ultrasonic.convert(micro, Ultrasonic::CM);
  delay(1000);
  return valor_distancia;
}
```

<br>

## Definições iniciais do buzzer

```c++
#define buzzer 5

void ativar_buzzer(float valor_distancia){
  if(valor_distancia < 30){
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
  } else {
    digitalWrite(buzzer, LOW);
  }
}
```

É definida o pino do buzzer e criada uma função que será chamada dando como parâmetro o valor do sensor ultrassônico, se o valor for menor do que 30cm, o buzzer é ligado.

<br>

## Definições iniciais do LED_RGB

```c++
//LED RGB
const int LED[3] = {11,12,13};

void LED_RGB_blue(){
  digitalWrite(LED[0],HIGH);
  digitalWrite(LED[2],LOW);
  digitalWrite(LED[1],LOW);  
}
void LED_RGB_red(){
  digitalWrite(LED[0],LOW);
  digitalWrite(LED[2],HIGH);
  digitalWrite(LED[1],LOW);    
}
void LED_RGB_green(){
  digitalWrite(LED[0],LOW);
  digitalWrite(LED[2],LOW);
  digitalWrite(LED[1],HIGH);
}

void LED_RGB_OFF(){
  digitalWrite(LED[0],LOW);
  digitalWrite(LED[2],LOW);
  digitalWrite(LED[1],LOW);
}

void controle_leds(float valor_distancia){
  if (valor_distancia < 30) {
    LED_RGB_red();
  } else if (valor_distancia < 60){
    LED_RGB_blue();
  } else {
    LED_RGB_green();
  }
}
```

Começamos definindo os pinos do LED RGB

```c++
const int LED[3] = {11,12,13};
```

Depois temos funções com cada uma das três cores, semelhante a essa

```c++
void LED_RGB_blue(){
  digitalWrite(LED[0],HIGH);
  digitalWrite(LED[2],LOW);
  digitalWrite(LED[1],LOW);  
}
```

Também há uma função que desliga o LED.

```c++
void LED_RGB_OFF(){
  digitalWrite(LED[0],LOW);
  digitalWrite(LED[2],LOW);
  digitalWrite(LED[1],LOW);
}
```

Novamente uma função que recebe a distância como parâmetro, caso o valor seja menor que 30cm, o led ficará vermelho, menos que 50cm, pórem maior que 30cm, ficará azul, por fim, se for maior que 50cm, o led ficará verde.

```c++
void controle_leds(float valor_distancia){
  if (valor_distancia < 30) {
    LED_RGB_red();
  } else if (valor_distancia < 60){
    LED_RGB_blue();
  } else {
    LED_RGB_green();
  }
}
```

<br>

## Definições iniciais do sensor DHT11

```c++
//Sensor DHT de temperatura
#include "DHT.h"

#define DHTPIN A3 
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

float dht_temperatura(){
  float temperatura = dht.readTemperature();
  return temperatura;
}

float dht_umidade(){
  float umidade = dht.readHumidity();
  return umidade;
}

//LEDs DHT11
#define led_dht 6
```

Primeiramente há a inclusão da biblioteca do DHT e as definições do pino e tipo de sensor.

```c++
#include "DHT.h"

#define DHTPIN A3 
#define DHTTYPE DHT11 
```

Após isso temos a criação de uma instância da classe DHT proveniente da biblioteca.

```c++
DHT dht(DHTPIN, DHTTYPE);
```

Funções que retornam a temperatura e a umidade.

```c++
float dht_temperatura(){
  float temperatura = dht.readTemperature();
  return temperatura;
}

float dht_umidade(){
  float umidade = dht.readHumidity();
  return umidade;
}
```

E a definição do led do DHT.

```c++
#define led_dht 6
```

<br>

## Definições do ON/OFF dos sensores

```c++
int hcsr04_on_off, dht11_on_off;
```

<br>

## Definições inicias do botão

```c++
//botão
#define botao 2
bool on_off_buzzer = true;

void evento_botao(){
  on_off_buzzer = !on_off_buzzer;
}
```

Há a definição do pino do botão de de uma variável booleana para o ON/OFF do buzzer.

```c++
#define botao 2
bool on_off_buzzer = true;
```

Após isso a função que será chamada na interrupção do botão.
```c++
void evento_botao(){
  on_off_buzzer = !on_off_buzzer;
}
```

<br>

## Função principal

```c++
void myTimerEvent(){  
  if(hcsr04_on_off){
    float distancia = sensor_hcsr04();
    if(on_off_buzzer){
      ativar_buzzer(distancia);
    }
    controle_leds(distancia);
    
    Blynk.virtualWrite(V5, distancia);
  } else {
    Blynk.virtualWrite(V5, 0);
    LED_RGB_OFF();
  }

  if(dht11_on_off){
    float temperatura = dht_temperatura();
    float umidade = dht_umidade();

    digitalWrite(led_dht, HIGH);
    
    Blynk.virtualWrite(V4, temperatura);
    Blynk.virtualWrite(V3, umidade); 
  } else {
    Blynk.virtualWrite(V4, 0);
    Blynk.virtualWrite(V3, 0);

    digitalWrite(led_dht, LOW);
  }
}
```
 Inicialmente verifica-se se o sensor está ligado, logo que a variável hcsr04_on_off é verdadeira, é chamada a função que retorna o valor da distância e atribui esse valor a uma variável.

```c++
  if(hcsr04_on_off){
    float distancia = sensor_hcsr04();
```

Após isso é conferida a variável on_off_buzzer, se for verdadeira o buzzer deve ser ligado, do contrário não.

```c++
    if(on_off_buzzer){
      ativar_buzzer(distancia);
    }
```

É ligado o LED RGB.

```c++
    controle_leds(distancia);
```

Por fim é enviado pelo pino virtual 5(leia a documentação do Blynk APP) a informação para o APP.

```c++
Blynk.virtualWrite(V5, distancia);
```

Caso o sensor esteja desligado, logo, a variável hcsr04_on_off for falsa, será mostrado o valor 0 e o LED é desligado.

```c++
  } else {
    Blynk.virtualWrite(V5, 0);
    LED_RGB_OFF();
  }
```

Em geral a estrutura inicial é semelhante para o DHT11, com a verificação da variável que diz se o sensor está ligado e a atribuição dos valores do sensor em variáveis.

```c++
  if(dht11_on_off){
    float temperatura = dht_temperatura();
    float umidade = dht_umidade();
```

O LED de indicação é ligado.

```c++
    digitalWrite(led_dht, HIGH);
```

Os dados são enviados pelos pinos virtuais para o APP.

```c++
    Blynk.virtualWrite(V4, temperatura);
    Blynk.virtualWrite(V3, umidade);
```

Do contrário o valor enviado será 0 e o LED é desligado.

```c++
  } else {
    Blynk.virtualWrite(V4, 0);
    Blynk.virtualWrite(V3, 0);

    digitalWrite(led_dht, LOW);
  }
}
```

<br>

## Conseguindo os dados provenientes do APP

```c++
BLYNK_WRITE(V1){
  hcsr04_on_off = param.asInt(); 
}

BLYNK_WRITE(V2){
  dht11_on_off = param.asInt(); 
}
```

Recebimento dos dados e atribuição de variáveis.

<br>

## Void setup

```c++
void setup(){
  //buzzer
  pinMode(buzzer, OUTPUT);

  //LED DHT
  pinMode(led_dht, OUTPUT);

  //pinMode leds
  for(int i=11; i<14; i++){
    pinMode(LED[i],OUTPUT);
  }

  //início serial
  Serial.begin(9600);

  dht.begin();

  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);

  attachInterrupt(0,evento_botao,CHANGE);

  timer.setInterval(1000L, myTimerEvent);
}
```

Definição dos pinos do buzzer e LEDs como OUTPUT

```c++
  //buzzer
  pinMode(buzzer, OUTPUT);

  //LED DHT
  pinMode(led_dht, OUTPUT);

  //pinMode leds
  for(int i=11; i<14; i++){
    pinMode(LED[i],OUTPUT);
  }
```

Iniciação da Serial, da instância do DHT, da comunicação com o ESP e o Blynk.

```c++
  //início serial
  Serial.begin(9600);

  dht.begin();

  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
```

Definição da interrupção do botão.

```c++
  attachInterrupt(0,evento_botao,CHANGE);
```

Definição do timer do Blynk. 

```c++
  timer.setInterval(1000L, myTimerEvent);
}
```

<br>

## Void loop

```c++
void loop(){
  Blynk.run();
  timer.run(); 
}
```

No Void loop quando usado o Blynk APP ocorre boa parte da comunicação com o app, deixe apenas o Blynk com método run e o timer nessa parte.

<br><br>

# APP NO BLYNK

Comece baixando o aplicativo e criando uma conta, após isso, crie um novo projeto. Nisso, será enviado no Email cadastrado o Token que deve ser colocado na programação do Arduino.

![APP no Blynk](/img/app_no_blynk/app_no_blynk1.jpeg)

Após coloque os displays que mostrarão os valores.

![APP no Blynk2](/img/app_no_blynk/app_no_blynk2.jpeg)
![APP no Blynk3](/img/app_no_blynk/app_no_blynk3.jpeg)
![APP no Blynk4](/img/app_no_blynk/app_no_blynk4.jpeg)

E os botões para ligar e desligar os sensores.

![APP no Blynk5](/img/app_no_blynk/app_no_blynk5.jpeg)
![APP no Blynk6](/img/app_no_blynk/app_no_blynk6.jpeg)

Após isso basta clicar nese botão para ligar o APP.

![APP no Blynk7](/img/app_no_blynk/app_no_blynk7.jpeg)

