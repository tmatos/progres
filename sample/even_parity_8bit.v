
// 8 bit even parity generator

module evenParity(a, b, c, d, e, f, g, h, pb);
  
  input a, b, c, d, e, f, g, h;
  
  output pb;
  
  wire w0, w1, w2, w3, w4, w5;
  
  xor(w0, a, b);
  xor(w1, c, w0);
  xor(w2, d, w1);
  xor(w3, e, w2);
  xor(w4, f, w3);
  xor(w5, g, w4);
  xor(pb, h, w5);
  
endmodule
