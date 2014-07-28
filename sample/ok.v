
// Alguma coisa pra teste...

module nao(entra, sai, complex);

input entra;
output sai;
output complex;

wire fio0, fio1;

not(sai, entra);

not #1(fio0, entra);

not(fio1, entra);

and #2 (complex, fio1, fio0);

endmodule




