
// flip flop SR with enable input

module flipflop_sr_en(s, r, en, q, qn);

input s, r, en;

output q, qn;

wire gate_1_out;
wire gate_2_out;

nand gate_1 (gate_1_out, en, s);
nand gate_2 (gate_2_out, en, r);

nand gate_3 ( q, gate_1_out, qn);
nand gate_4 (qn, gate_2_out, q);

endmodule
