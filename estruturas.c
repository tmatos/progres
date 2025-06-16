/*
 Progres - Simulador de circuitos combinacionais em Verilog
 (C) 2014, 2015 Tiago Matos Santos

 Under the terms of the MIT license.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "progres.h"
#include "sinais.h"
#include "estruturas.h"
#include "strutil.h"
#include "lex.h"
#include "erros.h"
#include "mem.h"

Module* novoCircuito()
{
    Module *circuito = (Module*) xmalloc(sizeof(Module));

    circuito->listaFiosEntrada = novaListaComponente();
    circuito->sinaisEntrada = NULL;

    circuito->listaFiosSaida = novaListaComponente();
    circuito->sinaisSaida = NULL;

    circuito->listaWires = novaListaComponente();

    circuito->listaPortas = novaListaComponente();

    circuito->listaReg.total = 0;
    circuito->listaReg.itens = NULL;

    circuito->listaParam.total = 0;
    circuito->listaParam.itens = NULL;

    circuito->timescale_number = (Tempo) 1;
    circuito->timescale_unit = UN_NS;
    circuito->timescale_precision_number = (Tempo) 1;
    circuito->timescale_precision_unit = UN_NS;

    return circuito;
}

void free_module(Module** mod)
{
    if ( *mod == NULL )
        return;

    if ( (*mod)->listaFiosEntrada->itens )
        free( (*mod)->listaFiosEntrada->itens );
    free( (*mod)->listaFiosEntrada );

    if ( (*mod)->listaFiosSaida->itens )
        free( (*mod)->listaFiosSaida->itens );
    free( (*mod)->listaFiosSaida );

    if ( (*mod)->listaPortas->itens )
        free( (*mod)->listaPortas->itens );
    free( (*mod)->listaPortas );

    if ( (*mod)->listaWires->itens )
        free( (*mod)->listaWires->itens );
    free( (*mod)->listaWires );

    if ( (*mod)->listaParam.itens )
        free( (*mod)->listaParam.itens );

    if ( (*mod)->listaReg.itens )
        free( (*mod)->listaReg.itens );

    if ( (*mod)->sinaisEntrada )
        free( (*mod)->sinaisEntrada );
    
    if ( (*mod)->sinaisSaida )
        free( (*mod)->sinaisSaida );
    
    free( *mod );
}

void adicionaEntrada(Module* circ, Componente comp)
{
    if(!circ || !comp)
        return;

    insereComponente(circ->listaFiosEntrada, comp);
}

void adicionaSaida(Module* circ, Componente comp)
{
    if(!circ || !comp)
        return;

    insereComponente(circ->listaFiosSaida, comp);
}

void adicionaWire(Module* circ, Componente comp)
{
    if(!circ || !comp)
        return;

    insereComponente(circ->listaWires, comp);
}

void adicionaPorta(Module* circ, Componente comp)
{
    if(!circ || !comp)
        return;

    insereComponente(circ->listaPortas, comp);
}

ListaComponente* novaListaComponente()
{
    return novaListaComponenteTamanho(0);
}

ListaComponente* novaListaComponenteTamanho(int size)
{
    int i;
    ListaComponente* list_comp;

    list_comp = (ListaComponente*) xmalloc( sizeof(ListaComponente) );
    list_comp->tamanho = size;

    if (size == 0) {
        list_comp->itens = NULL;
    }
    else {
        list_comp->itens = (Componente*) xmalloc( sizeof(Componente) * size );

        for ( i = 0; i < size; i++ ) {
            list_comp->itens[i] = NULL;
        }
    }

    return list_comp;
}

void insereComponente(ListaComponente* ls, Componente cp)
{
    ls->tamanho++;

    if (ls->tamanho == 1) {
        ls->itens = (Componente*) xmalloc(sizeof(Componente));
    }
    else {
        ls->itens = (Componente*) xrealloc( ls->itens, sizeof(Componente) * ls->tamanho );
    }

    ls->itens[ls->tamanho - 1] = cp;
}

void addRegister(Module* circ, const char* name, unsigned int size, int is_signed)
{
    Register* reg = (Register*) xmalloc(sizeof(Register));

    copy(reg->name, name);
    reg->size = size;
    reg->is_signed = is_signed;
    reg->value = (unsigned int) VAL_0;

    if(circ->listaReg.total == 0) {
        circ->listaReg.total++;
        circ->listaReg.itens = (Register**) xmalloc(sizeof(Register*));
    }
    else {
        circ->listaReg.total++;
        circ->listaReg.itens = (Register**) xrealloc(circ->listaReg.itens,
                                                     sizeof(Register*) * circ->listaReg.total);
    }

    circ->listaReg.itens[circ->listaReg.total - 1] = reg;
}

Register* get_reg_by_name(ListaReg list, const char* name)
{
    int i;

    if(!name)
        return NULL;
    
    for ( i = 0 ; i < list.total ; i++ ) {
        if( iguais(list.itens[i]->name, name) ) {
            return list.itens[i];
        }
    }
    
    return NULL;
}

void addParam(Module* circ, Param* param)
{
    if(circ->listaParam.total == 0) {
        circ->listaParam.total++;
        circ->listaParam.itens = (Param**) xmalloc(sizeof(Param*));
    }
    else {
        circ->listaParam.total++;
        circ->listaParam.itens = (Param**) xrealloc(circ->listaParam.itens,
                                                     sizeof(Param*) * circ->listaParam.total);
    }

    circ->listaParam.itens[circ->listaParam.total - 1] = param;
}

Param* get_param_by_name(ListaParam list, const char* name)
{
    int i;

    if(!name)
        return NULL;
    
    for ( i = 0 ; i < list.total ; i++ ) {
        if( iguais(list.itens[i]->name, name) ) {
            return list.itens[i];
        }
    }
    
    return NULL;
}

int contemComponente(ListaComponente* ls, Componente cp)
{
    int i;

    if(!ls || !cp)
        return 0;

    for( i=0 ; i < ls->tamanho ; i++ ) {
        if( ls->itens[i] == cp ) {
            return 1;
        }
    }

    return 0;
}

Componente novoComponente(const char* nome, t_operador porta)
{
    Componente c = (Componente) xmalloc(sizeof(struct st_componente));

    copy(c->nome, nome);
    c->tipo.operador = porta;
    c->tipo.atraso = 0; // atraso default eh zero

    c->listaEntrada = NULL;
    c->sinalEntrada = NULL;
    c->listaSaida = NULL;
    c->sinalSaida = NULL;

    if (porta != LITERAL_NUMBER) {
        c->listaEntrada = novaListaComponente();
        c->listaSaida = novaListaComponente();
    }

    c->valorDinamico = VAL_X;

    return c;
}

void delete_componente(Componente* c)
{
    if ( !(*c) )
        return;

    if ( (*c)->listaEntrada ) {
        if ( (*c)->listaEntrada->itens ) {
            free( (*c)->listaEntrada->itens );
        }
        free( (*c)->listaEntrada );
    }

    if ( (*c)->listaSaida ) {
        if ( (*c)->listaSaida->itens ) {
            free( (*c)->listaSaida->itens );
        }
        free( (*c)->listaSaida );
    }

    free(*c);
}

Componente getComponenteItemPorNome(ListaComponente* ls, const char* nome)
{
    int i;

    if(!ls || !nome)
        return NULL;

    for( i=0 ; i < ls->tamanho ; i++ ) {
        if( iguais(ls->itens[i]->nome, nome) ) {
           return ls->itens[i];
        }
    }

    return NULL;
}

Componente getPortaPorNome(Module* circ, const char* nome)
{
    if(!circ || !nome)
        return NULL;

    return getComponenteItemPorNome(circ->listaPortas, nome);
}

Componente getWirePorNome(Module* circ, const char* nome)
{
    if(!circ || !nome)
        return NULL;

    return getComponenteItemPorNome(circ->listaWires, nome);
}

Componente getInputPorNome(Module* circ, const char* nome)
{
    if(!circ || !nome)
        return NULL;

    return getComponenteItemPorNome(circ->listaFiosEntrada, nome);
}

Componente getOutputPorNome(Module* circ, const char* nome)
{
    if(!circ || !nome)
        return NULL;

    return getComponenteItemPorNome(circ->listaFiosSaida, nome);
}
