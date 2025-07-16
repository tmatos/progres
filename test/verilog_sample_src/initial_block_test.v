
// valid file

module initial_block_test;

reg ax;
reg bx;

localparam P = 1;

initial begin
    $display("Initial block started");
    ax = P;
	bx = 0;
    $display("Initial block ended");
end

endmodule
