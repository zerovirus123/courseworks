// Verilog netlist produced by program LSE :  version Diamond (64-bit) 2.2.0.101
// Netlist written on Wed May 28 08:35:05 2014
//
// Verilog Description of module DigitalAnalog
//

module DigitalAnalog (reset, R_bump, L_bump, Le, Ldir, Re, Rdir) /* synthesis syn_module_defined=1 */ ;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(2[8:21])
    input reset;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(3[14:19])
    input R_bump;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(4[14:20])
    input L_bump;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(4[22:28])
    output Le;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(5[15:17])
    output Ldir;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(5[19:23])
    output Re;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(5[25:27])
    output Rdir;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(5[29:33])
    
    wire GND_net;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(5[25:27])
    wire reset_c;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(3[14:19])
    wire R_bump_c;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(4[14:20])
    wire L_bump_c;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(4[22:28])
    wire Ldir_c;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(5[19:23])
    wire Rdir_c;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(5[29:33])
    wire osc;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(8[9:12])
    wire clock2;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(8[14:20])
    
    wire VCC_net, n4;
    wire [7:0]n53;
    
    wire n192, n71;
    
    VHI i175 (.Z(VCC_net));
    OSCH osc_int (.STDBY(GND_net), .OSC(osc)) /* synthesis syn_instantiated=1 */ ;
    defparam osc_int.NOM_FREQ = "2.08";
    VLO i1 (.Z(GND_net));
    LUT4 i43_2_lut (.A(R_bump_c), .B(n53[0]), .Z(n71)) /* synthesis lut_function=(!(A+!(B))) */ ;
    defparam i43_2_lut.init = 16'h4444;
    LUT4 i1_3_lut_4_lut (.A(n53[0]), .B(R_bump_c), .C(n53[4]), .D(L_bump_c), 
         .Z(n4)) /* synthesis lut_function=(A (B (C+(D))+!B (C))+!A (C)) */ ;
    defparam i1_3_lut_4_lut.init = 16'hf8f0;
    LUT4 i1_2_lut_3_lut (.A(n53[0]), .B(R_bump_c), .C(L_bump_c), .Z(n192)) /* synthesis lut_function=(!(((C)+!B)+!A)) */ ;
    defparam i1_2_lut_3_lut.init = 16'h0808;
    OB Ldir_pad (.I(Ldir_c), .O(Ldir));
    OB Le_pad (.I(GND_net), .O(Le));
    GSR GSR_INST (.GSR(reset_c));
    PUR PUR_INST (.PUR(VCC_net));
    defparam PUR_INST.RST_PULSE = 1;
    LUT4 i17_2_lut (.A(n53[0]), .B(n53[2]), .Z(Rdir_c)) /* synthesis lut_function=(A+(B)) */ ;
    defparam i17_2_lut.init = 16'heeee;
    LUT4 i14_2_lut (.A(n53[0]), .B(n53[4]), .Z(Ldir_c)) /* synthesis lut_function=(A+(B)) */ ;
    defparam i14_2_lut.init = 16'heeee;
    clock_counter slow (.clock2(clock2), .osc(osc), .GND_net(GND_net)) /* synthesis syn_module_defined=1 */ ;
    FD1S3AX state_FSM_i5 (.D(n53[3]), .CK(clock2), .Q(n53[4]));
    defparam state_FSM_i5.GSR = "ENABLED";
    FD1S3AX state_FSM_i4 (.D(n192), .CK(clock2), .Q(n53[3]));
    defparam state_FSM_i4.GSR = "ENABLED";
    FD1S3AX state_FSM_i3 (.D(n53[1]), .CK(clock2), .Q(n53[2]));
    defparam state_FSM_i3.GSR = "ENABLED";
    FD1S3AX state_FSM_i2 (.D(n71), .CK(clock2), .Q(n53[1]));
    defparam state_FSM_i2.GSR = "ENABLED";
    FD1S3JX state_FSM_i1 (.D(n4), .CK(clock2), .PD(n53[2]), .Q(n53[0]));
    defparam state_FSM_i1.GSR = "ENABLED";
    OB Re_pad (.I(GND_net), .O(Re));
    OB Rdir_pad (.I(Rdir_c), .O(Rdir));
    IB reset_pad (.I(reset), .O(reset_c));
    IB R_bump_pad (.I(R_bump), .O(R_bump_c));
    IB L_bump_pad (.I(L_bump), .O(L_bump_c));
    
