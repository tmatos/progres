
// invalid file

module badverilog_88;

reg ax;
reg bx;

initial begin // invalid initial block because it has no corresponding end keyword
	ax = 0;
    bx = 1;

endmodule
