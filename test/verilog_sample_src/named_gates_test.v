///////////////////////////////////////
//
// valid file:
// logic gates with declared names
//
///////////////////////////////////////

module named_gates_test(a0, b0, x0, y0);

input  a0, b0;  // entradas
output x0, y0;  // saidas

//-------------------------------------

wire wi0, wi1, wi2, wi3;
wire wo0, wo1, wo2, wo3, wo4, wo5;

buf buf_00(wi0, a0);
buf buf_01 (wi1, b0);

not not_00(wi2, a0);
not not_01 (wi3, b0);

and and_00(wo0, wi0, wi1, wi2, wi3);
or  or_00 (wo1, wi0, wi1, wi2, wi3);
nand nand_00(wo2, wi0, wi1, wi2, wi3);

nor nor_00 (wo3, wi0, wi1, wi2, wi3);
xor xor_00 (wo4, wi0, wi1, wi2, wi3);
xnor xnor_00(wo5, wi0, wi1, wi2, wi3);

not _not_FIN(x0, wo0);
buf _buf_FIN(y0, wo0);

endmodule
