
// invalid file:
// unexpected token

module badverilog_84a(a, x, y);

input a;
output x, y; 

wire w;

assign w = a;
assign x w;   // here
assign y = a;

endmodule
