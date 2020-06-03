# Home Alarme

Sistema desenvolvido em C++ para ser executado em Microcontroladores.

## Instalação

Siga os passos para instala o plugin do [PlatformIO](https://platformio.org/install/ide?install=vscode) no VsCode.

### Não esqueça de colocar as músicas no formato mp3 em um cartão de memória.

## Uso
### Ver Log:
1. Conecta a placa via em uma porta USB para ver o log. Também pode ser usada somente com a alimentação conectando o cabo que acompanha na porta branca e em alguma porta usb ou fonte de 5 volts.

2. Após a instalação da extenção do PlatformIO, abra o projeto no VsCode para as informações da placa.

3. `F1` e digite `PlatformIO: Serial Monitor` 

4. Clica no Botão de RST da placa para que ela reinicie, e o log do que está acontecendo aparecerá no VsCode. 

### Uso direto

1. Conecta o cabo em uma porta usb e na conexão Rj11 da placa esperar a placa iniciar com a indicação da fita de led em verde.
2. Conecta no wifi da placa:
> SSID: ESP8266 AP | SENHA: 10203040
3. Após conectar, o IP de acesso é: [192.168.4.1](http://192.168.4.1/)
4. Abrirá a página Home.

## Observações
1. O sistema foi feito para funcionar automáticamente sem que precise conectá-lo a nenhum wifi. Essa conexão é usada somente para que ele envie a notificação via IFTTT.
2. Somente Ligando ele "na tomada" já está funcionando.
3. Não esqueça deverificar as conexões.
## Considerações finais
O sistema foi testado várias vezes com o devido funcionamento, não apresentando nenhum erro, desde que siga as instruções acima.
