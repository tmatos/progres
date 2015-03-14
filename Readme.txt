
Progres - Copyright (c) 2014, 2015 Tiago Matos Santos

Simulador em construção como requisito para aprovação na disciplina ENGG52
da graduação em Engenharia de Computação na UFBA.

Ele provê a simulação de circuitos combinacionais escritos em Verilog estrutural.
Ou seja, apenas um pequeno subconjuto do Verilog é suportado e principalmente,
não há suporte à simulação de circuitos sequênciais.


----------------------------------------------------------------------------------------
1. Uso
----------------------------------------------------------------------------------------

progres fonte.v [entradas.in] [saidas.out]

Um argumento entre colchetes é opcional.

Faz a análise do código fonte em Verilog. Na ocorrência do primeiro erro sintático neste
fonte, o programa é interrompido e é informada uma mensagem de erro indicando linha e
coluna onde está o erro mais uma descrição do problema.

Exemplo: "5:13 erro: Simbolo esperado nao foi encontrado..."

Nos diz que na linha 5 e coluna 13, um simbolo esperado não foi encontrado, o resto da
mensagem pode ser algo específico a cada erro que surja.

Caso o arquivo fonte esteja correto e tenha sido fornecido um arquivo de entrada válido,
o programa tentará simular o circuito para essa entrada e gerar a saída correspondente.

Se houve sucesso na simulação, um arquivo de saída será criado conforme especificado nos
argumentos do programa ou, se estes foram omitidos, será criado um arquivo com o mesmo 
nome do arquivo de entrada porém com a extensão mudada para ".out".


----------------------------------------------------------------------------------------
2. Arquivo de entrada e saída
----------------------------------------------------------------------------------------

Para a simulação, é necessário que o arquivo de entrada esteja no formato específico de
nosso programa e que haja uma correspondência entre os sinais de entrada do circuito em
Verilog e do arquivo "*.in".

Os arquivos de entrada, "*.in", e de saída, "*.out", são arquivos de texto que seguem um
formato extremamente simples.

Comentários de uma linha são válidos, tudo que seguir um "//" será ignorado.

Cada sinal inicia com um identificador que o denomina. Em seguida e entre chaves, estará
uma sequência de pulsos de valor específico ("0", "1" ou "x"), com seus determinados
tempos de duração. Sendo cada pulso separado por vírgula um do outro.

Exemplo:

// Esta entrada contém dois sinas.

primeiro
{
 X(2), 0(5), 1(3)
}

segundo { 0(2),1(1),x(3),0(1) }

// Final do arquivo de entrada.


Neste arquivo temos as representações dos sinas:

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
