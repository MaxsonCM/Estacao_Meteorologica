# Estação Meteorológica com ESP8266

[Baseado na sexta edição da blue box da Filipe Flop](https://www.filipeflop.com/blog/estacao-meteorologica-com-esp8266-nodemcu/)

### [Dependências do projeto](http://img.filipeflop.com/files/download/BLU06/libraries.rar)

### Versão do código para placa
* ESP8266 versão: 2.7.4 (recomendada)

### Blibliotecas usadas
* Adafruit BME280 Library versão: 1.1.0
* Adafruit GFX Library versão: 1.10.2
* Adafruit ST7735 and ST7789 Library versão: 1.6.0
* DHT sensor library versão: 1.4.0
* ESP8266WiFi versão: 1.0.0
* NTPCliente Versão: 3.2.0
* Adafruit Unified Sensor versão: 1.1.4
* Json Streaming Parser versão: 1.0.5
* ~ESP8266 Weather Station versão: 2.0.1~
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
- [x] [Consulta pela API Open Weather](https://openweathermap.org/api/one-call-api), retirado dependencia do projeto externo ~OpenWeatherMapOneCall.h~.
- [ ] Retirar parâmetros hardcoded.

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

## Criando novas imagens para o display LCD TFT no formato hexadecimal

Para dar um exemplo usaremos a seguinte imagem:

![sol-1](https://github.com/MaxsonCM/Estacao_Meteorologica/blob/main/extra/sol-1.png)

Vamos utilizar o seguinte site para converter a imagem em um array hexadecimal [clique aqui para acessar o site](http://javl.github.io/image2cpp/).
Esse método deixará a imagem com as cores preto e branco, onde o branco será a parte que será desenhada no display TFT, o preto será ignorado.
Para que a conversão sai da melhor resolução possível é recomendado criar imagens separadas, para quando houver mais cores na mesma imagem, assim como na imagem exemplo, onde temos o contorno de uma cor e o preenchimento de outra.

Então as imagens ficarão como as de baixo:

![sol-2](https://github.com/MaxsonCM/Estacao_Meteorologica/blob/main/extra/sol-2.png) ![sol-3](https://github.com/MaxsonCM/Estacao_Meteorologica/blob/main/extra/sol-3.png)

Para converter a imagem acesse o site e selecione a imagem sol-2.png, e depois clique em "generate code", e você terá o resultado a baixo.

```
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 
0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x40, 0x01, 0xc0, 0x02, 0x00, 0x00, 0x20, 0x0f, 0xf8, 
0x04, 0x00, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x03, 
0xff, 0xff, 0xc0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 
0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x1f, 0xff, 0xff, 
0xf8, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 
0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x38, 0x7f, 0xff, 0xff, 0xfe, 0x1c, 
0x38, 0x7f, 0xff, 0xff, 0xfe, 0x1c, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 
0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x1f, 
0xff, 0xff, 0xf8, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 
0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x03, 0xff, 0xff, 
0xc0, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x20, 
0x0f, 0xf0, 0x04, 0x00, 0x00, 0x40, 0x01, 0x80, 0x02, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 
0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
```
Agora com esse array é possível substituir o valor da variável correspontende no arquivo icones.c para alterar a imagem padrão do projeto, ficando como o exemplo a baixo:

```
unsigned char sol1[] ={
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 
0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 
0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x40, 0x01, 0xc0, 0x02, 0x00, 0x00, 0x20, 0x0f, 0xf8, 
0x04, 0x00, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x03, 
0xff, 0xff, 0xc0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 
0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x1f, 0xff, 0xff, 
0xf8, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 
0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x38, 0x7f, 0xff, 0xff, 0xfe, 0x1c, 
0x38, 0x7f, 0xff, 0xff, 0xfe, 0x1c, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 
0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x1f, 
0xff, 0xff, 0xf8, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 
0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x03, 0xff, 0xff, 
0xc0, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x20, 
0x0f, 0xf0, 0x04, 0x00, 0x00, 0x40, 0x01, 0x80, 0x02, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 
0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
}
```

Agora só falta fazer o mesmo para a segunda imagem e fazer uma pequena alteração no código principal, comentando ou apagando a linha que desenha o contorno em preto do desenho anterior.

```C
//tft.drawBitmap(posicaoX, posicaoY,sol3,48,48,PRETO);
```
