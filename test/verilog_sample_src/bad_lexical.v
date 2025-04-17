
// file for lexer test

module nome(a, b, c);

input  a;
input  b;
output c;

// invalid cedil char here
and(ç, a, b); // after 21 valid tokens

endmodule
