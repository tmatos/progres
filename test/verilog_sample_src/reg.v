
// valid file

module main;

localparam N_FLAGS = 20;

reg bit_is_on;
reg bit_is_ok;

reg [10:10] silly_bit;

reg [15:0] ra;
reg [15:0] rb;

reg [7:0] ral;
reg [15:8] rah;

reg [31:0] rax;
reg [31:0] rbx;

reg [N_FLAGS:1] r_flag;

localparam EXTRA_FLAGS_START = 21;
localparam EXTRA_FLAGS_END = 23;

reg [EXTRA_FLAGS_END:EXTRA_FLAGS_START] r_extra_flag;

endmodule
