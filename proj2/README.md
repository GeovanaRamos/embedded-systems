# Projeto 2

Esta pasta contém os arquivos referentes ao projeto 2. Na pasta *central* está o servidor central e na *distributed* o servidor distribuido. Os comandos Makefile apresentados na seção *Como Rodar* valem para ambos. O servidor distribuído deve ser executado primeiro e assim que a conexão for estabelecida ele vai logar as ações de envio e recebimento.

## Como Rodar

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


## Protocolo de Comunicação

As opções selecionadas pelo usuário no menu se transformam em um código que será enviado para o servidor e descodificado para que ele realize a ação. O código segue o seguinte formato:

```
<MODO><TIPO_DISPOSITIVO><NÚMERO_DISPOSITIVO>
```

Abaixo tem-se se os possíveis valores para cada parâmetro do código.

|   PARÂMETRO    |   Valores | 
|-----------|------------|
| MODO   |   (L): Ligar dispositivo<br>(D): Desligar dispositivo  |   
|TIPO_DISPOSITIVO |   (1): Lâmpada<br>(2): Ar condicionado   |   
|NÚMERO_DISPOSITIVO |   (1-4): Para lâmpadas<br>(1-2): Para ar condicionado  |   

Por exemplo, se o usuário selecionasse a opção de ligar o ar condicionado 1 o código enviado para o servidor distribuído seria:
```
L21
```
