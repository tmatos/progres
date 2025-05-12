
module initial_single_sample(x, y);

input x;
output y;

reg is_on;

reg [15:0] ax;
reg [15:0] bx;

localparam MAX = 65535;

initial
	is_on = 1;

initial
	ax = 12345;

initial
	bx = MAX;

endmodule
