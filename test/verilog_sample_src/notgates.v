
module notGates(a, b, na, nnb);

input  a, b;     // entradas
output na, nnb;  // saidas

wire wnb;

not(na, a);

not(wnb, b);

not(nnb, wnb);

endmodule
