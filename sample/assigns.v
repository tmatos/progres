
module assigns(a, x, y);

input a;
output x, y; 

wire w;

assign w = a;
assign x = w;

assign y = ~a;

endmodule
