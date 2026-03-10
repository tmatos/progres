
// 8 bit even parity generator

module even_parity_8bit(a, b, c, d, e, f, g, h, pb);
  
  input a, b, c, d, e, f, g, h;
  
  output pb;
  
  wire w0, w1, w2, w3, w4, w5;
  
  xor U0 (w0, a, b);
  xor U1 (w1, c, w0);
  xor U2 (w2, d, w1);
  xor U3 (w3, e, w2);
  xor U4 (w4, f, w3);
  xor U5 (w5, g, w4);
  xor U6 (pb, h, w5);
  
endmodule
