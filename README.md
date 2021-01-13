# Estação Meteorológica com ESP8266

[Baseado na blue box 6 da Filipe Flop](https://www.filipeflop.com/blog/estacao-meteorologica-com-esp8266-nodemcu/)
[Dependências do projeto](http://img.filipeflop.com/files/download/BLU06/libraries.rar)

## Itens usados
* Módulo WiFi ESP8266 NodeMCU
* Display LCD TFT 1.8” 128×160
* Sensor de umidade de temperatura DHT11
* Sensor de pressão e temperatura BMP180
* Jumpers Fêmea-Fêmea
* Barra de Pinos 1×40 90º
* Parafusos M2.2 x 6.5mm
* Tubo termo retrátil φ2.0 x 40mm
* Case da estação meteorológica
* botão momentâneo


## Funcionalidades adicionais:

- [x] temporizador para troca de tela
- [x] Clique curto no botão momentâneo para troca de tela
- [x] Clique longo no botão momentâneo para troca da cor do tema
- [ ] Temporizador para API (em breve, retira o delay onde aparece a tela da previsão)
- [ ] Tela da previsão do dia atual (em breve)


## Conexões dos componentes:

|Display LCD TFT 1.8”|ESP8266 NodeMCU|
|:------------------:|:-------------:|
|VCC                 |3.3V           |
|GND                 |GND            |
|CLK                 |D5             |
|SDA                 |D7             |
|RS                  |D8             |
|RST                 |D4             |
|CS                  |D3             |

|Sensor BMP180 |ESP8266 NodeMCU  |
|:------------:|:---------------:|
|VIN           |3.3V             |
|GND           |GND              |
|SCL           |D1               |
|SDA           |D2               |

|Sensor DHT11  |ESP8266 NodeMCU |
|:------------:|:--------------:|
|Pino 1        |3.3V            |
|Pino 2        |D6              |
|Pino 4        |GND             |