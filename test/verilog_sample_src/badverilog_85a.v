
// invalid file:
// undeclared zzz

module badverilog_85a(a, x, y);

input a;
output x, y; 

wire w;

assign w = zzz; // here

endmodule
