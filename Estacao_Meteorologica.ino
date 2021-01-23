//Bibliotecas
#include <Adafruit_BMP085.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <time.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "OpenWeatherMapOneCall.h"

// Pinos
#define TFT_CS         0
#define TFT_RST        2
#define TFT_DC        15
#define TFT_MOSI      13
#define TFT_SCLK      14
#define DHT_PIN       12
#define BOTAO         16 //D0

//controle da cor do tema
boolean dark_theme = true;
uint16_t text_color;
uint16_t back_color;

//controle de tempo de clique
const int SHORT_PRESS_TIME = 1000;
long pressedTime = 0;
int action = 0;
int lastState = LOW;

//controle de troca de tela
int passo = 0;
boolean desenha = true;
long temporizador;

//WiFi - Definir no arquivo config.c
extern const char* WIFI_SSID;
extern const char* WIFI_PASSWORD;

WiFiClient wifiClient;

// Definições Cliente NTP
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = -10800;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

//Definições Open Weather Map 
extern const char* OPEN_WEATHER_MAP_APP_ID; 
// - Definir no arquivo config.c
extern float OPEN_WEATHER_MAP_LOCATTION_LAT;
extern float OPEN_WEATHER_MAP_LOCATTION_LON;
String OPEN_WEATHER_MAP_LANGUAGE = "pt";
boolean IS_METRIC = true;

OpenWeatherMapOneCallData openWeatherMapOneCallData;

// Variáveis de consulta do clima
long temporizador_clima;
const long UPDATE_TIME = 1200000;
boolean consulta;

String weatherMain[4];
int tempMin[4];
int tempMax[4];
float rain[4];
float uvi[4];
String wday[4];
int mday[4];
String mon[4];

// Definições Dias da Semana e Mês
const String WDAY_NAMES[] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};
const String MONTH_NAMES[] = {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"};

// Definições Display
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Definições Sensor DHT11
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

// Definições Sensor BMP180
Adafruit_BMP085 bmp;

// Definição de cores
#define PRETO     0x0000
#define BRANCO    0xFFFF
#define CHUMBO    0x4208
#define TEMP1     0xCE39
#define TEMP2     0xEBF0
#define UMID1     0x7DDD
#define UMID2     0xCF1F
#define PRES1     0x7DDD
#define PRES2     0xCF1F
#define SOL1      0xFC29
#define SOL2      0xFE0C
#define SEN1      0xFE8E
#define SEN2      0xD6FD
#define NUB1      0x8451
#define NUB2      0xD6FD
#define CHU1      0x43B3
#define CHU2      0x7DDD
#define TEM1      0xB659
#define TEM2      0xFE08
#define TEM3      0xA65F
#define AZUL      0x001F
#define VERDE     0x07E0
#define VERMELHO  0xF800

// Caracteres externos
extern unsigned char temperatura1[];
extern unsigned char temperatura2[];
extern unsigned char temperatura3[];
extern unsigned char umidade1[];
extern unsigned char umidade2[];
extern unsigned char umidade3[];
extern unsigned char pressao1[];
extern unsigned char pressao2[];
extern unsigned char pressao3[];
extern unsigned char sol1[];
extern unsigned char sol2[];
extern unsigned char sol3[];
extern unsigned char solenuvem1[];
extern unsigned char solenuvem2[];
extern unsigned char solenuvem3[];
extern unsigned char nublado1[];
extern unsigned char nublado2[];
extern unsigned char nublado3[];
extern unsigned char chuva1[];
extern unsigned char chuva2[];
extern unsigned char chuva3[];
extern unsigned char tempestade1[];
extern unsigned char tempestade2[];
extern unsigned char tempestade3[];
extern unsigned char tempestade4[];
extern unsigned char wifi[];
extern unsigned char tapa_texto[];
extern unsigned char cancel[];
extern unsigned char wifi_3[];
extern unsigned char wifi_2[];
extern unsigned char wifi_1[];
extern unsigned char wifi_0[];
extern unsigned char seta_cima[];
extern unsigned char seta_cima2[];
extern unsigned char seta_baixo[];
extern unsigned char seta_baixo2[];
extern unsigned char circulo1[];
extern unsigned char circulo2[];


void connectWifi() {
  int tentativa;

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando...");
  Serial.println(WIFI_SSID);
  
  tft.fillScreen(back_color);
  //            Hor, Ver, img, Lar, Alt, cor
  tft.drawBitmap(40, 72, wifi, 44, 22, text_color);
  
  tft.setTextSize(1);
  tft.setTextColor(text_color);
  tft.setCursor(25,150);
  tft.print("Conectando...");
  
  tentativa = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tentativa += 1;
    if (tentativa > 5 ){      
      break;
    }
  }
  Serial.println("");

  tft.drawBitmap(0, 150, tapa_texto, 128, 5, back_color);
  tft.drawBitmap(0, 155, tapa_texto, 128, 5, back_color);
  
  if (WiFi.status() != WL_CONNECTED) {
    tft.drawBitmap(30, 50, cancel, 64, 64, VERMELHO);    
    tft.setCursor(30,150);
    tft.print("Modo Offline!");
    Serial.println("Modo Offiline!");
  }else{
    tft.setCursor(40,150);
    tft.print("Conectado!");
    Serial.println("Conectado!");
    Serial.println(WiFi.localIP());
  }
  
  Serial.println();
  
  delay(1000);
}

