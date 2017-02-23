/* Verilog model created from schematic RemoteControllerSchematic.sch -- Apr 17, 2014 18:56 */

module RemoteControllerSchematic( A, B, C, Ld, Le, Rd, Re );
 input A;
 input B;
 input C;
output Ld;
output Le;
output Rd;
output Re;
wire N_28;

wire N_27;

wire N_26;

wire N_20;

wire N_21;

wire N_18;

wire N_19;

wire N_15;

wire N_16;

wire N_11;

wire N_12;

wire N_13;

wire N_9;

wire N_10;

wire N_4;

wire N_5;




IB I23 ( .I(C), .O(N_26) );
IB I24 ( .I(B), .O(N_28) );
IB I25 ( .I(A), .O(N_27) );
INV I20 ( .A(N_27), .Z(Rd) );
INV I14 ( .A(N_26), .Z(N_18) );
INV I15 ( .A(N_28), .Z(N_19) );
INV I10 ( .A(N_26), .Z(N_11) );
INV I11 ( .A(N_26), .Z(N_12) );
INV I12 ( .A(N_28), .Z(N_13) );
INV I6 ( .A(N_28), .Z(N_9) );
INV I7 ( .A(N_27), .Z(N_10) );
OR2 I3 ( .A(N_5), .B(N_4), .Z(Le) );
OR2 I18 ( .A(N_20), .B(N_21), .Z(Re) );
OR2 I13 ( .A(N_16), .B(N_15), .Z(Ld) );
AND3 I16 ( .A(N_27), .B(N_28), .C(N_26), .Z(N_21) );
AND3 I4 ( .A(N_27), .B(N_28), .C(N_26), .Z(N_4) );
AND2 I17 ( .A(N_19), .B(N_18), .Z(N_20) );
AND2 I8 ( .A(N_28), .B(N_11), .Z(N_15) );
AND2 I9 ( .A(N_13), .B(N_12), .Z(N_16) );
AND2 I5 ( .A(N_10), .B(N_9), .Z(N_5) );

endmodule // RemoteControllerSchematic
