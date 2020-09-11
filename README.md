# KGBcomM

>UNICAMP - Universidade Estadual de Campinas.<br>
>FT - Faculdade de Tecnologia.<br>
>Limeira - SP.
><br><br>
>Maio 2019 - 1S2019


## Introdução
O projeto tem como objetivo a criação de um programa, a partir da linguagem C, que utilize múltiplas _threads_ para dividir uma matriz NxN, em que N representa as dimensões da matriz, respectivamente, linhas e colunas, em outras duas matrizes também NxN de tal forma que a primeira seja composta por elementos a partir da diagonal principal e acima; e a segunda matriz com elementos abaixo da diagonal principal.

O programa foi escrito para o sistema operacional Linux e utilizou a biblioteca **_POSIX threads_**.

## Material e Método

### Instrução para a compilação do código
   Para compilar o código, foi utilizado o sistema Linux e o comando: 
   > **_`gcc -pthread divideMat.c -o divideMat`_**

### Explicação do código em alto nível
 Ao executar o programa, é passado por linha de comando, a dimensão da matriz o número de _threads_ com qual o programa será executado e o arquivo de entrada, em que está a matriz que será processada. O número de _threads_ pode ser 2, 4, 8 ou 16 e a matriz em questão é quadrática.
 
 Se a quantidade de _threads_ for respeitada, há a alocação das três matrizes, isto é, a matriz principal, a matriz diagonal 1 e a matriz diagonal 2. Caso contrário, é informado ao usuário que o número de _threads_ é inválido. Após as alocações das matrizes, abre-se o arquivo de entrada que possui os dados da matriz principal, ou seja, a matriz que será processada. Se não houver erro na abertura, inicia-se leitura dos dados do arquivo e, ao terminar, o arquivo é fechado. Vale ressaltar, que logo após o término da leitura começa a marcação do tempo inicial, como se fosse o início do cronômetro. A partir disso, o próximo passo é a criação das _threads_, com o comando:

 > **_`pthread_create(&vThread[i], NULL, threadDividirMatriz, (void *)&info[i]);`_**

 Esse comando está dentro de um FOR, o qual tem como limite o número de _threads_. O comando é responsável pela criação de uma thread, bem como, pela chamada da função que tem como responsabilidade realizar a divisão da matriz principal em matriz diagonal 1 e matriz diagonal 2. Também há um FOR responsável para que todas as _threads_ retornem ao mesmo tempo, por meio do comando:

  > **_`pthread_join(vThread[i], NULL);`_**

 Após todas as _threads_ terminarem seus respectivos processamentos, há a marcação do tempo final. Então ao realizar a subtração do tempo final pelo tempo inicial tem-se o tempo total de execução.
 
 Por fim, abre-se o arquivo de destino e se não houver erro de abertura,grava-se a matriz diagonal 2 no arquivo e o fecha. O mesmo procedimento ocorre para matriz 1 diagonal 1.

## Conclusão
 No início do projeto, era esperado que o tempo médio de execução fosse inversamente proporcional ao número de _threads_, ou seja, quanto maior fosse o número de _threads_, menor seria o tempo de execução. Esse resultado era esperado, pois, ao alocar dezesseis _threads_ trabalhando simultaneamente, espera-se que desempenho seja maior do que duas _threads_ trabalhando simultaneamente na mesma tarefa.

 Entretanto, os resultados alcançados mostraram que para o programa desenvolvido, o ideal é trabalhar com quatro _threads_, isso porque é um programa de baixa complexidade. Logo, a criação de cada uma das _threads_ demanda muito tempo e isso faz com que o desempenho fique menor.

 Contudo, para programas com uma alta complexidade, possivelmente o tempo médio de execução será inversamente proporcional ao número de _threads_.


## Informações extras 
* [Vídeo - Google Drive](https://drive.google.com/file/d/1cuCApIXgdM9TFokg3PJjr4exgj-A61-Z/view)
* [Planilha - Tempos de execução:](https://docs.google.com/spreadsheets/d/1U43o1vI2ausUwkoF3c3XVV9LwzetEEIeB6wiBzNzLHA/edit#gid=2025868260)

