/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

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

Module* new_module()
{
    Module *circuito = (Module*) xmalloc(sizeof(Module));

    circuito->listaFiosEntrada = new_list_component();
    circuito->sinaisEntrada = NULL;

    circuito->listaFiosSaida = new_list_component();
    circuito->sinaisSaida = NULL;

    circuito->listaWires = new_list_component();

    circuito->listaPortas = new_list_component();

    circuito->listaReg.total = 0;
    circuito->listaReg.itens = NULL;

    circuito->listaParam.total = 0;
    circuito->listaParam.itens = NULL;

    circuito->timescale_number = (Tempo) 1;
    circuito->timescale_unit = UN_NS;
    circuito->timescale_precision_number = (Tempo) 1;
    circuito->timescale_precision_unit = UN_NS;

    copy(circuito->name, "");

    return circuito;
}

void free_module(Module** mod)
{
    if ( *mod == NULL )
        return;

    delete_list_component( &((*mod)->listaFiosEntrada) );
    delete_list_component( &((*mod)->listaFiosSaida) );
    delete_list_component( &((*mod)->listaPortas) );
    delete_list_component( &((*mod)->listaWires) );

    if ( (*mod)->listaParam.itens )
        free( (*mod)->listaParam.itens );

    if ( (*mod)->listaReg.itens )
        free( (*mod)->listaReg.itens );

    if ( (*mod)->sinaisEntrada )
        free( (*mod)->sinaisEntrada );
    
    if ( (*mod)->sinaisSaida )
        free( (*mod)->sinaisSaida );
    
    free( *mod );
    *mod = NULL;
}

void add_input(Module* circ, Component* comp)
{
    if(!circ || !comp)
        return;

    insert_component(circ->listaFiosEntrada, comp);
}

void add_output(Module* circ, Component* comp)
{
    if(!circ || !comp)
        return;

    insert_component(circ->listaFiosSaida, comp);
}

void add_wire(Module* circ, Component* comp)
{
    if(!circ || !comp)
        return;

    insert_component(circ->listaWires, comp);
}

void add_gate(Module* circ, Component* comp)
{
    if(!circ || !comp)
        return;

    insert_component(circ->listaPortas, comp);
}

ListComponent* new_list_component()
{
    return new_list_component_of_size(0);
}

ListComponent* new_list_component_of_size(unsigned int size)
{
    unsigned int i;
    ListComponent* list_comp;

    list_comp = (ListComponent*) xmalloc( sizeof(ListComponent) );
    list_comp->tamanho = size;

    if (size == 0) {
        list_comp->itens = NULL;
    }
    else {
        list_comp->itens = (Component**) xmalloc( sizeof(Component*) * size );

        for ( i = 0; i < size; i++ ) {
            list_comp->itens[i] = NULL;
        }
    }

    return list_comp;
}

void delete_list_component(ListComponent** ppl)
{
    if ( *ppl == NULL )
        return;
    
    if ( (*ppl)->itens ) {
        int i;

        for ( i = 0; i < (*ppl)->tamanho; i++ )
        {
            if ( (*ppl)->itens[i] ) {
                delete_componente( &((*ppl)->itens[i]) );
            }
        }

        free( (*ppl)->itens );
    }

    free(*ppl);
    *ppl = NULL;
}

void insert_component(ListComponent* ls, Component* cp)
{
    ls->tamanho++;

    if (ls->tamanho == 1) {
        ls->itens = (Component**) xmalloc(sizeof(Component*));
    }
    else {
        ls->itens = (Component**) xrealloc( ls->itens, sizeof(Component*) * ls->tamanho );
    }

    ls->itens[ls->tamanho - 1] = cp;
}

void add_register(Module* circ, const char* name, unsigned int size, int is_signed)
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

void add_param(Module* circ, Param* param)
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

int has_component_by_pointer(ListComponent* ls, Component* cp)
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

Component* new_component(const char* nome, t_operador porta)
{
    Component* c = (Component*) xmalloc( sizeof(Component) );

    copy(c->nome, nome);
    c->tipo.operador = porta;
    c->tipo.atraso = 0; // atraso default eh zero

    c->listaEntrada = NULL;
    c->sinalEntrada = NULL;
    c->listaSaida = NULL;
    c->sinalSaida = NULL;

    if (porta != ROLE_LITERAL_NUMBER) {
        c->listaEntrada = new_list_component();
        c->listaSaida = new_list_component();
    }

    c->valorDinamico = VAL_X;

    return c;
}

void delete_componente(Component** c)
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
    *c = NULL;
}

Component* get_component_by_name(ListComponent* ls, const char* nome)
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

Component* get_gate_by_name(Module* circ, const char* nome)
{
    if(!circ || !nome)
        return NULL;

    return get_component_by_name(circ->listaPortas, nome);
}

Component* get_wire_by_name(Module* circ, const char* nome)
{
    if(!circ || !nome)
        return NULL;

    return get_component_by_name(circ->listaWires, nome);
}

Component* get_input_by_name(Module* circ, const char* nome)
{
    if(!circ || !nome)
        return NULL;

    return get_component_by_name(circ->listaFiosEntrada, nome);
}

Component* get_output_by_name(Module* circ, const char* nome)
{
    if(!circ || !nome)
        return NULL;

    return get_component_by_name(circ->listaFiosSaida, nome);
}
