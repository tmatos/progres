
// Binary multiplier (2 bit in, 4 bit out)
// a * b = c

module BinaryMultiply(a0, a1,
                      b0, b1,
                      c0, c1, c2, c3);
  
  input a0, a1;
  input b0, b1;
  
  output c0, c1, c2, c3;
  
  wire wand0, wand1, wand2, wand3;
  wire wand4, wand5;
  
  and(wand0, a0, b1);
  and(c0, a0, b0);
  and(wand2, a1, b0);
  and(wand3, a1, b1);
  
  xor(c1, wand0, wand2);
  
  and(wand4, wand0, wand2);
  
  xor(c2, wand4, wand3);
  
  and(c3 , wand4, wand3);
  
endmodule
