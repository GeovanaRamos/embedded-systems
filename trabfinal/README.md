# Trabalho FINAL

Na pasta *central* está o servidor central da Raspberry Pi e na *client* o código da ESP32. O servidor *central* deve ser executado primeiro e assim que a conexão for estabelecida ele vai mudar os dados do menu.

## Requisitos

- Clonar o respositório DHT11 na pasta esp-idf. Foi utilizado um [fork](https://github.com/icaropires/esp32-DHT11) pois o repositório oficial possui um erro.


## Como Rodar

Executar primeiro a *central* depois o *client*.

### Central
Dentro da pasta *central*

Para compilar:
```
make 
```
Para rodar:
```
make run
```

Para limpar arquivos *.o*:
```
make clean
```

### Client

Rodar o comando `export.sh` da esp-idf dentro da pasta *client*. 

Rodar o comando `idf.py menuconfig` e adicionar as credenciais de WI-FI.

Depois:
```
idf.py build
idf.py -p {PORTA} flash monitor
```
- Caso o menu apresente um novo cliente, digite 100 para configurar.
- Para ligar/desligar o alarme digite 200
- Para ligar/desligar um dispositivo de saída, digite o código que aparece na lista Cômodo-Saída.
- Para sair digite 0.

