
// decodificador de 2 para 4

module decod_2para4(a, b, q0, q1, q2, q3);

input a, b; // um numero representado por dois bits

output q0, q1, q2, q3; // saida

wire a_neg, b_neg;

not(a_neg, a);
not(b_neg, b);

and(q0, a_neg, b_neg);
and(q1, b, a_neg);
and(q2, a, b_neg);
and(q3, a, b);

endmodule
