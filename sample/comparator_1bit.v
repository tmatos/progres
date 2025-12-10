/*
 1-bit comparator

 compares two single-bit inputs, 'a' and 'b', produces three outputs:
 - a_gt_b:  High (1) if a is greater than b
 - a_lt_b:  High (1) if a is less than b
 - a_eq_b:  High (1) if a is equal to b
*/

module comparator_1bit (a,
                        b,
                        a_gt_b,
                        a_lt_b,
                        a_eq_b);
  input a;
  input b;

  output a_gt_b; // a > b
  output a_lt_b; // a < b
  output a_eq_b; // a == b

  wire not_a;
  wire not_b;

  not U1 (not_a, a);
  not U2 (not_b, b);

  and U3 (a_gt_b, a, not_b);
  and U4 (a_lt_b, not_a, b);

  xnor U5 (a_eq_b, a, b);

endmodule
