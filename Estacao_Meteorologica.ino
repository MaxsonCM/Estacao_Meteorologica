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
#define BOTAO         16

//controle da cor do tema
boolean dark_theme = true;
uint16_t text_color;
uint16_t back_color;

//controle de tempo de clique
const int SHORT_PRESS_TIME = 1000;
long pressedTime  = 0;
int action = 0;
int lastState = LOW;

//controle de troca de tela
int passo = 0;
boolean desenha = true;
unsigned long temporizador;

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

// Definições Dias da Semana e Mês
const String WDAY_NAMES[] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};
const String MONTH_NAMES[] = {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"};

// Definições Display
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Definições DHT11
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

// Definições BMP180
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


void previsao() {
  tft.setTextSize(1);
  
  OpenWeatherMapOneCall *oneCallClient = new OpenWeatherMapOneCall();
  oneCallClient->setMetric(IS_METRIC);
  oneCallClient->setLanguage(OPEN_WEATHER_MAP_LANGUAGE);

  long executionStart = millis();
  oneCallClient->update(&openWeatherMapOneCallData, String(OPEN_WEATHER_MAP_APP_ID), OPEN_WEATHER_MAP_LOCATTION_LAT, OPEN_WEATHER_MAP_LOCATTION_LON);
  delete oneCallClient;
  oneCallClient = nullptr;
  float pop;
  float uvi;
  time_t time;

  time_t observationTimestamp1 = openWeatherMapOneCallData.daily[1].dt;
  struct tm* timeInfo1;
  timeInfo1 = localtime(&observationTimestamp1);
  tft.setCursor(47,35);
  tft.print(WDAY_NAMES[timeInfo1->tm_wday]);
  tft.print(" (");
  tft.print(timeInfo1->tm_mday);
  tft.print("/");
  tft.print(MONTH_NAMES[timeInfo1->tm_mon]);
  tft.print(")");
  
  tft.drawBitmap(47, 45, seta_baixo, 8, 8, VERDE);
  tft.drawBitmap(47, 45, seta_baixo2, 8, 8, PRETO);
  
  int tmin1 = openWeatherMapOneCallData.daily[1].tempMin;
  tft.setCursor(62,45);
  tft.print(tmin1);
  
  tft.drawBitmap(90, 45, seta_cima, 8, 8, VERMELHO);
  tft.drawBitmap(90, 45, seta_cima2, 8, 8, PRETO);
  
  tft.setCursor(105,45);
  int tmax1 = openWeatherMapOneCallData.daily[1].tempMax;
  tft.print(tmax1);
  
  String cond1 = openWeatherMapOneCallData.daily[1].weatherMain;
  pop = openWeatherMapOneCallData.daily[1].rain;
  uvi = openWeatherMapOneCallData.daily[1].uvi;
  
  if (cond1 == "Thunderstorm"){
    tft.setCursor(47,55);
    tft.print("Tempestade");   
    
    tft.drawBitmap(3,30,tempestade1,48,48,TEM1);
    tft.drawBitmap(3,30,tempestade2,48,48,TEM2);
    tft.drawBitmap(3,30,tempestade3,48,48,TEM3);
    tft.drawBitmap(3,30,tempestade4,48,48,PRETO);
  }
  else if (cond1 == "Rain"){
    tft.setCursor(47,55);
    tft.print("Chuva");
    tft.print((String)" " + pop + "mm");
    
    tft.drawBitmap(3,30,chuva1,48,48,CHU1);
    tft.drawBitmap(3,30,chuva2,48,48,CHU2);
    tft.drawBitmap(3,30,chuva3,48,48,PRETO);
  }
  else if(cond1 == "Drizzle"){
    tft.setCursor(47,55);
    tft.print("Pouca Chuva");
    //tft.print((String)" " + pop + "mm");
    
    tft.drawBitmap(3,30,chuva1,48,48,CHU1);
    tft.drawBitmap(3,30,chuva2,48,48,CHU2);
    tft.drawBitmap(3,30,chuva3,48,48,PRETO);
  }
  else if(cond1 == "Clouds"){
    tft.setCursor(47,55);
    tft.print("Nublado");
    //tft.print((String)"  UV " + uvi );
    
    tft.drawBitmap(3,30,nublado1,48,48,NUB1);
    tft.drawBitmap(3,30,nublado2,48,48,NUB2);
    tft.drawBitmap(3,30,nublado3,48,48,PRETO);
  }
  else if(cond1 == "Clear"){
    tft.setCursor(47,55);
    tft.print("Sol");
    tft.print((String)"  UV " + uvi );
    
    tft.drawBitmap(3,30,sol1,48,48,SOL1);
    tft.drawBitmap(3,30,sol2,48,48,SOL2);
    tft.drawBitmap(3,30,sol3,48,48,PRETO);
  }
  
  time_t observationTimestamp2 = openWeatherMapOneCallData.daily[2].dt;
  struct tm* timeInfo2;
  timeInfo2 = localtime(&observationTimestamp2);
  tft.setCursor(47,80);
  tft.print(WDAY_NAMES[timeInfo2->tm_wday]);
  tft.print(" (");
  tft.print(timeInfo2->tm_mday);
  tft.print("/");
  tft.print(MONTH_NAMES[timeInfo2->tm_mon]);
  tft.print(")");
  
  tft.drawBitmap(47, 90, seta_baixo, 8, 8, VERDE);
  tft.drawBitmap(47, 90, seta_baixo2, 8, 8, PRETO);
  
  tft.setCursor(62,90);
  int tmin2 = openWeatherMapOneCallData.daily[2].tempMin;
  tft.print(tmin2);
  
  tft.drawBitmap(90, 90, seta_cima, 8, 8, VERMELHO);
  tft.drawBitmap(90, 90, seta_cima2, 8, 8, PRETO);

  tft.setCursor(105,90);
  int tmax2 = openWeatherMapOneCallData.daily[2].tempMax;
  tft.print(tmax2);
  String cond2 = openWeatherMapOneCallData.daily[2].weatherMain;
  pop = openWeatherMapOneCallData.daily[2].rain;
  uvi = openWeatherMapOneCallData.daily[2].uvi;
  
  if (cond2 == "Thunderstorm"){
    tft.setCursor(47,100);
    tft.print("Tempestade");
    tft.print((String)" " + pop + "mm");
    
    tft.drawBitmap(3,75,tempestade1,48,48,TEM1);
    tft.drawBitmap(3,75,tempestade2,48,48,TEM2);
    tft.drawBitmap(3,75,tempestade3,48,48,TEM3);
    tft.drawBitmap(3,75,tempestade4,48,48,PRETO);
  }
  else if (cond2 == "Rain"){
    tft.setCursor(47,100);
    tft.print("Chuva");
    tft.print((String)" " + pop + "mm");
    
    tft.drawBitmap(3,75,chuva1,48,48,CHU1);
    tft.drawBitmap(3,75,chuva2,48,48,CHU2);
    tft.drawBitmap(3,75,chuva3,48,48,PRETO);
  }
  else if(cond2 == "Drizzle"){
    tft.setCursor(47,100);
    tft.print("Pouca Chuva");
    tft.print((String)" " + pop + "mm");
    
    tft.drawBitmap(3,75,chuva1,48,48,CHU1);
    tft.drawBitmap(3,75,chuva2,48,48,CHU2);
    tft.drawBitmap(3,75,chuva3,48,48,PRETO);
  }
  else if(cond2 == "Clouds"){
    tft.setCursor(47,100);
    tft.print("Nublado");
    tft.print((String)" UV " + uvi );
    
    tft.drawBitmap(3,75,nublado1,48,48,NUB1);
    tft.drawBitmap(3,75,nublado2,48,48,NUB2);
    tft.drawBitmap(3,75,nublado3,48,48,PRETO);
  }
  else if(cond2 == "Clear"){
    tft.setCursor(47, 100);
    tft.print("Sol");
    tft.print((String)"  UV " + uvi );
    
    tft.drawBitmap(3,75,sol1,48,48,SOL1);
    tft.drawBitmap(3,75,sol2,48,48,SOL2);
    tft.drawBitmap(3,75,sol3,48,48,PRETO);
  }

  time_t observationTimestamp3 = openWeatherMapOneCallData.daily[3].dt;
  struct tm* timeInfo3;
  timeInfo3 = localtime(&observationTimestamp3);
  tft.setTextSize(1);
  tft.setCursor(47,125);
  tft.print(WDAY_NAMES[timeInfo3->tm_wday]);
  tft.print(" (");
  tft.print(timeInfo3->tm_mday);
  tft.print("/");
  tft.print(MONTH_NAMES[timeInfo3->tm_mon]);
  tft.print(")");
  
  tft.drawBitmap(47, 135, seta_baixo, 8, 8, VERDE);
  tft.drawBitmap(47, 135, seta_baixo2, 8, 8, PRETO);
  
  tft.setCursor(62,135);
  int tmin3 = openWeatherMapOneCallData.daily[3].tempMin;
  tft.print(tmin3);
  
  tft.drawBitmap(90, 135, seta_cima, 8, 8, VERMELHO);
  tft.drawBitmap(90, 135, seta_cima2, 8, 8, PRETO);

  tft.setCursor(105, 135);
  int tmax3 = openWeatherMapOneCallData.daily[3].tempMax;
  tft.print(tmax3);
  String cond3 = openWeatherMapOneCallData.daily[3].weatherMain;
  pop = openWeatherMapOneCallData.daily[3].rain;
  uvi = openWeatherMapOneCallData.daily[3].uvi;
  
  if (cond3 == "Thunderstorm"){
    tft.setCursor(47,145);
    tft.print("Tempestade");
    //tft.print((String)" " + pop + "mm");
    
    tft.drawBitmap(3,120,tempestade1,48,48,TEM1);
    tft.drawBitmap(3,120,tempestade2,48,48,TEM2);
    tft.drawBitmap(3,120,tempestade3,48,48,TEM3);
    tft.drawBitmap(3,120,tempestade4,48,48,PRETO);
  }
  else if (cond3 == "Rain"){
    tft.setCursor(47,145);
    tft.print("Chuva");
    tft.print((String)" " + pop + "mm");
    
    tft.drawBitmap(3,120,chuva1,48,48,CHU1);
    tft.drawBitmap(3,120,chuva2,48,48,CHU2);
    tft.drawBitmap(3,120,chuva3,48,48,PRETO);
  }
  else if(cond3 == "Drizzle"){
    tft.setCursor(47,145);
    tft.print("Pouca Chuva");
    //tft.print((String)" " + pop + "mm");
    
    tft.drawBitmap(3,120,chuva1,48,48,CHU1);
    tft.drawBitmap(3,120,chuva2,48,48,CHU2);
    tft.drawBitmap(3,120,chuva3,48,48,PRETO);
  }
  else if(cond3 == "Clouds"){
    tft.setCursor(47,145);
    tft.print("Nublado");
    //tft.print((String)" UV " + uvi );
    
    tft.drawBitmap(3,120,nublado1,48,48,NUB1);
    tft.drawBitmap(3,120,nublado2,48,48,NUB2);
    tft.drawBitmap(3,120,nublado3,48,48,PRETO);
  }
  else if(cond3 == "Clear"){
    tft.setCursor(47, 145);
    tft.print("Sol");
    tft.print((String)"  UV " + uvi );
    
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
    if ( passo >= 3 ) passo = -1;
    
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
    if (WiFi.status() == WL_CONNECTED) {
      tft.fillScreen(back_color);
      data();
      horario();
      previsao();
      desenha = false;
      //temporizador = millis();
    } else { 
      passo = -1;
    }
  } else if ( passo == 4 && desenha ) {
    tft.drawBitmap(0, 15, tapa_texto, 128, 5, back_color);
    tft.drawBitmap(0, 20, tapa_texto, 128, 5, back_color);
    tft.drawBitmap(0, 25, tapa_texto, 128, 5, back_color);
    horario();
    
    desenha = false;
    passo = -1;
  }
}
