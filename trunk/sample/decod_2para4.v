
// decodificador de 2 para 4

module doisParaQuatro(a, b, q0, q1, q2, q3);

input a, b; // um numero rep. por dois bits

output q0, q1, q2, q3; // saida

wire aNeg, bNeg;

not(aNeg, a);
not(bNeg, b);

and(q0, aNeg, bNeg);
and(q1, b, aNeg);
and(q2, a, bNeg);
and(q3, a, b);

endmodule
