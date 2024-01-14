# Progres

[![Build Status](https://github.com/tmatos/progres/actions/workflows/makefile.yml/badge.svg)](https://github.com/tmatos/progres/actions)
[![codecov](https://codecov.io/gh/tmatos/progres/graph/badge.svg?token=XCUNGD1HQD)](https://codecov.io/gh/tmatos/progres)

Simulador orientado a eventos para um subset da linguagem Verilog.

No momento, provê a simulação de circuitos combinacionais escritos em Verilog estrutural.


## 1. Uso

Na interface de linha de comando:

```
progres fonte.v [entradas.in] [saidas.out]
```

Argumentos entre colchetes são opcionais.

O comando acima faz uma análise do arquivo com código-fonte Verilog fornecido.
Na ocorrência do primeiro erro sintático, o programa é interrompido e uma mensagem
será informada, indicando linha e coluna da ocorrência com uma possível descrição do problema.

Exemplo de saída com erro:

`5:13 erro: Simbolo esperado nao foi encontrado...`

A mensagem acima nos diz que na linha 5 e coluna 13 do arquivo `.v` fornecido,
um simbolo esperado não foi encontrado.
O restante do texto poderá conter algo mais específico a cada erro que venha a surgir.

Caso o arquivo fonte esteja correto e um arquivo de entrada válido tenha sido fornecido,
o programa tentará simular o circuito com esta entrada e gerar a saída correspondente.

Se houver sucesso na simulação, um arquivo de saída será criado conforme especificado nos
argumentos do programa ou, se estes foram omitidos, será criado um arquivo com o mesmo 
nome do arquivo de entrada porém com a extensão mudada para `.out`.


## 2. Arquivos de entrada e saída

Para a simulação, é necessário que o arquivo de entrada esteja no formato específico de
nosso programa e que haja uma correspondência entre os sinais de entrada do circuito em
Verilog e do arquivo `.in`.

Os arquivos de entrada, `.in`, e de saída, `.out`, são arquivos de texto que seguem um
formato extremamente simples:

* Comentários de uma linha são válidos, tudo que seguir um `//` será ignorado.

* Cada sinal inicia com um identificador que o denomina. Em seguida e entre chaves, estará
uma sequência de pulsos de valor específico ("0", "1" ou "x"), com seus determinados
tempos de duração. Sendo cada pulso separado por vírgula um do outro.

Exemplo:

``` 
// Arquivo de entrada contendo dois sinas

primeiro
{
 X(2), 0(5), 1(3)
}

segundo { 0(2),1(1),x(3),0(1) }

// Final do arquivo de entrada
```

Neste arquivo, temos as representações dos sinas abaixo:

```
                    ___
   primeiro: XX_____
unid. tempo: 0123456789
               _
    segundo: __ XXX____
unid. tempo: 0123456789
```


## 3. Compilação e instalação

O código fonte é escrito principalmente em C e deverá compilar sob qualquer ambiente
com suporte ao padrão C89. Um makefile é provido, portanto basta apenar utilizar:

```
make
```

Para gerar o executável, que poderá ser copiado para qualquer local desejado.


## 4. Contato

* Email: tiago AT tmatos.net
* Site: https://tmatos.net


(C) 2014, 2015 Tiago Matos Santos
