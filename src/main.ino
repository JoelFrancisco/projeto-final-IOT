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

//HC-SR04 setup
#include <Ultrasonic.h>

#define echo A4
#define trig A5

Ultrasonic ultrasonic(trig, echo);

// Conseguindo valor do sensor HC-SR04 em cm
float sensor_hcsr04(){ 
  float valor_distancia;
  long micro = ultrasonic.timing();
  valor_distancia = ultrasonic.convert(micro, Ultrasonic::CM);
  delay(1000);
  return valor_distancia;
}

//Definindo buzzer
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

//On Off
int hcsr04_on_off, dht11_on_off;

//botão
#define botao 2
bool on_off_buzzer = true;

void evento_botao(){
  on_off_buzzer = !on_off_buzzer;
}

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

BLYNK_WRITE(V1){
  hcsr04_on_off = param.asInt(); 
}

BLYNK_WRITE(V2){
  dht11_on_off = param.asInt(); 
}

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

void loop(){
  Blynk.run();
  timer.run(); 
}