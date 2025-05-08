
// invalid file:
// wrong left side type on initial attrib

module badverilog_64(a0, b0, x0, y0);

input  a0, b0;  // entradas
output x0, y0;  // saidas

initial
  a0 = 1; // wont work with input, will work with reg

endmodule