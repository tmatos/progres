
// flip flop SR

module flipflopSR(q, qn, s, r, clk);

input s, r, clk;

output q, qn;

wire nand1Out;
wire nand2Out;

nand(nand1Out, clk, s);
nand(nand2Out, clk, r);
nand(q, nand1Out, qn);
nand(qn, nand2Out, q);

endmodule
