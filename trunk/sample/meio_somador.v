
// meio somador (soma dois bits)

module meioSomador(a, b, s, c);

input a, b; // bits a somar
output s; // soma
output c; // carry

xor(s, a, b);

and(c, a, b);

endmodule
