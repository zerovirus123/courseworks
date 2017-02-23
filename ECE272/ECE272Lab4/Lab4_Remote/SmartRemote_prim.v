// Verilog netlist produced by program LSE :  version Diamond (64-bit) 2.2.0.101
// Netlist written on Mon May 12 13:31:50 2014
//
// Verilog Description of module SmartRemote
//

module SmartRemote (A, B, C, ANLG_Ldir, ANLG_Rdir, LE_o, LD_o, 
            RE_o, RD_o) /* synthesis syn_module_defined=1 */ ;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(1[8:19])
    input A /* synthesis black_box_pad_pin=1 */ ;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(2[8:9])
    input B /* synthesis black_box_pad_pin=1 */ ;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(3[8:9])
    input C /* synthesis black_box_pad_pin=1 */ ;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(4[8:9])
    input ANLG_Ldir;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(5[8:17])
    input ANLG_Rdir;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(6[8:17])
    output LE_o;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(7[9:13])
    output LD_o;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(8[9:13])
    output RE_o;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(9[9:13])
    output RD_o;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(10[9:13])
    
    wire ANLG_Ldir_c;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(5[8:17])
    wire ANLG_Rdir_c;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(6[8:17])
    wire LE_o_c;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(7[9:13])
    wire LD_o_c;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(8[9:13])
    wire RE_o_c;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(9[9:13])
    wire RD_o_c;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(10[9:13])
    wire LEN;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(14[7:10])
    wire LDIR;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(14[12:16])
    wire REN;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(14[18:21])
    wire N_28;   // z:/windows.documents/desktop/ece272lab4/lab4_remote/remotecontrollerschematic.v(11[6:10])
    wire N_27;   // z:/windows.documents/desktop/ece272lab4/lab4_remote/remotecontrollerschematic.v(13[6:10])
    wire N_26;   // z:/windows.documents/desktop/ece272lab4/lab4_remote/remotecontrollerschematic.v(15[6:10])
    
    wire VCC_net;
    
    VHI i18 (.Z(VCC_net));
    LUT4 i1_2_lut_3_lut (.A(ANLG_Ldir_c), .B(ANLG_Rdir_c), .C(REN), .Z(RE_o_c)) /* synthesis lut_function=(A (B (C))) */ ;
    defparam i1_2_lut_3_lut.init = 16'h8080;
    GSR GSR_INST (.GSR(VCC_net));
    RemoteControllerSchematic myremote (.N_28(N_28), .N_27(N_27), .N_26(N_26), 
            .LEN(LEN), .REN(REN), .LDIR(LDIR)) /* synthesis syn_module_defined=1 */ ;
    OB LD_o_pad (.I(LD_o_c), .O(LD_o));
    IB I24 (.I(B), .O(N_28)) /* synthesis syn_instantiated=1 */ ;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(18[30] 26[3])
    IB I23 (.I(C), .O(N_26)) /* synthesis syn_instantiated=1 */ ;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(18[30] 26[3])
    IB I25 (.I(A), .O(N_27)) /* synthesis syn_instantiated=1 */ ;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(18[30] 26[3])
    OB LE_o_pad (.I(LE_o_c), .O(LE_o));
    OB RD_o_pad (.I(RD_o_c), .O(RD_o));
    OB RE_o_pad (.I(RE_o_c), .O(RE_o));
    IB ANLG_Rdir_pad (.I(ANLG_Rdir), .O(ANLG_Rdir_c));
    IB ANLG_Ldir_pad (.I(ANLG_Ldir), .O(ANLG_Ldir_c));
    mux2 RD_MUX (.ANLG_Ldir_c(ANLG_Ldir_c), .ANLG_Rdir_c(ANLG_Rdir_c), .N_27(N_27), 
         .RD_o_c(RD_o_c)) /* synthesis syn_module_defined=1 */ ;
    LUT4 i1_2_lut_3_lut_adj_1 (.A(ANLG_Ldir_c), .B(ANLG_Rdir_c), .C(LEN), 
         .Z(LE_o_c)) /* synthesis lut_function=(A (B (C))) */ ;
    defparam i1_2_lut_3_lut_adj_1.init = 16'h8080;
    PUR PUR_INST (.PUR(VCC_net));
    defparam PUR_INST.RST_PULSE = 1;
    mux2_U0 LD_MUX (.ANLG_Rdir_c(ANLG_Rdir_c), .ANLG_Ldir_c(ANLG_Ldir_c), 
            .LDIR(LDIR), .LD_o_c(LD_o_c)) /* synthesis syn_module_defined=1 */ ;
    
