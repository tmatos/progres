
// valid file

module tri_state_gates(
    c0, c1, c2, c3,
    d0, d1, d2, d3,
    o0, o1, o2, o3
);

input c0;
input c1;
input c2;
input c3;

input d0;
input d1;
input d2;
input d3;

output o0;
output o1;
output o2;
output o3;

bufif0(o0, c0, d0);
bufif1(o1, c1, d1);

notif0(o2, c2, d2);
notif1(o3, c3, d3);

endmodule
