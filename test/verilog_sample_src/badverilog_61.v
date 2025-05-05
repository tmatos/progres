
// invalid file:
// incomplete code, unexpected eof

module badverilog_61(a0, b0, x0, y0);

input  a0, b0;  // entradas
output x0, y0;  // saidas

wire wi0, wi1, wi2, wi3;

buf buffer_0(wi0, a0);
buf buffer_1(wi1, b0);
not invert_0 