void data(){
  
  String weekDay = WDAY_NAMES[timeClient.getDay()];
  timeClient.update();
  
  tft.setTextSize(1);
  tft.setTextColor(text_color);

  if (WiFi.status() != WL_CONNECTED) {
    tft.setCursor(30,2);
    tft.print("Modo Offline");
  }else{
    tft.setCursor(15,2);
    tft.print(weekDay);
    tft.print(", ");    
    
    unsigned long epochTime = timeClient.getEpochTime();
    struct tm*ptm = gmtime ((time_t *)&epochTime); 
    int monthDay = ptm->tm_mday;
    tft.print(monthDay);
    
    tft.print(" ");
    
    int currentMonth = ptm -> tm_mon + 1;
    String currentMonthName = MONTH_NAMES[currentMonth-1];
    int currentYear = ptm -> tm_year + 1900;
    tft.print(currentMonthName);
    
    tft.print(" ");
    
    tft.print(currentYear);
    if ( WiFi.RSSI() > -40 ) {
      tft.drawBitmap(120, 0, wifi_3, 8, 8, text_color);
    } else if ( WiFi.RSSI() > -60 ) {
      tft.drawBitmap(120, 0, wifi_2, 8, 8, text_color);
    } else if ( WiFi.RSSI() > -80 ) {
      tft.drawBitmap(120, 0, wifi_1, 8, 8, text_color);
    } else {
      tft.drawBitmap(120, 0, wifi_0, 8, 8, text_color);
    }
    
  }
  
}

void horario(){
  
  if (WiFi.status() != WL_CONNECTED) {
    
  }else{
    tft.setCursor(35,15);
    tft.setTextSize(2);
    
    int currentHour = timeClient.getHours();
    int currentMinute = timeClient.getMinutes();
    
    if (currentHour < 10){
      tft.print("0");
      tft.print(currentHour);
    }
    else {
      tft.print(currentHour); 
    } 
    tft.print(":");
    if (currentMinute < 10){
      tft.print("0");
      tft.print(currentMinute);
    }
    else {
      tft.print(currentMinute);
    }
  }
}

void temperatura(){

  tft.drawBitmap(16,32,temperatura1,96,96,TEMP1);
  tft.drawBitmap(16,32,temperatura2,96,96,TEMP2);
  tft.drawBitmap(16,32,temperatura3,96,96,PRETO);
  float t = bmp.readTemperature();
  tft.setCursor (30,130);
  tft.setTextSize(1);
  tft.print("Temperatura");
  tft.setCursor(25,140);
  tft.setTextSize(2);
  tft.print(t);
  tft.print(" C");
  tft.setCursor(90,138);
  tft.setTextSize(1);
  tft.print("o");
  
}

