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

    circuito->list_input_net = new_list_component();
    circuito->sinais_input = NULL;

    circuito->list_output_net = new_list_component();
    circuito->sinais_output = NULL;

    circuito->list_wire_net = new_list_component();

    circuito->list_logic_gate = new_list_component();

    circuito->list_register.total = 0;
    circuito->list_register.itens = NULL;

    circuito->list_param.total = 0;
    circuito->list_param.itens = NULL;

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

    delete_list_component( &((*mod)->list_input_net) );
    delete_list_component( &((*mod)->list_output_net) );
    delete_list_component( &((*mod)->list_logic_gate) );
    delete_list_component( &((*mod)->list_wire_net) );

    delete_list_param( &((*mod)->list_param) );
    delete_list_register( &((*mod)->list_register) );

    if ( (*mod)->sinais_input )
        free( (*mod)->sinais_input );
    
    if ( (*mod)->sinais_output )
        free( (*mod)->sinais_output );
    
    free( *mod );
    *mod = NULL;
}

void free_circuit(ListModule** circuit)
{
    if ( !circuit || *circuit == NULL )
        return;

    for ( int i = 0 ; i < (*circuit)->total ; i++ )
    {
        free_module( &((*circuit)->itens[i]) );
    }

    if ( (*circuit)->itens )
        free( (*circuit)->itens );

    free( *circuit );
    *circuit = NULL;
}

void add_input(Module* circ, Component* comp)
{
    if(!circ || !comp)
        return;

    insert_component(circ->list_input_net, comp);
}

void add_output(Module* circ, Component* comp)
{
    if(!circ || !comp)
        return;

    insert_component(circ->list_output_net, comp);
}

void add_wire(Module* circ, Component* comp)
{
    if(!circ || !comp)
        return;

    insert_component(circ->list_wire_net, comp);
}

void add_gate(Module* circ, Component* comp)
{
    if(!circ || !comp)
        return;

    insert_component(circ->list_logic_gate, comp);
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

void delete_list_param(ListParam* list_param)
{
    if ( !list_param || !list_param->itens )
        return;

    int i;

    for ( i = 0; i < list_param->total; i++ )
    {
        if ( list_param->itens[i] ) {
            free( list_param->itens[i] );
        }
    }

    free( list_param->itens );
    list_param->itens = NULL;
    list_param->total = 0;
}

void delete_list_register(ListRegister* list_reg)
{
    if ( !list_reg || !list_reg->itens )
        return;

    int i;

    for ( i = 0; i < list_reg->total; i++ )
    {
        if ( list_reg->itens[i] ) {
            free( list_reg->itens[i] );
        }
    }

    free( list_reg->itens );
    list_reg->itens = NULL;
    list_reg->total = 0;
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

    if(circ->list_register.total == 0) {
        circ->list_register.total++;
        circ->list_register.itens = (Register**) xmalloc(sizeof(Register*));
    }
    else {
        circ->list_register.total++;
        circ->list_register.itens = (Register**) xrealloc(circ->list_register.itens,
                                                     sizeof(Register*) * circ->list_register.total);
    }

    circ->list_register.itens[circ->list_register.total - 1] = reg;
}

Register* get_reg_by_name(ListRegister list, const char* name)
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
    if(circ->list_param.total == 0) {
        circ->list_param.total++;
        circ->list_param.itens = (Param**) xmalloc(sizeof(Param*));
    }
    else {
        circ->list_param.total++;
        circ->list_param.itens = (Param**) xrealloc(circ->list_param.itens,
                                                     sizeof(Param*) * circ->list_param.total);
    }

    circ->list_param.itens[circ->list_param.total - 1] = param;
}

Param* get_param_by_name(ListParam list, const char* name)
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

Component* new_component(const char* nome, Role role)
{
    Component* c = (Component*) xmalloc( sizeof(Component) );

    copy(c->nome, nome);
    c->atributos.role = role;
    c->atributos.delay = 0; // atraso default eh zero

    c->list_input = NULL;
    c->input_signal = NULL;
    c->list_output = NULL;
    c->output_signal = NULL;

    if (role != ROLE_LITERAL_NUMBER) {
        c->list_input = new_list_component();
        c->list_output = new_list_component();
    }

    c->dynamic_value = VAL_X;

    return c;
}

void delete_componente(Component** c)
{
    if ( !(*c) )
        return;

    if ( (*c)->list_input ) {
        if ( (*c)->list_input->itens ) {
            free( (*c)->list_input->itens );
        }
        free( (*c)->list_input );
    }

    if ( (*c)->list_output ) {
        if ( (*c)->list_output->itens ) {
            free( (*c)->list_output->itens );
        }
        free( (*c)->list_output );
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

    return get_component_by_name(circ->list_logic_gate, nome);
}

Component* get_wire_by_name(Module* circ, const char* nome)
{
    if(!circ || !nome)
        return NULL;

    return get_component_by_name(circ->list_wire_net, nome);
}

Component* get_input_by_name(Module* circ, const char* nome)
{
    if(!circ || !nome)
        return NULL;

    return get_component_by_name(circ->list_input_net, nome);
}

Component* get_output_by_name(Module* circ, const char* nome)
{
    if(!circ || !nome)
        return NULL;

    return get_component_by_name(circ->list_output_net, nome);
}
