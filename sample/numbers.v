
module numbers(in, out, outn);

input in;
output out, outn;

and (out, 1, 01, 11, 001, 011, 101, 111, 901, 8051, in);

assign outn = ~out;

endmodule
