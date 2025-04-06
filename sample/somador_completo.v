
// somador completo

module somador_completo(a, b, c_in, s, c_out);

input a, b; // bits a somar
input c_in; // carry in

output s; // soma
output c_out; // carry out

wire wx1, wa1, wa2, wa3;

xor(wx1, a, b);
xor(s, wx1, c_in);

and(wa1, a, b);
and(wa2, a, c_in);
and(wa3, b, c_in);

or(c_out, wa1, wa2, wa3);

endmodule
