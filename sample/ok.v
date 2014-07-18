
// Alguma coisa pra teste...

module ok(a , b, c);

input a;// sinal de entrada
input b;
output c;

wire tmp;

not #2(tmp, a);

and(c, tmp, b);

endmodule
