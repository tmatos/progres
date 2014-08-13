
// somador completo

module somadorCompleto(a, b, cIn, s, cOut);

input a, b; // bits a somar
input cIn; // carry in

output s; // soma
output cOut; // carry out

wire wx1, wa1, wa2, wa3;

xor(wx1, a, b);
xor(s, wx1, cIn);

and(wa1, a, b);
and(wa2, a, cIn);
and(wa3, b, cIn);

or(cOut, wa1, wa2, wa3);

endmodule
