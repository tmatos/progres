
// invalid file:
// unexpected token

module badverilog_69(a0, b0, x0, y0);

input  a0, b0;  // entradas
output x0, y0;  // saidas

reg rax;

initial
  rax = 0 =  // here

endmodule
