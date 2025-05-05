
// invalid file:
// duplicated logic gate name

module badverilog_60(a0, b0, x0, y0);

input  a0, b0;  // entradas
output x0, y0;  // saidas

wire wi0, wi1, wi2, wi3;

buf buffer_0(wi0, a0);
buf buffer_1(wi1, b0);
not invert_0(wi2, a0);
not invert_1(wi3, b0);

and invert_1(x0, wi0, wi1, wi2, wi3);

endmodule
