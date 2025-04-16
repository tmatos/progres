
module localparam_test(a, b, c);

input a, b;
output c;

localparam VER_NUM = 123;

and( c, a, b );

localparam _AUTHOR_ID = 001;

endmodule
