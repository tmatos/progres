`resetall

`timescale 1 ns / 1 ps

`define VER 10

module preproc_directives;

    localparam DEF = `VER;
    
endmodule


// slower time

`resetall
`timescale 100ns/1ns

module preproc_directives_slow;

    localparam DEF = `VER;
    
endmodule
