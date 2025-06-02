
// invalid file

`resetall
`timescale 1 ns / 1 ns

module bad_timescale()

`timescale 1 ns , 1 ns // error here

endmodule
