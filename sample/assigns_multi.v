
module assigns_multi(a, b, c, d, out_and, out_or);

input wire a, b, c, d;
output wire out_and, out_or;

assign out_and = a & b & c & d;
assign out_or = a | b | c | d;

endmodule