void umidade(){

  tft.drawBitmap(16,32,umidade2,96,96,UMID2);
  tft.drawBitmap(16,32,umidade1,96,96,UMID1);
  tft.drawBitmap(16,32,umidade3,96,96,PRETO);
  float h = dht.readHumidity();
  tft.setCursor (45,130);
  tft.setTextSize(1);
  tft.print("Umidade");
  tft.setCursor(32,140);
  tft.setTextSize(2);
  tft.print(h);
  tft.print("%");
}

void pressao(){

  tft.drawBitmap(16,32,pressao1,96,96,PRES1);
  tft.drawBitmap(16,32,pressao2,96,96,PRES2);
  tft.drawBitmap(16,32,pressao3,96,96,PRETO);
  float t = bmp.readPressure();
  tft.setCursor (45,130);
  tft.setTextSize(1);
  tft.print("Pressao");
  tft.setCursor(3,140);
  tft.setTextSize(2);
  tft.print(t/100);
  tft.print(" hPa"); 
}

void consultar_previsao() {

  Serial.print("Consultando API...");
  
  tft.drawBitmap(2, 2, circulo1, 8, 8, VERMELHO);
  tft.drawBitmap(2, 2, circulo2, 8, 8, PRETO);
  
  OpenWeatherMapOneCall *oneCallClient = new OpenWeatherMapOneCall();
  oneCallClient->setMetric(IS_METRIC);
  oneCallClient->setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  
  oneCallClient->update(&openWeatherMapOneCallData, String(OPEN_WEATHER_MAP_APP_ID), OPEN_WEATHER_MAP_LOCATTION_LAT, OPEN_WEATHER_MAP_LOCATTION_LON);
  delete oneCallClient;
  oneCallClient = nullptr;

  time_t observationTimestamp;
  struct tm* timeInfo;

  for (int i = 0; i <= 3; i++) {
    observationTimestamp = openWeatherMapOneCallData.daily[i].dt;
    timeInfo = localtime(&observationTimestamp);
    wday[i] = WDAY_NAMES[timeInfo->tm_wday];
    mday[i] = (timeInfo->tm_mday);
    mon[i] = MONTH_NAMES[timeInfo->tm_mon];
    tempMin[i] = openWeatherMapOneCallData.daily[i].tempMin;
    tempMax[i] = openWeatherMapOneCallData.daily[i].tempMax;
    weatherMain[i] = openWeatherMapOneCallData.daily[i].weatherMain;
    rain[i] = openWeatherMapOneCallData.daily[i].rain;
    uvi[i] = openWeatherMapOneCallData.daily[i].uvi;
  }
  
  consulta = true;
  tft.drawBitmap(2, 2, circulo1, 8, 8, VERDE);
}

