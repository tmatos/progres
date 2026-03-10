
// flip flop SR

module flipflop_sr_alt(q, qn, s, r, clk);

input s, r, clk;
output q, qn;

wire nand_1_out;
wire nand_2_out;
wire q_internal;
wire qn_internal;

nand(nand_1_out, clk, s);
nand(nand_2_out, clk, r);
nand(q_internal, nand_1_out, qn_internal);
nand(qn_internal, nand_2_out, q_internal);

buf(q, q_internal);
buf(qn, qn_internal);

endmodule
