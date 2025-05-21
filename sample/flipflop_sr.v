
// flip flop SR

module flipflop_sr(s, r, q, qn);

input s, r;

output q, qn;

wire nand_1_out;
wire nand_2_out;

nor gate_1 (nand_1_out, nand_2_out, r);
nor gate_2 (nand_2_out, nand_1_out, s);

assign q = nand_1_out;
assign qn = nand_2_out;

endmodule