void exibir_previsao1() {
  //primeiro dia
  int posicaoX, posicaoY;
  posicaoX=40;
  posicaoY=40;
  
  tft.setTextSize(1);
  
  if (weatherMain[0] == "Thunderstorm"){
    tft.drawBitmap(posicaoX, posicaoY,tempestade1,48,48,TEM1);
    tft.drawBitmap(posicaoX, posicaoY,tempestade2,48,48,TEM2);
    tft.drawBitmap(posicaoX, posicaoY,tempestade3,48,48,TEM3);
    tft.drawBitmap(posicaoX, posicaoY,tempestade4,48,48,PRETO);
    posicaoY += 45;
    tft.setCursor(45, posicaoY);
    tft.print("Tempestade");
  }
  else if (weatherMain[0] == "Rain"){
    tft.drawBitmap(posicaoX, posicaoY,chuva1,48,48,CHU1);
    tft.drawBitmap(posicaoX, posicaoY,chuva2,48,48,CHU2);
    tft.drawBitmap(posicaoX, posicaoY,chuva3,48,48,PRETO);
    posicaoY += 45;
    tft.setCursor(47, posicaoY);
    tft.print("Chuva");
  }
  else if(weatherMain[0] == "Drizzle"){
    tft.drawBitmap(posicaoX, posicaoY,chuva1,48,48,CHU1);
    tft.drawBitmap(posicaoX, posicaoY,chuva2,48,48,CHU2);
    tft.drawBitmap(posicaoX, posicaoY,chuva3,48,48,PRETO);
    posicaoY += 45;
    tft.setCursor(43, posicaoY);
    tft.print("Pouca Chuva");
  }
  else if(weatherMain[0] == "Clouds"){
    tft.drawBitmap(posicaoX, posicaoY,nublado1,48,48,NUB1);
    tft.drawBitmap(posicaoX, posicaoY,nublado2,48,48,NUB2);
    tft.drawBitmap(posicaoX, posicaoY,nublado3,48,48,PRETO);
    posicaoY += 45;
    tft.setCursor(47, posicaoY);
    tft.print("Nublado");
  }
  else if(weatherMain[0] == "Clear"){
    tft.drawBitmap(posicaoX, posicaoY,sol1,48,48,SOL1);
    tft.drawBitmap(posicaoX, posicaoY,sol2,48,48,SOL2);
    tft.drawBitmap(posicaoX, posicaoY,sol3,48,48,PRETO);
    posicaoY += 45;
    tft.setCursor(52, posicaoY);
    tft.print("Sol");
  }
  
  posicaoY += 10;
  /*
  tft.setCursor(27, posicaoY);
  tft.print(wday[0]);
  tft.print(" (");
  tft.print(mday[0]);
  tft.print("/");
  tft.print(mon[0]);
  tft.print(")");
  */
  posicaoY += 10;
  tft.drawBitmap(27, posicaoY, seta_baixo, 8, 8, VERDE);
  tft.drawBitmap(27, posicaoY, seta_baixo2, 8, 8, PRETO);
  
  tft.setCursor(42, posicaoY);
  tft.print(tempMin[0]);
  
  tft.drawBitmap(70, posicaoY, seta_cima, 8, 8, VERMELHO);
  tft.drawBitmap(70, posicaoY, seta_cima2, 8, 8, PRETO);

  tft.setCursor(85, posicaoY);
  tft.print(tempMax[0]);

  posicaoY += 10;
  tft.setCursor(10, posicaoY);
  tft.print((String)"UV " + uvi[0]);
  tft.setCursor(70, posicaoY);
  tft.print((String)" " + rain[0] + "mm");
  
  tft.setTextSize(2);
  posicaoY += 20;  
  tft.setCursor(40, posicaoY);
  tft.print("HOJE");
}

