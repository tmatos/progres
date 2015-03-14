
Progres - Copyright (c) 2014, 2015 Tiago Matos Santos

Simulador em constru��o como requisito para aprova��o na disciplina ENGG52
da gradua��o em Engenharia de Computa��o na UFBA.

Ele prov� a simula��o de circuitos combinacionais escritos em Verilog estrutural.
Ou seja, apenas um pequeno subconjuto do Verilog � suportado e principalmente,
n�o h� suporte � simula��o de circuitos sequ�nciais.


----------------------------------------------------------------------------------------
1. Uso
----------------------------------------------------------------------------------------

progres fonte.v [entradas.in] [saidas.out]

Um argumento entre colchetes � opcional.

Faz a an�lise do c�digo fonte em Verilog. Na ocorr�ncia do primeiro erro sint�tico neste
fonte, o programa � interrompido e � informada uma mensagem de erro indicando linha e
coluna onde est� o erro mais uma descri��o do problema.

Exemplo: "5:13 erro: Simbolo esperado nao foi encontrado..."

Nos diz que na linha 5 e coluna 13, um simbolo esperado n�o foi encontrado, o resto da
mensagem pode ser algo espec�fico a cada erro que surja.

Caso o arquivo fonte esteja correto e tenha sido fornecido um arquivo de entrada v�lido,
o programa tentar� simular o circuito para essa entrada e gerar a sa�da correspondente.

Se houve sucesso na simula��o, um arquivo de sa�da ser� criado conforme especificado nos
argumentos do programa ou, se estes foram omitidos, ser� criado um arquivo com o mesmo 
nome do arquivo de entrada por�m com a extens�o mudada para ".out".


----------------------------------------------------------------------------------------
2. Arquivo de entrada e sa�da
----------------------------------------------------------------------------------------

Para a simula��o, � necess�rio que o arquivo de entrada esteja no formato espec�fico de
nosso programa e que haja uma correspond�ncia entre os sinais de entrada do circuito em
Verilog e do arquivo "*.in".

Os arquivos de entrada, "*.in", e de sa�da, "*.out", s�o arquivos de texto que seguem um
formato extremamente simples.

Coment�rios de uma linha s�o v�lidos, tudo que seguir um "//" ser� ignorado.

Cada sinal inicia com um identificador que o denomina. Em seguida e entre chaves, estar�
uma sequ�ncia de pulsos de valor espec�fico ("0", "1" ou "x"), com seus determinados
tempos de dura��o. Sendo cada pulso separado por v�rgula um do outro.

Exemplo:

// Esta entrada cont�m dois sinas.

primeiro
{
 X(2), 0(5), 1(3)
}

segundo { 0(2),1(1),x(3),0(1) }

// Final do arquivo de entrada.


Neste arquivo temos as representa��es dos sinas:

                    ___
   primeiro: XX_____
unid. tempo: 0123456789
               _
    segundo: __ XXX____
unid. tempo: 0123456789


----------------------------------------------------------------------------------------
3. Contato
----------------------------------------------------------------------------------------

Tiago Matos Santos - tiagoms88@gmail.com



2015
