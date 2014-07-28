
// tentativa de flip flop rs

module flipflop(r, s, ck, q, qn, prbSn, prbRn, prb3);

input r, s, ck;
output q, qn;

output prbSn, prbRn, prb3;

wire sn, rn;
wire qW, qnW;

// aqui o núcleo//////
nand #1(sn, s, ck);
nand #1(rn, r, ck);

nand #1(qW, sn, qnW);
nand #2(qnW, rn, qW);
//////////////////////

buf (prbSn, sn);
buf (prbRn, rn);

buf(q, qW);
buf(qn, qnW);

endmodule