void exibir_previsao2() {
  
  //Segundo dia
  tft.setTextSize(1);
  
  tft.setCursor(47,35);
  tft.print(wday[1]);
  tft.print(" (");
  tft.print(mday[1]);
  tft.print("/");
  tft.print(mon[1]);
  tft.print(")");
  
  tft.drawBitmap(47, 45, seta_baixo, 8, 8, VERDE);
  tft.drawBitmap(47, 45, seta_baixo2, 8, 8, PRETO);
  
  tft.setCursor(62,45);
  tft.print(tempMin[1]);
  
  tft.drawBitmap(90, 45, seta_cima, 8, 8, VERMELHO);
  tft.drawBitmap(90, 45, seta_cima2, 8, 8, PRETO);
  
  tft.setCursor(105,45);
  tft.print(tempMax[1]);
  
  if (weatherMain[1] == "Thunderstorm"){
    tft.setCursor(47,55);
    tft.print("Tempestade");   
    tft.print((String)" " + (int)rain[1] + "mm");
    
    tft.drawBitmap(3,30,tempestade1,48,48,TEM1);
    tft.drawBitmap(3,30,tempestade2,48,48,TEM2);
    tft.drawBitmap(3,30,tempestade3,48,48,TEM3);
    tft.drawBitmap(3,30,tempestade4,48,48,PRETO);
  }
  else if (weatherMain[1] == "Rain"){
    tft.setCursor(47,55);
    tft.print("Chuva");
    tft.print((String)" " + rain[1] + "mm");
    
    tft.drawBitmap(3,30,chuva1,48,48,CHU1);
    tft.drawBitmap(3,30,chuva2,48,48,CHU2);
    tft.drawBitmap(3,30,chuva3,48,48,PRETO);
  }
  else if(weatherMain[1] == "Drizzle"){
    tft.setCursor(47,55);
    tft.print("Pouca Chuva");
    //tft.print((String)" " + (int)rain[1] + "mm");
    
    tft.drawBitmap(3,30,chuva1,48,48,CHU1);
    tft.drawBitmap(3,30,chuva2,48,48,CHU2);
    tft.drawBitmap(3,30,chuva3,48,48,PRETO);
  }
  else if(weatherMain[1] == "Clouds"){
    tft.setCursor(47,55);
    tft.print("Nublado");
    tft.print((String)" UV " + (int)uvi[1] );
    
    tft.drawBitmap(3,30,nublado1,48,48,NUB1);
    tft.drawBitmap(3,30,nublado2,48,48,NUB2);
    tft.drawBitmap(3,30,nublado3,48,48,PRETO);
  }
  else if(weatherMain[1] == "Clear"){
    tft.setCursor(47,55);
    tft.print("Sol");
    tft.print((String)"  UV " + uvi[1] );
    
    tft.drawBitmap(3,30,sol1,48,48,SOL1);
    tft.drawBitmap(3,30,sol2,48,48,SOL2);
    tft.drawBitmap(3,30,sol3,48,48,PRETO);
  }
  
  //Terceiro dia
  tft.setCursor(47,80);
  tft.print(wday[2]);
  tft.print(" (");
  tft.print(mday[2]);
  tft.print("/");
  tft.print(mon[2]);
  tft.print(")");
  
  tft.drawBitmap(47, 90, seta_baixo, 8, 8, VERDE);
  tft.drawBitmap(47, 90, seta_baixo2, 8, 8, PRETO);
  
  tft.setCursor(62,90);
  tft.print(tempMin[2]);
  
  tft.drawBitmap(90, 90, seta_cima, 8, 8, VERMELHO);
  tft.drawBitmap(90, 90, seta_cima2, 8, 8, PRETO);

  tft.setCursor(105,90);  
  tft.print(tempMax[2]);
  
  if (weatherMain[2] == "Thunderstorm"){
    tft.setCursor(47,100);
    tft.print("Tempestade");
    tft.print((String)" " + (int)rain[2] + "mm");
    
    tft.drawBitmap(3,75,tempestade1,48,48,TEM1);
    tft.drawBitmap(3,75,tempestade2,48,48,TEM2);
    tft.drawBitmap(3,75,tempestade3,48,48,TEM3);
    tft.drawBitmap(3,75,tempestade4,48,48,PRETO);
  }
  else if (weatherMain[2] == "Rain"){
    tft.setCursor(47,100);
    tft.print("Chuva");
    tft.print((String)" " + rain[2] + "mm");
    
    tft.drawBitmap(3,75,chuva1,48,48,CHU1);
    tft.drawBitmap(3,75,chuva2,48,48,CHU2);
    tft.drawBitmap(3,75,chuva3,48,48,PRETO);
  }
  else if(weatherMain[2] == "Drizzle"){
    tft.setCursor(47,100);
    tft.print("Pouca Chuva");
    //tft.print((String)" " + (int)rain[2] + "mm");
    
    tft.drawBitmap(3,75,chuva1,48,48,CHU1);
    tft.drawBitmap(3,75,chuva2,48,48,CHU2);
    tft.drawBitmap(3,75,chuva3,48,48,PRETO);
  }
  else if(weatherMain[2] == "Clouds"){
    tft.setCursor(47,100);
    tft.print("Nublado");
    tft.print((String)" UV " + (int)uvi[2] );
    
    tft.drawBitmap(3,75,nublado1,48,48,NUB1);
    tft.drawBitmap(3,75,nublado2,48,48,NUB2);
    tft.drawBitmap(3,75,nublado3,48,48,PRETO);
  }
  else if(weatherMain[2] == "Clear"){
    tft.setCursor(47, 100);
    tft.print("Sol");
    tft.print((String)"  UV " + uvi[2] );
    
    tft.drawBitmap(3,75,sol1,48,48,SOL1);
    tft.drawBitmap(3,75,sol2,48,48,SOL2);
    tft.drawBitmap(3,75,sol3,48,48,PRETO);
  }

  //Quarto dia
  tft.setTextSize(1);
  tft.setCursor(47,125);
  tft.print(wday[3]);
  tft.print(" (");
  tft.print(mday[3]);
  tft.print("/");
  tft.print(mon[3]);
  tft.print(")");
  
  tft.drawBitmap(47, 135, seta_baixo, 8, 8, VERDE);
  tft.drawBitmap(47, 135, seta_baixo2, 8, 8, PRETO);
  
  tft.setCursor(62,135);
  tft.print(tempMin[3]);
  
  tft.drawBitmap(90, 135, seta_cima, 8, 8, VERMELHO);
  tft.drawBitmap(90, 135, seta_cima2, 8, 8, PRETO);

  tft.setCursor(105, 135);
  tft.print(tempMax[3]);
  
  if (weatherMain[3] == "Thunderstorm"){
    tft.setCursor(47,145);
    tft.print("Tempestade");
    tft.print((String)" " + (int)rain[3] + "mm");
    
    tft.drawBitmap(3,120,tempestade1,48,48,TEM1);
    tft.drawBitmap(3,120,tempestade2,48,48,TEM2);
    tft.drawBitmap(3,120,tempestade3,48,48,TEM3);
    tft.drawBitmap(3,120,tempestade4,48,48,PRETO);
  }
  else if (weatherMain[3] == "Rain"){
    tft.setCursor(47,145);
    tft.print("Chuva");
    tft.print((String)" " + rain[3] + "mm");
    
    tft.drawBitmap(3,120,chuva1,48,48,CHU1);
    tft.drawBitmap(3,120,chuva2,48,48,CHU2);
    tft.drawBitmap(3,120,chuva3,48,48,PRETO);
  }
  else if(weatherMain[3] == "Drizzle"){
    tft.setCursor(47,145);
    tft.print("Pouca Chuva");
    //tft.print((String)" " + (int)rain[3] + "mm");
    
    tft.drawBitmap(3,120,chuva1,48,48,CHU1);
    tft.drawBitmap(3,120,chuva2,48,48,CHU2);
    tft.drawBitmap(3,120,chuva3,48,48,PRETO);
  }
  else if(weatherMain[3] == "Clouds"){
    tft.setCursor(47,145);
    tft.print("Nublado");
    tft.print((String)" UV " + (int)uvi[3] );
    
    tft.drawBitmap(3,120,nublado1,48,48,NUB1);
    tft.drawBitmap(3,120,nublado2,48,48,NUB2);
    tft.drawBitmap(3,120,nublado3,48,48,PRETO);
  }
  else if(weatherMain[3] == "Clear"){
    tft.setCursor(47, 145);
    tft.print("Sol");
    tft.print((String)"  UV " + uvi[3] );
    
    tft.drawBitmap(3,120,sol1,48,48,SOL1);
    tft.drawBitmap(3,120,sol2,48,48,SOL2);
    tft.drawBitmap(3,120,sol3,48,48,PRETO);
  }
  
}


