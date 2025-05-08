
module initial_single_sample(x, y);

input x;
output y;

reg ax;
reg bx;

localparam P = 0;

initial
	ax = P;

initial
	bx = 1;

endmodule
