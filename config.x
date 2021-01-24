//Definições WiFi
const char* WIFI_SSID     = "nome_wifi";
const char* WIFI_PASSWORD = "senha_wifi";

//Define qual API será usada
const int USAR_API = 2;

//Definições Open Weather Map
const char* OPEN_WEATHER_MAP_APP_ID = "chave_api_open_weather";
//Go to https://www.latlong.net/
//latitude
float OPEN_WEATHER_MAP_LOCATTION_LAT = 0;
//longitude
float OPEN_WEATHER_MAP_LOCATTION_LON = 0;

//Difinições da API clima tempo
const int CLIMA_TEMPO_LOCATE = 0;//código da cidade
const char* CLIMA_TEMPO_TOKEN = "token";

/*
**Consultar as cidades cadastradas
GET: http://apiadvisor.climatempo.com.br/api-manager/user-token/"CLIMA_TEMPO_TOKEN"/locales

**Consultar códigos de cidades
GET: http://apiadvisor.climatempo.com.br/api/v1/locale/city?country=BR&token="CLIMA_TEMPO_TOKEN"

**Registrar uma cidade
PUT: http://apiadvisor.climatempo.com.br/api-manager/user-token/"CLIMA_TEMPO_TOKEN"/locales
Header: Content-Type application/json
Body:{ "localeId": [código da cidade] }

/*