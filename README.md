# Projeto-metrologia

## Descrição:
Projeto elaborado para o segundo Projeto avaliativo de Metrologia Mecânica. O projeto é um sistema de medição com a utilização de um sensor ultrassônico para determinar a altura de um empilhamento de chapas planas e realizar a contagem das mesmas.

## Componentes:
- Esp32 devki
- Sensor HC-SR04

## Documentação:

O código funciona a partir da utilização de quatro funções, que realizam todos os processos de medição, calibração e coleta de dados.

- distancia():
    Parâmetros: nada
    Retorno: mmMsec(double), conversão de segundos em milimetros
    Descrição: Função que aciona o sensor e retorna a distancia em milimetros

- calibracao():
    Parâmetros: media(double), media de diversas medições feitas
    Retorno: resultado(double)
    Descrição: Função que aplica as devidas correções no valor recebido de acordo com a calibração feita previamente

- distanciaCalibrada():
    Parâmetros: nada
    Retorno: distancia(double)
    Descrição: faz 50 medições de um mesmo ponto e retorna a distancia em milimetros, já aplicada as devidas correções

- definirDados():
    Parâmetros: nada
    Retorno: nada
    Descrição: utilizando um vetor com alocação dinâmica, faz todos os calculos necessários para o funcinamento do sistema e os armazena na struct Dados.

- struct dados:
    Um tipo de variavel criado para armazenar seis outras váriavies, sendo elas:
    - (double) dados.media -> média das medições
    - (double) dados.desvP -> desvio padrão gerado
    - (double) dados.precisao -> precisão do sensor
    - (double) dados.referencia -> referencia do 0 (zero) do sistema
    - (double) dados.espessura -> espessura das chapas a serem medidas  