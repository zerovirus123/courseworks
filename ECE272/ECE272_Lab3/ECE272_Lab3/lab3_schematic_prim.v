// Verilog netlist produced by program LSE :  version Diamond (64-bit) 2.2.0.101
// Netlist written on Wed Apr 23 13:05:40 2014
//
// Verilog Description of module lab3_schematic
//

module lab3_schematic (A, B, C, D, seg_a, seg_b, seg_c, seg_d, 
            seg_e, seg_f, seg_g) /* synthesis syn_module_defined=1 */ ;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(3[8:22])
    input A;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(5[8:9])
    input B;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(6[8:9])
    input C;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(7[8:9])
    input D;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(8[8:9])
    output seg_a;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(9[8:13])
    output seg_b;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(10[8:13])
    output seg_c;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(11[8:13])
    output seg_d;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(12[8:13])
    output seg_e;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(13[8:13])
    output seg_f;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(14[8:13])
    output seg_g;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(15[8:13])
    
    wire A_c;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(5[8:9])
    wire B_c;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(6[8:9])
    wire C_c;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(7[8:9])
    wire D_c;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(8[8:9])
    wire seg_a_c;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(9[8:13])
    wire seg_b_c;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(10[8:13])
    wire seg_c_c;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(11[8:13])
    wire seg_d_c;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(12[8:13])
    wire seg_e_c;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(13[8:13])
    wire seg_f_c;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(14[8:13])
    wire seg_g_c;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(15[8:13])
    wire N_31;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(16[6:10])
    wire N_32;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(18[6:10])
    wire N_33;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(20[6:10])
    wire N_34;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(22[6:10])
    wire N_28;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(24[6:10])
    wire N_29;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(26[6:10])
    wire N_30;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(28[6:10])
    wire N_24;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(30[6:10])
    wire N_25;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(32[6:10])
    wire N_26;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(34[6:10])
    wire N_27;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(36[6:10])
    wire N_20;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(38[6:10])
    wire N_21;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(40[6:10])
    wire N_22;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(42[6:10])
    wire N_13;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(44[6:10])
    wire N_14;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(46[6:10])
    wire N_15;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(48[6:10])
    wire N_16;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(50[6:10])
    wire N_17;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(52[6:10])
    wire N_18;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(54[6:10])
    wire N_19;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(56[6:10])
    wire N_1;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(58[6:9])
    wire N_2;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(60[6:9])
    wire N_3;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(62[6:9])
    wire N_4;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(64[6:9])
    wire N_9;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(66[6:9])
    wire N_10;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(68[6:10])
    wire N_11;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(70[6:10])
    wire N_12;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab3/lab3_schematic.v(72[6:10])
    
    wire VCC_net;
    
    OB seg_e_pad (.I(seg_e_c), .O(seg_e));
    OR3 I25 (.A(N_22), .B(N_21), .C(N_20), .Z(seg_e_c)) /* synthesis syn_instantiated=1 */ ;
    OR3 I15 (.A(N_19), .B(N_18), .C(N_17), .Z(seg_c_c)) /* synthesis syn_instantiated=1 */ ;
    AND3 I33 (.A(N_34), .B(N_33), .C(N_32), .Z(N_30)) /* synthesis syn_instantiated=1 */ ;
    AND3 I28 (.A(N_34), .B(N_33), .C(C_c), .Z(N_25)) /* synthesis syn_instantiated=1 */ ;
    AND3 I29 (.A(N_34), .B(N_33), .C(D_c), .Z(N_26)) /* synthesis syn_instantiated=1 */ ;
    AND3 I30 (.A(N_34), .B(C_c), .C(D_c), .Z(N_27)) /* synthesis syn_instantiated=1 */ ;
    AND3 I26 (.A(N_33), .B(N_32), .C(D_c), .Z(N_21)) /* synthesis syn_instantiated=1 */ ;
    AND3 I27 (.A(N_34), .B(B_c), .C(N_32), .Z(N_20)) /* synthesis syn_instantiated=1 */ ;
    AND3 I16 (.A(B_c), .B(C_c), .C(D_c), .Z(N_16)) /* synthesis syn_instantiated=1 */ ;
    AND3 I17 (.A(A_c), .C(N_31), .Z(N_18)) /* synthesis syn_instantiated=1 */ ;
    AND3 I18 (.A(A_c), .B(B_c), .C(C_c), .Z(N_19)) /* synthesis syn_instantiated=1 */ ;
    AND3 I10 (.A(A_c), .B(C_c), .C(D_c), .Z(N_4)) /* synthesis syn_instantiated=1 */ ;
    OR4 I32 (.A(N_27), .B(N_26), .C(N_25), .D(N_24), .Z(seg_f_c)) /* synthesis syn_instantiated=1 */ ;
    OR4 I19 (.A(N_16), .B(N_15), .C(N_14), .D(N_13), .Z(seg_d_c)) /* synthesis syn_instantiated=1 */ ;
    OR4 I11 (.A(N_4), .B(N_3), .C(N_2), .D(N_1), .Z(seg_b_c)) /* synthesis syn_instantiated=1 */ ;
    OR4 I5 (.A(N_11), .B(N_9), .C(N_10), .D(N_12), .Z(seg_a_c)) /* synthesis syn_instantiated=1 */ ;
    AND4 I34 (.A(A_c), .B(N_33), .C(N_32), .D(N_31), .Z(N_28)) /* synthesis syn_instantiated=1 */ ;
    AND4 I35 (.A(N_34), .B(B_c), .C(C_c), .D(D_c), .Z(N_29)) /* synthesis syn_instantiated=1 */ ;
    AND4 I31 (.A(A_c), .B(B_c), .C(N_32), .D(D_c), .Z(N_24)) /* synthesis syn_instantiated=1 */ ;
    AND4 I20 (.A(A_c), .B(N_33), .C(C_c), .D(N_31), .Z(N_13)) /* synthesis syn_instantiated=1 */ ;
    AND4 I21 (.A(N_34), .B(B_c), .C(N_32), .D(N_31), .Z(N_14)) /* synthesis syn_instantiated=1 */ ;
    AND4 I22 (.A(N_34), .B(N_33), .C(N_32), .D(D_c), .Z(N_15)) /* synthesis syn_instantiated=1 */ ;
    AND4 I23 (.A(N_34), .B(N_33), .C(C_c), .D(N_31), .Z(N_17)) /* synthesis syn_instantiated=1 */ ;
    AND4 I14 (.A(A_c), .B(B_c), .C(N_32), .D(N_31), .Z(N_1)) /* synthesis syn_instantiated=1 */ ;
    AND4 I13 (.A(N_34), .B(B_c), .C(C_c), .D(N_31), .Z(N_2)) /* synthesis syn_instantiated=1 */ ;
    AND4 I12 (.A(N_34), .B(B_c), .C(N_32), .D(D_c), .Z(N_3)) /* synthesis syn_instantiated=1 */ ;
    AND4 I9 (.A(N_34), .B(N_33), .C(N_32), .D(D_c), .Z(N_11)) /* synthesis syn_instantiated=1 */ ;
    AND4 I8 (.A(N_34), .B(B_c), .C(N_32), .D(N_31), .Z(N_9)) /* synthesis syn_instantiated=1 */ ;
    AND4 I7 (.A(A_c), .B(B_c), .C(N_32), .D(D_c), .Z(N_10)) /* synthesis syn_instantiated=1 */ ;
    AND4 I6 (.A(A_c), .B(N_33), .C(C_c), .D(D_c), .Z(N_12)) /* synthesis syn_instantiated=1 */ ;
    INV I4 (.A(A_c), .Z(N_34));
    OB seg_d_pad (.I(seg_d_c), .O(seg_d));
    OB seg_c_pad (.I(seg_c_c), .O(seg_c));
    OB seg_b_pad (.I(seg_b_c), .O(seg_b));
    OB seg_a_pad (.I(seg_a_c), .O(seg_a));
    VHI i26 (.Z(VCC_net));
    AND2 I24 (.A(N_34), .B(D_c), .Z(N_22)) /* synthesis syn_instantiated=1 */ ;
    OR3 I36 (.A(N_30), .B(N_29), .C(N_28), .Z(seg_g_c)) /* synthesis syn_instantiated=1 */ ;
    INV I3 (.A(B_c), .Z(N_33));
    INV I1 (.A(C_c), .Z(N_32));
    INV I2 (.A(D_c), .Z(N_31));
    OB seg_f_pad (.I(seg_f_c), .O(seg_f));
    OB seg_g_pad (.I(seg_g_c), .O(seg_g));
    IB A_pad (.I(A), .O(A_c));
    IB B_pad (.I(B), .O(B_c));
    IB C_pad (.I(C), .O(C_c));
    IB D_pad (.I(D), .O(D_c));
    GSR GSR_INST (.GSR(VCC_net));
    PUR PUR_INST (.PUR(VCC_net));
    defparam PUR_INST.RST_PULSE = 1;
    
endmodule
//
// Verilog Description of module PUR
// module not written out since it is a black-box. 
//

