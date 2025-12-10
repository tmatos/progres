
// invalid file

module badverilog_88e;

reg ax;
reg bx;

initial begin
    ax = 0;
    bx = 1; // unexpected end of file
