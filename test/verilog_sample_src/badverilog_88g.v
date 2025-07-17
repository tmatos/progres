
// invalid file

module badverilog_88g;

reg ax;
reg bx;

initial begin
    #UNKNOWN_TIME; // this is an invalid time delay number
	ax = 0;
    bx = 1;
end

endmodule
