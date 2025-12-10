
// invalid file

module badverilog_88c;

reg ax;
reg bx;

initial
	ax = 0;
    bx = 1;
end // invalid initial block because it has no corresponding begin keyword

endmodule
