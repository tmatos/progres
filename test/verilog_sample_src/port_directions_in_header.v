// valid file

module port_directions_in_header(
  input a,
  input b,
  input c_in,
  output s,
  output c_out
);

  wire wx1, wa1, wa2, wa3;
  
  xor(wx1, a, b);
  xor(s, wx1, c_in);
  and(wa1, a, b);
  and(wa2, a, c_in);
  and(wa3, b, c_in);
  or(c_out, wa1, wa2, wa3);

endmodule