// invalid file

// TODO: check if the error is a consensus among different verilog parsers

module badverilog_89(
  output bx,
  output by);

  assign bx = 1;
  assign by = 1;
endmodule

module second_mod();
  wire bx;
  wire by;

  assign bx = 1;
  assign by = 1;
endmodule

module third_mod();
  wire bx;
  wire by;
  wire second_mod;  // errror here, second_mod is a module already defined above

  assign bx = 1;
  assign by = 1;
  assign second_mod = 1;
endmodule
