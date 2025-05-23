
`define RELEASE 1

`define VER 5


module preproc_undef;

`undef RELEASE

    localparam DEF = `VER;
    
endmodule