endmodule
//
// Verilog Description of module RemoteControllerSchematic
//

module RemoteControllerSchematic (N_28, N_27, N_26, LEN, REN, LDIR) /* synthesis syn_module_defined=1 */ ;
    input N_28;
    input N_27;
    input N_26;
    output LEN;
    output REN;
    output LDIR;
    
    wire N_4;   // z:/windows.documents/desktop/ece272lab4/lab4_remote/remotecontrollerschematic.v(39[6:9])
    wire N_11;   // z:/windows.documents/desktop/ece272lab4/lab4_remote/remotecontrollerschematic.v(29[6:10])
    wire N_9;   // z:/windows.documents/desktop/ece272lab4/lab4_remote/remotecontrollerschematic.v(35[6:9])
    wire N_10;   // z:/windows.documents/desktop/ece272lab4/lab4_remote/remotecontrollerschematic.v(37[6:10])
    wire N_5;   // z:/windows.documents/desktop/ece272lab4/lab4_remote/remotecontrollerschematic.v(41[6:9])
    wire N_16;   // z:/windows.documents/desktop/ece272lab4/lab4_remote/remotecontrollerschematic.v(27[6:10])
    wire N_21;   // z:/windows.documents/desktop/ece272lab4/lab4_remote/remotecontrollerschematic.v(19[6:10])
    wire N_15;   // z:/windows.documents/desktop/ece272lab4/lab4_remote/remotecontrollerschematic.v(25[6:10])
    
    AND3 I4 (.A(N_27), .B(N_28), .C(N_26), .Z(N_4)) /* synthesis syn_instantiated=1 */ ;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(18[30] 26[3])
    INV I10 (.A(N_26), .Z(N_11));   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(18[30] 26[3])
    INV I6 (.A(N_28), .Z(N_9));   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(18[30] 26[3])
    INV I7 (.A(N_27), .Z(N_10));   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(18[30] 26[3])
    OR2 I3 (.A(N_5), .B(N_4), .Z(LEN)) /* synthesis syn_instantiated=1 */ ;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(18[30] 26[3])
    OR2 I18 (.A(N_16), .B(N_21), .Z(REN)) /* synthesis syn_instantiated=1 */ ;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(18[30] 26[3])
    OR2 I13 (.A(N_16), .B(N_15), .Z(LDIR)) /* synthesis syn_instantiated=1 */ ;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(18[30] 26[3])
    AND3 I16 (.A(N_27), .B(N_28), .C(N_26), .Z(N_21)) /* synthesis syn_instantiated=1 */ ;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(18[30] 26[3])
    AND2 I8 (.A(N_28), .B(N_11), .Z(N_15)) /* synthesis syn_instantiated=1 */ ;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(18[30] 26[3])
    AND2 I9 (.A(N_9), .B(N_11), .Z(N_16)) /* synthesis syn_instantiated=1 */ ;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(18[30] 26[3])
    AND2 I5 (.A(N_10), .B(N_9), .Z(N_5)) /* synthesis syn_instantiated=1 */ ;   // z:/windows.documents/desktop/ece272lab4/smart_remote.v(18[30] 26[3])
    
endmodule
//
// Verilog Description of module mux2
//

module mux2 (ANLG_Ldir_c, ANLG_Rdir_c, N_27, RD_o_c) /* synthesis syn_module_defined=1 */ ;
    input ANLG_Ldir_c;
    input ANLG_Rdir_c;
    input N_27;
    output RD_o_c;
    
    
    LUT4 i15_3_lut (.A(ANLG_Ldir_c), .B(ANLG_Rdir_c), .C(N_27), .Z(RD_o_c)) /* synthesis lut_function=(!(A ((C)+!B)+!A !(B))) */ ;
    defparam i15_3_lut.init = 16'h4c4c;
    
endmodule
//
// Verilog Description of module PUR
// module not written out since it is a black-box. 
//

//
// Verilog Description of module mux2_U0
//

module mux2_U0 (ANLG_Rdir_c, ANLG_Ldir_c, LDIR, LD_o_c) /* synthesis syn_module_defined=1 */ ;
    input ANLG_Rdir_c;
    input ANLG_Ldir_c;
    input LDIR;
    output LD_o_c;
    
    
    LUT4 i12_3_lut (.A(ANLG_Rdir_c), .B(ANLG_Ldir_c), .C(LDIR), .Z(LD_o_c)) /* synthesis lut_function=(A (B (C))+!A (B)) */ ;
    defparam i12_3_lut.init = 16'hc4c4;
    
endmodule
