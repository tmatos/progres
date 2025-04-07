
module nandGates(a, b, c, y);

input  a, b, c;  // entradas
output y;        // saida

nand(y, a, b, c);

endmodule
