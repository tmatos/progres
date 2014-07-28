module teste(entra, out);

input entra;
output out;

and #3 (out, entra, entra);

endmodule

