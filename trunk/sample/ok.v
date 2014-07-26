
// Alguma coisa pra teste...

module ok(a , b,c, x);

input a;// sinal de entrada
input b, c;
output x;

wire tmp, af, gr;

not #2(tmp, a);

and(x, tmp, af);

/*babahathathatha*/not(tmp, a);/*ji*/

/*
not #2(tmp, a);

not #2(tmp, a);

not #2(tmp, a);

not #2(tmp, a);

not #2(tmp, a);

not #2(tmp, a);

and(c, tmp, af);

and(c, tmp, af);
and(c, tmp, af);
and(c, tmp, af);
and(c, tmp, af);
and(c, tmp, af, af, af, af ,af, af, af, af,af);
and(c, tmp, af, af, af, af ,af, af, af, af,af);
and(c, tmp, af, af, af, af ,af, af, af, af,af);
and(c, tmp, af, af, af, af ,af, af, af, af,af);
and(c, tmp, af, af, af, af ,af, af, af, af,af);
and(c, tmp, af, af, af, af ,af, af, af, af,af);*/

endmodule

