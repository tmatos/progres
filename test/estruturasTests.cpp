#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cstring>

#include "strutil.h"
#include "lex.h"
#include "sinais.h"
#include "estruturas.h"

class Testes_estruturas : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( Testes_estruturas );
  CPPUNIT_TEST( test_new_component );
  CPPUNIT_TEST( test_new_list_component );
  CPPUNIT_TEST( test_new_list_component_of_size );
  CPPUNIT_TEST( test_new_module );
  CPPUNIT_TEST( test_insert_component );
  CPPUNIT_TEST( test_getXPorNome );
  CPPUNIT_TEST( test_get_param_by_name );
  CPPUNIT_TEST( test_get_reg_by_name );
  CPPUNIT_TEST_SUITE_END();

public:

  void test_new_component()
  {
    char nome[] = "entrada_0"; 
    Component* c0 = new_component( (char*)nome, ROLE_INPUT );
    CPPUNIT_ASSERT(c0);
    CPPUNIT_ASSERT( !strcmp( (char*)(c0->nome), (char*)nome ) );
    CPPUNIT_ASSERT_EQUAL( c0->atributos.role, ROLE_INPUT );
    CPPUNIT_ASSERT_EQUAL( c0->dynamic_value, VAL_X );
    CPPUNIT_ASSERT( c0->list_input );
    CPPUNIT_ASSERT( c0->list_output );
  }

  void test_new_list_component()
  {
    ListComponent* list = new_list_component();
    CPPUNIT_ASSERT(list);
    CPPUNIT_ASSERT_EQUAL(list->tamanho, 0);
    CPPUNIT_ASSERT(!list->itens);
  }

  void test_new_list_component_of_size()
  {
    ListComponent* list = new_list_component_of_size(50);
    CPPUNIT_ASSERT(list);
    CPPUNIT_ASSERT_EQUAL(list->tamanho, 50);
    CPPUNIT_ASSERT(list->itens);
  }

  void test_new_module()
  {
    Module* circ = new_module();
    CPPUNIT_ASSERT(circ);

    CPPUNIT_ASSERT(circ->list_input_net);
    CPPUNIT_ASSERT_EQUAL(circ->list_input_net->tamanho, 0);
    CPPUNIT_ASSERT(!circ->list_input_net->itens);

    CPPUNIT_ASSERT(circ->list_output_net);
    CPPUNIT_ASSERT_EQUAL(circ->list_output_net->tamanho, 0);
    CPPUNIT_ASSERT(!circ->list_output_net->itens);

    CPPUNIT_ASSERT(circ->list_wire_net);
    CPPUNIT_ASSERT_EQUAL(circ->list_wire_net->tamanho, 0);
    CPPUNIT_ASSERT(!circ->list_wire_net->itens);

    CPPUNIT_ASSERT(circ->list_logic_gate);
    CPPUNIT_ASSERT_EQUAL(circ->list_logic_gate->tamanho, 0);
    CPPUNIT_ASSERT(!circ->list_logic_gate->itens);
  }

  void test_insert_component()
  {
    char str_c0[] = "entrada_0";
    char str_c1[] = "entrada_1"; 
    Component* c0 = new_component( (char*)str_c0, ROLE_INPUT );
    Component* c1 = new_component( (char*)str_c1, ROLE_INPUT );

    ListComponent* list = new_list_component();

    CPPUNIT_ASSERT(c0);
    CPPUNIT_ASSERT(c1);
    CPPUNIT_ASSERT(list);
    CPPUNIT_ASSERT_EQUAL(list->tamanho, 0);

    insert_component(list, c0);
    CPPUNIT_ASSERT_EQUAL(list->tamanho, 1);
    CPPUNIT_ASSERT(list->itens);
    CPPUNIT_ASSERT_EQUAL(list->itens[0], c0);

    insert_component(list, c1);
    CPPUNIT_ASSERT_EQUAL(list->tamanho, 2);
    CPPUNIT_ASSERT(list->itens);
    CPPUNIT_ASSERT_EQUAL(list->itens[1], c1);
  }

  void test_getXPorNome()
  {
    char* null_name = NULL;
    Module* circ = new_module();
    CPPUNIT_ASSERT(circ);    

    CPPUNIT_ASSERT( !get_gate_by_name(circ, null_name) );
    CPPUNIT_ASSERT( !get_wire_by_name(circ, null_name) );
    CPPUNIT_ASSERT( !get_input_by_name(circ, null_name) );
    CPPUNIT_ASSERT( !get_output_by_name(circ, null_name) );

    char str_porta[] = "porta_01";
    char str_wire[] = "fio_01"; 
    char str_in[] = "entrada_01";
    char str_out[] = "saida_01"; 
    Component* c_porta = new_component( (char*)str_porta, ROLE_AND );
    Component* c_wire = new_component( (char*)str_wire, ROLE_WIRE );
    Component* c_in = new_component( (char*)str_in, ROLE_INPUT );
    Component* c_out = new_component( (char*)str_out, ROLE_OUTPUT );

    insert_component(circ->list_logic_gate, c_porta);
    insert_component(circ->list_wire_net, c_wire);
    insert_component(circ->list_input_net, c_in);
    insert_component(circ->list_output_net, c_out);

    CPPUNIT_ASSERT( get_gate_by_name(circ, str_porta) );
    CPPUNIT_ASSERT( get_wire_by_name(circ, str_wire) );
    CPPUNIT_ASSERT( get_input_by_name(circ, str_in) );
    CPPUNIT_ASSERT( get_output_by_name(circ, str_out) );

    CPPUNIT_ASSERT( !get_gate_by_name(circ, str_in) );
    CPPUNIT_ASSERT( !get_wire_by_name(circ, str_in) );
    CPPUNIT_ASSERT( !get_input_by_name(circ, str_out) );
    CPPUNIT_ASSERT( !get_output_by_name(circ, str_in) );
  }

  void test_get_param_by_name()
  {
    char str_param_name[] = "ALGUM_NOME_001";
    int N = 255;

    Module* circ = new_module();
    CPPUNIT_ASSERT(circ);

    CPPUNIT_ASSERT( ! get_param_by_name(circ->list_param, str_param_name) );

    Param p;
    p.is_local = 0;
    p.value = N;
    copy(p.name, str_param_name);

    add_param(circ, &p);

    CPPUNIT_ASSERT( ! get_param_by_name(circ->list_param, NULL) );
    CPPUNIT_ASSERT( ! get_param_by_name(circ->list_param, "") );
    CPPUNIT_ASSERT( ! get_param_by_name(circ->list_param, "algum") );
    CPPUNIT_ASSERT( ! get_param_by_name(circ->list_param, "ALGUM") );

    Param* r = get_param_by_name(circ->list_param, str_param_name);

    CPPUNIT_ASSERT( r );
    CPPUNIT_ASSERT( !strcmp(str_param_name, r->name) );
    CPPUNIT_ASSERT_EQUAL( N, r->value );
  }

  void test_get_reg_by_name()
  {
    char str_reg_name[] = "register_ALGUM_NOME_001";

    Module* circ = new_module();
    CPPUNIT_ASSERT(circ);

    CPPUNIT_ASSERT( ! get_reg_by_name(circ->list_register, str_reg_name) );

    add_register(circ, str_reg_name, 32, 0);

    CPPUNIT_ASSERT( ! get_reg_by_name(circ->list_register, NULL) );
    CPPUNIT_ASSERT( ! get_reg_by_name(circ->list_register, "") );
    CPPUNIT_ASSERT( ! get_reg_by_name(circ->list_register, "algum") );
    CPPUNIT_ASSERT( ! get_reg_by_name(circ->list_register, "ALGUM") );

    Register* r = get_reg_by_name(circ->list_register, str_reg_name);

    CPPUNIT_ASSERT( r );
    CPPUNIT_ASSERT( !strcmp(str_reg_name, r->name) );
  }

};

