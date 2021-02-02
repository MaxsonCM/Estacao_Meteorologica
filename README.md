# Estação Meteorológica com ESP8266

[Baseado na blue box 6 da Filipe Flop](https://www.filipeflop.com/blog/estacao-meteorologica-com-esp8266-nodemcu/)

### [Dependências do projeto](http://img.filipeflop.com/files/download/BLU06/libraries.rar)

* Adafruit BME280 Library versão: 1.1.0
* Adafruit GFX Library versão: 1.10.2
* Adafruit ST7735 and ST7789 Library versão: 1.6.0
* DHT sensor library versão: 1.4.0
* ESP8266WiFi versão: 1.0.0
* ArduinoJson versão: 6.17.2


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
* Resistor de 150 Ohms e 10K Ohms
* botão momentâneo


## Funcionalidades adicionais:

- [x] Temporizador para troca de tela.
- [x] Clique curto no botão momentâneo para troca de tela.
- [x] Clique longo no botão momentâneo para troca da cor do tema.
- [x] Icone de Indicação de intensidade sinal de rede.
- [x] Temporizador para API (retira o delay onde aparece a tela da previsão).
- [x] Tela da previsão do dia atual.
- [x] Novos icones(Wifi, indicador e consulta de API, clima - neve).
- [x] [Consulta pela API climatempo](https://advisor.climatempo.com.br/).

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

### Botão momentâneo
![Conexão do botão](https://github.com/MaxsonCM/Estacao_Meteorologica/blob/main/extra/pin_button.png)

## Criando um token grátis para API clima tempo.

Acesse o site e faça o seu cadastro [aqui](https://advisor.climatempo.com.br/).

No menu vá na opção Token e depois clique em Novo projeto.

![Menu Clima tempo](https://github.com/MaxsonCM/Estacao_Meteorologica/blob/main/extra/Menu%20token.png)

Preencha o nome do projeto e clique em criar.

![Novo projeto](https://github.com/MaxsonCM/Estacao_Meteorologica/blob/main/extra/novo%20projeto.png)

Apos esse processo seu token estará pronto para funcionar em pouco tempo.

![Token gerado](https://github.com/MaxsonCM/Estacao_Meteorologica/blob/main/extra/token%20gerado.png)

### Testando seu token clima tempo (cadastrando uma cidade)

Faça o download de um cliente para teste de API, exemplo [Insomnia](https://insomnia.rest/download/) ou [Postman](https://www.postman.com/downloads/)

Em um programa cliente para API, crie um novo request com o metodo GET e cópie e cole o link abaixo substituindo o seu_token_aqui pelo token gerado pelo no [site cliema tempo](https://advisor.climatempo.com.br/).
```
  http://apiadvisor.climatempo.com.br/api/v1/locale/city?country=BR&token=seu_token_aqui
```
Ao relalizar sua primeira requisição será retornado uma lista de código e cidades, por meio dessa lista você precisará encontrar o código da cidade que deseja fazer a consulta da previsão do tempo.

![Token gerado](https://github.com/MaxsonCM/Estacao_Meteorologica/blob/main/extra/exemplo_cidade.png)

Para cadastrar a cidade crie outra requisição com o metodo PUT, enviando também um body com o código da cidade escolhido.

```
http://apiadvisor.climatempo.com.br/api-manager/user-token/seu_token_aqui/locales
Body:{ "localeId": [3477] }
```

## Criando novas imagens para o display LCD TFT

[Site para conversor de imagem em array hexadecimais](http://javl.github.io/image2cpp/)
* Para facilitar a imagem deve conter apenas as cores preto e branco.
* Apenas a parte brancas na imagem será a area que será preenchida pela biblioteca do display.
