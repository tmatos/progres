
// logic gates with propagation delays

module delays(in0, in1, out0, out1, outn0, outn1,
              outand0, outor0, outand1, outor1);

input in0, in1;
output out0, out1;
output outn0, outn1;
output outand0, outand1;
output outor0, outor1;

buf #0 (out0, in0);
buf #10 (out1, in1);

not #0 (outn0, in0);
not #10 (outn1, in1);

and #2 (outand0, in0, in1);
and #22 (outand1, in0, in1);

or #2 (outor0, in0, in1);
or #22 (outor1, in0, in1);

endmodule