void setup() {

  // Inicialização comunicação serial
  Serial.begin(115200);
  pinMode(BOTAO, INPUT);
  // Inicialização display
  tft.initR(INITR_BLACKTAB);
  
  if (dark_theme == true){
    text_color = BRANCO;
    back_color = CHUMBO;
  }else{
    text_color = PRETO;
    back_color = BRANCO;
  }
  
  // Inicialização DHT11
  dht.begin();
  
  // Inicialização BMP180
  bmp.begin();
  
  // Inicialização WiFi
  connectWifi();
  // Inicialização cliente NTP
  timeClient.begin();
  timeClient.update();
  
  passo = 0;
  desenha = true;
  temporizador = millis();
  temporizador_clima = UPDATE_TIME * -1;
  consulta = false;
}

void loop() {

  action = 0;
  
  if (digitalRead(BOTAO) == HIGH) {
    
    if ( lastState == LOW ) {
       Serial.println("Botao marcado como precionado!");
       pressedTime = millis();
       lastState = HIGH;
       
    }
    
  }else if ( lastState == HIGH && millis() > pressedTime + 100){
    Serial.println("Botao solto!");
    
    if( (millis() - pressedTime) <= SHORT_PRESS_TIME ){
      action = 1;
      Serial.println("Acao 1");
    }else{
      action = 2;
      Serial.println("Acao 2");
    }
    
    lastState = LOW;
    pressedTime = 0;
  }
  
  if (action == 1){
    
    temporizador = 0;
    desenha = false;
    if ( passo == 3 ) passo = 4; //pula exibe previsão 1
    if ( passo == 5 ) passo = -1; //pula exibe previsão 2
    
  }else if (action == 2 ){
    
    if ( dark_theme == true ){
      text_color = PRETO;
      back_color = BRANCO;
      dark_theme = false;
    }else{
      text_color = BRANCO;
      back_color = CHUMBO;
      dark_theme = true;
    }    
    
  }
  
  
  if ( desenha == false ){
    if ( millis() >= temporizador + 15000 ) {
      desenha = true;
      passo += 1;
      temporizador = millis();
    }
  }
  
  if ( passo == 0 && desenha ){
    tft.fillScreen(back_color);
    data();
    horario();
    temperatura();    
    desenha = false;
    
  } else if ( passo == 1 && desenha ){
    tft.fillScreen(back_color);
    data();
    horario(); 
    umidade();
    desenha = false;
  
  } else if ( passo == 2 && desenha ){  
    tft.fillScreen(back_color);
    data();
    horario(); 
    pressao();
    desenha = false;
    
  } else if ( passo == 3 && desenha ) {
    if (consulta == true) { 
      tft.fillScreen(back_color);
      data();
      horario();
      exibir_previsao1();
      desenha = false;
    } else { 
      passo = -1;
      temporizador = 0;
      desenha = false;
    }
  } else if ( passo == 4 && desenha ) {
    tft.drawBitmap(0, 15, tapa_texto, 128, 5, back_color);
    tft.drawBitmap(0, 20, tapa_texto, 128, 5, back_color);
    tft.drawBitmap(0, 25, tapa_texto, 128, 5, back_color);
    horario();
    
    desenha = false;
  } else if ( passo == 5 && desenha ) {
    if (consulta == true) { 
      tft.fillScreen(back_color);
      data();
      horario();
      exibir_previsao2();
      desenha = false;
    } else { 
      passo = -1;
      temporizador = 0;
      desenha = false;
    }
  } else if ( passo == 6 && desenha ) {
    tft.drawBitmap(0, 15, tapa_texto, 128, 5, back_color);
    tft.drawBitmap(0, 20, tapa_texto, 128, 5, back_color);
    tft.drawBitmap(0, 25, tapa_texto, 128, 5, back_color);
    horario();
    
    desenha = false;
    passo = -1;
  }

  if ( temporizador_clima + UPDATE_TIME <= millis() ) {
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Consultando previsão!");
        consultar_previsao();
        temporizador_clima = millis();
      }
  }

}
