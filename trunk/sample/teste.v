// teste simples para as portas

module teste(a, b, out, out2);

input a, b;
output out, out2;

xor #3 (out, a, b);

not #10 (out2, out);

endmodule


