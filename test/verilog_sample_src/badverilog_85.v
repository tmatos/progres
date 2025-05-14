
// invalid file:
// undeclared zzz

module badverilog_85(a, x, y);

input a;
output x, y; 

wire w;

assign zzz = a; // here

endmodule
