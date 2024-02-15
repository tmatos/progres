/////////////////////////////////
//
// comentario na parte de cima
//
/////////////////////////////////

module modTudo(a0, b0, x0, y0);

input  a0, b0;  // entradas
output x0, y0;  // saidas

//------------------------

wire wi0, wi1, wi2, wi3;
wire wo0, wo1, wo2, wo3, wo4, wo5;

buf(wi0, a0);
buf(wi1, b0);
not(wi2, a0);
not(wi3, b0);

and (wo0, wi0, wi1, wi2, wi3);
or  (wo1, wi0, wi1, wi2, wi3);
nand(wo2, wi0, wi1, wi2, wi3);
nor (wo3, wi0, wi1, wi2, wi3);
xor (wo4, wi0, wi1, wi2, wi3);
xnor(wo5, wi0, wi1, wi2, wi3);

not(x0, wo0);
buf(y0, wo0);

endmodule

// comentario no final do arquivo //