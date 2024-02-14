
// comentario na parte de cima //

module modTudo(a0, b0, x0, y0);

input a0, b0; // entradas
output x0,y0; // saidas

//------------------------

not(y0, b0);

xor(x0, a0, b0);

endmodule

// comentario no final do arquivo //