endmodule
//
// Verilog Description of module PUR
// module not written out since it is a black-box. 
//

//
// Verilog Description of module clock_counter
//

module clock_counter (clock2, osc, GND_net) /* synthesis syn_module_defined=1 */ ;
    output clock2;
    input osc;
    input GND_net;
    
    wire [20:0]count;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(84[14:19])
    
    wire n31, n29;
    wire [19:0]n86;
    
    wire n12, n186, n7, n10, n187, n12_adj_1, n183, n177, n178, 
        n176, n181, n182, n180, n174, n175, n179;
    
    FD1S3AX clk_o_13 (.D(n31), .CK(osc), .Q(clock2));
    defparam clk_o_13.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i1 (.D(n86[0]), .CK(osc), .CD(n29), .Q(count[0]));
    defparam dff_11_51_52__i1.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i20 (.D(n86[19]), .CK(osc), .CD(n29), .Q(count[19]));
    defparam dff_11_51_52__i20.GSR = "ENABLED";
    LUT4 i15_2_lut (.A(clock2), .B(n29), .Z(n31)) /* synthesis lut_function=(!(A (B)+!A !(B))) */ ;
    defparam i15_2_lut.init = 16'h6666;
    LUT4 i5_4_lut (.A(count[0]), .B(count[4]), .C(count[3]), .D(count[6]), 
         .Z(n12)) /* synthesis lut_function=(A (B (C (D)))) */ ;
    defparam i5_4_lut.init = 16'h8000;
    LUT4 i6_4_lut (.A(count[2]), .B(n12), .C(count[1]), .D(count[5]), 
         .Z(n186)) /* synthesis lut_function=(A (B (C (D)))) */ ;
    defparam i6_4_lut.init = 16'h8000;
    LUT4 i2_4_lut (.A(count[10]), .B(count[8]), .C(count[7]), .D(n186), 
         .Z(n7)) /* synthesis lut_function=(A (B+(C+(D)))) */ ;
    defparam i2_4_lut.init = 16'haaa8;
    LUT4 i3_2_lut (.A(count[17]), .B(count[19]), .Z(n10)) /* synthesis lut_function=(A (B)) */ ;
    defparam i3_2_lut.init = 16'h8888;
    LUT4 i4_4_lut (.A(n7), .B(count[12]), .C(count[11]), .D(count[9]), 
         .Z(n187)) /* synthesis lut_function=(A (B (C (D)))) */ ;
    defparam i4_4_lut.init = 16'h8000;
    LUT4 i5_4_lut_adj_1 (.A(n187), .B(n10), .C(count[16]), .D(count[13]), 
         .Z(n12_adj_1)) /* synthesis lut_function=(A (B (C))+!A (B (C (D)))) */ ;
    defparam i5_4_lut_adj_1.init = 16'hc080;
    LUT4 i6_4_lut_adj_2 (.A(count[15]), .B(n12_adj_1), .C(count[14]), 
         .D(count[18]), .Z(n29)) /* synthesis lut_function=(A (B (C (D)))) */ ;
    defparam i6_4_lut_adj_2.init = 16'h8000;
    CCU2D dff_11_51_52_add_4_21 (.A0(count[19]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(GND_net), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .CIN(n183), .S0(n86[19]));
    defparam dff_11_51_52_add_4_21.INIT0 = 16'hfaaa;
    defparam dff_11_51_52_add_4_21.INIT1 = 16'h0000;
    defparam dff_11_51_52_add_4_21.INJECT1_0 = "NO";
    defparam dff_11_51_52_add_4_21.INJECT1_1 = "NO";
    CCU2D dff_11_51_52_add_4_9 (.A0(count[7]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[8]), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .CIN(n177), .COUT(n178), .S0(n86[7]), .S1(n86[8]));
    defparam dff_11_51_52_add_4_9.INIT0 = 16'hfaaa;
    defparam dff_11_51_52_add_4_9.INIT1 = 16'hfaaa;
    defparam dff_11_51_52_add_4_9.INJECT1_0 = "NO";
    defparam dff_11_51_52_add_4_9.INJECT1_1 = "NO";
    CCU2D dff_11_51_52_add_4_7 (.A0(count[5]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[6]), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .CIN(n176), .COUT(n177), .S0(n86[5]), .S1(n86[6]));
    defparam dff_11_51_52_add_4_7.INIT0 = 16'hfaaa;
    defparam dff_11_51_52_add_4_7.INIT1 = 16'hfaaa;
    defparam dff_11_51_52_add_4_7.INJECT1_0 = "NO";
    defparam dff_11_51_52_add_4_7.INJECT1_1 = "NO";
    CCU2D dff_11_51_52_add_4_17 (.A0(count[15]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[16]), .B1(GND_net), .C1(GND_net), 
          .D1(GND_net), .CIN(n181), .COUT(n182), .S0(n86[15]), .S1(n86[16]));
    defparam dff_11_51_52_add_4_17.INIT0 = 16'hfaaa;
    defparam dff_11_51_52_add_4_17.INIT1 = 16'hfaaa;
    defparam dff_11_51_52_add_4_17.INJECT1_0 = "NO";
    defparam dff_11_51_52_add_4_17.INJECT1_1 = "NO";
    CCU2D dff_11_51_52_add_4_15 (.A0(count[13]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[14]), .B1(GND_net), .C1(GND_net), 
          .D1(GND_net), .CIN(n180), .COUT(n181), .S0(n86[13]), .S1(n86[14]));
    defparam dff_11_51_52_add_4_15.INIT0 = 16'hfaaa;
    defparam dff_11_51_52_add_4_15.INIT1 = 16'hfaaa;
    defparam dff_11_51_52_add_4_15.INJECT1_0 = "NO";
    defparam dff_11_51_52_add_4_15.INJECT1_1 = "NO";
    CCU2D dff_11_51_52_add_4_3 (.A0(count[1]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[2]), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .CIN(n174), .COUT(n175), .S0(n86[1]), .S1(n86[2]));
    defparam dff_11_51_52_add_4_3.INIT0 = 16'hfaaa;
    defparam dff_11_51_52_add_4_3.INIT1 = 16'hfaaa;
    defparam dff_11_51_52_add_4_3.INJECT1_0 = "NO";
    defparam dff_11_51_52_add_4_3.INJECT1_1 = "NO";
    CCU2D dff_11_51_52_add_4_1 (.A0(GND_net), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[0]), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .COUT(n174), .S1(n86[0]));
    defparam dff_11_51_52_add_4_1.INIT0 = 16'hF000;
    defparam dff_11_51_52_add_4_1.INIT1 = 16'h0555;
    defparam dff_11_51_52_add_4_1.INJECT1_0 = "NO";
    defparam dff_11_51_52_add_4_1.INJECT1_1 = "NO";
    CCU2D dff_11_51_52_add_4_5 (.A0(count[3]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[4]), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .CIN(n175), .COUT(n176), .S0(n86[3]), .S1(n86[4]));
    defparam dff_11_51_52_add_4_5.INIT0 = 16'hfaaa;
    defparam dff_11_51_52_add_4_5.INIT1 = 16'hfaaa;
    defparam dff_11_51_52_add_4_5.INJECT1_0 = "NO";
    defparam dff_11_51_52_add_4_5.INJECT1_1 = "NO";
    CCU2D dff_11_51_52_add_4_13 (.A0(count[11]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[12]), .B1(GND_net), .C1(GND_net), 
          .D1(GND_net), .CIN(n179), .COUT(n180), .S0(n86[11]), .S1(n86[12]));
    defparam dff_11_51_52_add_4_13.INIT0 = 16'hfaaa;
    defparam dff_11_51_52_add_4_13.INIT1 = 16'hfaaa;
    defparam dff_11_51_52_add_4_13.INJECT1_0 = "NO";
    defparam dff_11_51_52_add_4_13.INJECT1_1 = "NO";
    CCU2D dff_11_51_52_add_4_11 (.A0(count[9]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[10]), .B1(GND_net), .C1(GND_net), 
          .D1(GND_net), .CIN(n178), .COUT(n179), .S0(n86[9]), .S1(n86[10]));
    defparam dff_11_51_52_add_4_11.INIT0 = 16'hfaaa;
    defparam dff_11_51_52_add_4_11.INIT1 = 16'hfaaa;
    defparam dff_11_51_52_add_4_11.INJECT1_0 = "NO";
    defparam dff_11_51_52_add_4_11.INJECT1_1 = "NO";
    FD1S3IX dff_11_51_52__i19 (.D(n86[18]), .CK(osc), .CD(n29), .Q(count[18]));
    defparam dff_11_51_52__i19.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i18 (.D(n86[17]), .CK(osc), .CD(n29), .Q(count[17]));
    defparam dff_11_51_52__i18.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i17 (.D(n86[16]), .CK(osc), .CD(n29), .Q(count[16]));
    defparam dff_11_51_52__i17.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i16 (.D(n86[15]), .CK(osc), .CD(n29), .Q(count[15]));
    defparam dff_11_51_52__i16.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i15 (.D(n86[14]), .CK(osc), .CD(n29), .Q(count[14]));
    defparam dff_11_51_52__i15.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i14 (.D(n86[13]), .CK(osc), .CD(n29), .Q(count[13]));
    defparam dff_11_51_52__i14.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i13 (.D(n86[12]), .CK(osc), .CD(n29), .Q(count[12]));
    defparam dff_11_51_52__i13.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i12 (.D(n86[11]), .CK(osc), .CD(n29), .Q(count[11]));
    defparam dff_11_51_52__i12.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i11 (.D(n86[10]), .CK(osc), .CD(n29), .Q(count[10]));
    defparam dff_11_51_52__i11.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i10 (.D(n86[9]), .CK(osc), .CD(n29), .Q(count[9]));
    defparam dff_11_51_52__i10.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i9 (.D(n86[8]), .CK(osc), .CD(n29), .Q(count[8]));
    defparam dff_11_51_52__i9.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i8 (.D(n86[7]), .CK(osc), .CD(n29), .Q(count[7]));
    defparam dff_11_51_52__i8.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i7 (.D(n86[6]), .CK(osc), .CD(n29), .Q(count[6]));
    defparam dff_11_51_52__i7.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i6 (.D(n86[5]), .CK(osc), .CD(n29), .Q(count[5]));
    defparam dff_11_51_52__i6.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i5 (.D(n86[4]), .CK(osc), .CD(n29), .Q(count[4]));
    defparam dff_11_51_52__i5.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i4 (.D(n86[3]), .CK(osc), .CD(n29), .Q(count[3]));
    defparam dff_11_51_52__i4.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i3 (.D(n86[2]), .CK(osc), .CD(n29), .Q(count[2]));
    defparam dff_11_51_52__i3.GSR = "ENABLED";
    FD1S3IX dff_11_51_52__i2 (.D(n86[1]), .CK(osc), .CD(n29), .Q(count[1]));
    defparam dff_11_51_52__i2.GSR = "ENABLED";
    CCU2D dff_11_51_52_add_4_19 (.A0(count[17]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[18]), .B1(GND_net), .C1(GND_net), 
          .D1(GND_net), .CIN(n182), .COUT(n183), .S0(n86[17]), .S1(n86[18]));
    defparam dff_11_51_52_add_4_19.INIT0 = 16'hfaaa;
    defparam dff_11_51_52_add_4_19.INIT1 = 16'hfaaa;
    defparam dff_11_51_52_add_4_19.INJECT1_0 = "NO";
    defparam dff_11_51_52_add_4_19.INJECT1_1 = "NO";
    
endmodule
