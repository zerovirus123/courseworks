/* Verilog model created from schematic lab3_schematic.sch -- Apr 30, 2014 13:37 */

module lab3_schematic( A, B, C, D, seg_a, seg_b, seg_c, seg_d, seg_e, seg_f,
                       seg_g );
 input A;
 input B;
 input C;
 input D;
output seg_a;
output seg_b;
output seg_c;
output seg_d;
output seg_e;
output seg_f;
output seg_g;
wire N_35;

wire N_36;

wire N_37;

wire N_38;

wire N_31;

wire N_32;

wire N_33;

wire N_34;

wire N_28;

wire N_29;

wire N_30;

wire N_24;

wire N_25;

wire N_26;

wire N_27;

wire N_20;

wire N_21;

wire N_22;

wire N_13;

wire N_14;

wire N_15;

wire N_16;

wire N_17;

wire N_18;

wire N_19;

wire N_1;

wire N_2;

wire N_3;

wire N_4;

wire N_9;

wire N_10;

wire N_11;

wire N_12;




AND2 I24 ( .A(N_34), .B(N_36), .Z(N_22) );
OR3 I36 ( .A(N_30), .B(N_29), .C(N_28), .Z(seg_g) );
OR3 I25 ( .A(N_22), .B(N_21), .C(N_20), .Z(seg_e) );
OR3 I15 ( .A(N_19), .B(N_18), .C(N_17), .Z(seg_c) );
AND3 I33 ( .A(N_34), .B(N_33), .C(N_32), .Z(N_30) );
AND3 I28 ( .A(N_34), .B(N_33), .C(N_37), .Z(N_25) );
AND3 I29 ( .A(N_34), .B(N_33), .C(N_36), .Z(N_26) );
AND3 I30 ( .A(N_34), .B(N_37), .C(N_36), .Z(N_27) );
AND3 I26 ( .A(N_33), .B(N_32), .C(N_36), .Z(N_21) );
AND3 I27 ( .A(N_34), .B(N_38), .C(N_32), .Z(N_20) );
AND3 I16 ( .A(N_38), .B(N_37), .C(N_36), .Z(N_16) );
AND3 I17 ( .A(N_35), .C(N_31), .Z(N_18) );
AND3 I18 ( .A(N_35), .B(N_38), .C(N_37), .Z(N_19) );
AND3 I10 ( .A(N_35), .B(N_37), .C(N_36), .Z(N_4) );
OR4 I32 ( .A(N_27), .B(N_26), .C(N_25), .D(N_24), .Z(seg_f) );
OR4 I19 ( .A(N_16), .B(N_15), .C(N_14), .D(N_13), .Z(seg_d) );
OR4 I11 ( .A(N_4), .B(N_3), .C(N_2), .D(N_1), .Z(seg_b) );
OR4 I5 ( .A(N_11), .B(N_9), .C(N_10), .D(N_12), .Z(seg_a) );
AND4 I34 ( .A(N_35), .B(N_33), .C(N_32), .D(N_31), .Z(N_28) );
AND4 I35 ( .A(N_34), .B(N_38), .C(N_37), .D(N_36), .Z(N_29) );
AND4 I31 ( .A(N_35), .B(N_38), .C(N_32), .D(N_36), .Z(N_24) );
AND4 I20 ( .A(N_35), .B(N_33), .C(N_37), .D(N_31), .Z(N_13) );
AND4 I21 ( .A(N_34), .B(N_38), .C(N_32), .D(N_31), .Z(N_14) );
AND4 I22 ( .A(N_34), .B(N_33), .C(N_32), .D(N_36), .Z(N_15) );
AND4 I23 ( .A(N_34), .B(N_33), .C(N_37), .D(N_31), .Z(N_17) );
AND4 I14 ( .A(N_35), .B(N_38), .C(N_32), .D(N_31), .Z(N_1) );
AND4 I13 ( .A(N_34), .B(N_38), .C(N_37), .D(N_31), .Z(N_2) );
AND4 I12 ( .A(N_34), .B(N_38), .C(N_32), .D(N_36), .Z(N_3) );
AND4 I9 ( .A(N_34), .B(N_33), .C(N_32), .D(N_36), .Z(N_11) );
AND4 I8 ( .A(N_34), .B(N_38), .C(N_32), .D(N_31), .Z(N_9) );
AND4 I7 ( .A(N_35), .B(N_38), .C(N_32), .D(N_36), .Z(N_10) );
AND4 I6 ( .A(N_35), .B(N_33), .C(N_37), .D(N_36), .Z(N_12) );
INV I37 ( .A(D), .Z(N_36) );
INV I38 ( .A(C), .Z(N_37) );
INV I39 ( .A(B), .Z(N_38) );
INV I40 ( .A(A), .Z(N_35) );
INV I4 ( .A(N_35), .Z(N_34) );
INV I3 ( .A(N_38), .Z(N_33) );
INV I1 ( .A(N_37), .Z(N_32) );
INV I2 ( .A(N_36), .Z(N_31) );

endmodule // lab3_schematic
