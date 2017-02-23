// Verilog netlist produced by program LSE :  version Diamond (64-bit) 2.2.0.101
// Netlist written on Thu Apr 17 18:56:21 2014
//
// Verilog Description of module RemoteControllerSchematic
//

module RemoteControllerSchematic (A, B, C, Ld, Le, Rd, Re) /* synthesis syn_module_defined=1 */ ;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(3[8:33])
    input A /* synthesis black_box_pad_pin=1 */ ;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(4[8:9])
    input B /* synthesis black_box_pad_pin=1 */ ;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(5[8:9])
    input C /* synthesis black_box_pad_pin=1 */ ;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(6[8:9])
    output Ld;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(7[8:10])
    output Le;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(8[8:10])
    output Rd;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(9[8:10])
    output Re;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(10[8:10])
    
    wire Ld_c;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(7[8:10])
    wire Le_c;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(8[8:10])
    wire Rd_c;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(9[8:10])
    wire Re_c;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(10[8:10])
    wire N_28;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(11[6:10])
    wire N_27;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(13[6:10])
    wire N_26;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(15[6:10])
    wire N_20;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(17[6:10])
    wire N_21;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(19[6:10])
    wire N_18;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(21[6:10])
    wire N_19;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(23[6:10])
    wire N_15;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(25[6:10])
    wire N_16;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(27[6:10])
    wire N_4;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(39[6:9])
    wire N_5;   // z:/windows.documents/desktop/ece272 lab2/ece272_lab2/remotecontrollerschematic.v(41[6:9])
    
    wire VCC_net;
    
    AND3 I4 (.A(N_27), .B(N_28), .C(N_26), .Z(N_4)) /* synthesis syn_instantiated=1 */ ;
    AND2 I17 (.A(N_19), .B(N_18), .Z(N_20)) /* synthesis syn_instantiated=1 */ ;
    PUR PUR_INST (.PUR(VCC_net));
    defparam PUR_INST.RST_PULSE = 1;
    VHI i34 (.Z(VCC_net));
    IB I23 (.I(C), .O(N_26)) /* synthesis syn_instantiated=1 */ ;
    IB I24 (.I(B), .O(N_28)) /* synthesis syn_instantiated=1 */ ;
    IB I25 (.I(A), .O(N_27)) /* synthesis syn_instantiated=1 */ ;
    INV I20 (.A(N_27), .Z(Rd_c));
    INV I14 (.A(N_26), .Z(N_18));
    INV I15 (.A(N_28), .Z(N_19));
    GSR GSR_INST (.GSR(VCC_net));
    OB Re_pad (.I(Re_c), .O(Re));
    OB Rd_pad (.I(Rd_c), .O(Rd));
    OB Le_pad (.I(Le_c), .O(Le));
    OB Ld_pad (.I(Ld_c), .O(Ld));
    OR2 I3 (.A(N_5), .B(N_4), .Z(Le_c)) /* synthesis syn_instantiated=1 */ ;
    OR2 I18 (.A(N_20), .B(N_21), .Z(Re_c)) /* synthesis syn_instantiated=1 */ ;
    OR2 I13 (.A(N_16), .B(N_15), .Z(Ld_c)) /* synthesis syn_instantiated=1 */ ;
    AND3 I16 (.A(N_27), .B(N_28), .C(N_26), .Z(N_21)) /* synthesis syn_instantiated=1 */ ;
    AND2 I8 (.A(N_28), .B(N_18), .Z(N_15)) /* synthesis syn_instantiated=1 */ ;
    AND2 I9 (.A(N_19), .B(N_18), .Z(N_16)) /* synthesis syn_instantiated=1 */ ;
    AND2 I5 (.A(Rd_c), .B(N_19), .Z(N_5)) /* synthesis syn_instantiated=1 */ ;
    
endmodule
//
// Verilog Description of module PUR
// module not written out since it is a black-box. 
//

