
module assigns(a, x, y);

input a;
output x, y; 

wire w;

assign w = a;
assign x = w; // x gets his only input from this assign
assign y = ~a;

wire w_for_y;

assign w_for_y = y;

endmodule
