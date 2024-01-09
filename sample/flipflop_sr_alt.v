
// flip flop SR

module flipflopSR(q, qn, s, r, clk);

input s, r, clk;
output q, qn;

wire nand1Out;
wire nand2Out;
wire qInterno;
wire qnInterno;

nand(nand1Out, clk, s);
nand(nand2Out, clk, r);
nand(qInterno, nand1Out, qnInterno);
nand(qnInterno, nand2Out, qInterno);

buf(q, qInterno);
buf(qn, qnInterno);

endmodule
