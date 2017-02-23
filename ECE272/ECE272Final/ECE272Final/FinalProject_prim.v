// Verilog netlist produced by program LSE :  version Diamond (64-bit) 2.2.0.101
// Netlist written on Wed May 28 13:13:48 2014
//
// Verilog Description of module FinalProject
//

module FinalProject (reset_n, in, CS_n, RD_n, WR_n, led) /* synthesis syn_module_defined=1 */ ;   // z:/windows.documents/desktop/ece272final/finalproject.v(1[8:20])
    input reset_n;   // z:/windows.documents/desktop/ece272final/finalproject.v(1[28:35])
    input [7:0]in;   // z:/windows.documents/desktop/ece272final/finalproject.v(2[25:27])
    output CS_n;   // z:/windows.documents/desktop/ece272final/finalproject.v(3[14:18])
    output RD_n;   // z:/windows.documents/desktop/ece272final/finalproject.v(4[14:18])
    output WR_n;   // z:/windows.documents/desktop/ece272final/finalproject.v(5[14:18])
    output [7:0]led;   // z:/windows.documents/desktop/ece272final/finalproject.v(6[20:23])
    
    wire GND_net;   // z:/windows.documents/desktop/ece272final/finalproject.v(11[11:14])
    wire reset_n_c;   // z:/windows.documents/desktop/ece272final/finalproject.v(1[28:35])
    wire [7:0]in_c;   // z:/windows.documents/desktop/ece272final/finalproject.v(2[25:27])
    wire WR_n_c;   // z:/windows.documents/desktop/ece272final/finalproject.v(5[14:18])
    wire osc;   // z:/windows.documents/desktop/ece272final/finalproject.v(16[8:11])
    
    wire VCC_net;
    
    VHI i125 (.Z(VCC_net));
    OSCH osc_int (.STDBY(GND_net), .OSC(osc)) /* synthesis syn_instantiated=1 */ ;
    defparam osc_int.NOM_FREQ = "2.08";
    OB RD_n_pad (.I(GND_net), .O(RD_n));
    VLO i1 (.Z(GND_net));
    ADCinterface ADC (.WR_n_c(WR_n_c), .osc(osc), .GND_net(GND_net)) /* synthesis syn_module_defined=1 */ ;
    OB CS_n_pad (.I(GND_net), .O(CS_n));
    GSR GSR_INST (.GSR(reset_n_c));
    PUR PUR_INST (.PUR(VCC_net));
    defparam PUR_INST.RST_PULSE = 1;
    OB WR_n_pad (.I(WR_n_c), .O(WR_n));
    OB led_pad_7 (.I(in_c[7]), .O(led[7]));
    OB led_pad_6 (.I(in_c[6]), .O(led[6]));
    OB led_pad_5 (.I(in_c[5]), .O(led[5]));
    OB led_pad_4 (.I(in_c[4]), .O(led[4]));
    OB led_pad_3 (.I(in_c[3]), .O(led[3]));
    OB led_pad_2 (.I(in_c[2]), .O(led[2]));
    OB led_pad_1 (.I(in_c[1]), .O(led[1]));
    OB led_pad_0 (.I(in_c[0]), .O(led[0]));
    IB reset_n_pad (.I(reset_n), .O(reset_n_c));
    IB n22_pad (.I(in[7]), .O(in_c[7]));
    IB n23_pad (.I(in[6]), .O(in_c[6]));
    IB n24_pad (.I(in[5]), .O(in_c[5]));
    IB n25_pad (.I(in[4]), .O(in_c[4]));
    IB n26_pad (.I(in[3]), .O(in_c[3]));
    IB n27_pad (.I(in[2]), .O(in_c[2]));
    IB n28_pad (.I(in[1]), .O(in_c[1]));
    IB led_c_pad (.I(in[0]), .O(in_c[0]));
    
endmodule
//
// Verilog Description of module ADCinterface
//

module ADCinterface (WR_n_c, osc, GND_net) /* synthesis syn_module_defined=1 */ ;
    output WR_n_c;
    input osc;
    input GND_net;
    
    wire [7:0]count;   // z:/windows.documents/desktop/ece272final/finalproject.v(44[12:17])
    
    wire n40, n14;
    wire [7:0]n38;
    
    wire n29, n15, n133, n6, n115, n113, n110, n111, n112;
    
    FD1S3AX WR_n_17 (.D(n40), .CK(osc), .Q(WR_n_c));
    defparam WR_n_17.GSR = "ENABLED";
    FD1S3IX dff_15_11__i7 (.D(n38[7]), .CK(osc), .CD(n14), .Q(count[7]));
    defparam dff_15_11__i7.GSR = "ENABLED";
    LUT4 i9_2_lut (.A(n29), .B(n15), .Z(n14)) /* synthesis lut_function=(!(A+!(B))) */ ;
    defparam i9_2_lut.init = 16'h4444;
    LUT4 i2_3_lut (.A(count[5]), .B(count[1]), .C(count[0]), .Z(n133)) /* synthesis lut_function=(A+(B+(C))) */ ;
    defparam i2_3_lut.init = 16'hfefe;
    LUT4 i1_3_lut (.A(count[7]), .B(count[4]), .C(count[2]), .Z(n6)) /* synthesis lut_function=(A+!(B (C))) */ ;
    defparam i1_3_lut.init = 16'hbfbf;
    LUT4 i4_4_lut (.A(count[6]), .B(n133), .C(count[3]), .D(n6), .Z(n15)) /* synthesis lut_function=(A+(B+(C+(D)))) */ ;
    defparam i4_4_lut.init = 16'hfffe;
    LUT4 i2_3_lut_adj_1 (.A(count[3]), .B(count[7]), .C(count[6]), .Z(n115)) /* synthesis lut_function=(A (B (C))) */ ;
    defparam i2_3_lut_adj_1.init = 16'h8080;
    LUT4 i3_4_lut (.A(count[4]), .B(n115), .C(count[2]), .D(n133), .Z(n29)) /* synthesis lut_function=(A+((C+(D))+!B)) */ ;
    defparam i3_4_lut.init = 16'hfffb;
    LUT4 i13_3_lut (.A(WR_n_c), .B(n29), .C(n15), .Z(n40)) /* synthesis lut_function=(A (B (C))+!A !(B (C))) */ ;
    defparam i13_3_lut.init = 16'h9595;
    CCU2D dff_15_11_add_4_9 (.A0(count[7]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(GND_net), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .CIN(n113), .S0(n38[7]));
    defparam dff_15_11_add_4_9.INIT0 = 16'hfaaa;
    defparam dff_15_11_add_4_9.INIT1 = 16'h0000;
    defparam dff_15_11_add_4_9.INJECT1_0 = "NO";
    defparam dff_15_11_add_4_9.INJECT1_1 = "NO";
    CCU2D dff_15_11_add_4_3 (.A0(count[1]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[2]), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .CIN(n110), .COUT(n111), .S0(n38[1]), .S1(n38[2]));
    defparam dff_15_11_add_4_3.INIT0 = 16'hfaaa;
    defparam dff_15_11_add_4_3.INIT1 = 16'hfaaa;
    defparam dff_15_11_add_4_3.INJECT1_0 = "NO";
    defparam dff_15_11_add_4_3.INJECT1_1 = "NO";
    CCU2D dff_15_11_add_4_7 (.A0(count[5]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[6]), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .CIN(n112), .COUT(n113), .S0(n38[5]), .S1(n38[6]));
    defparam dff_15_11_add_4_7.INIT0 = 16'hfaaa;
    defparam dff_15_11_add_4_7.INIT1 = 16'hfaaa;
    defparam dff_15_11_add_4_7.INJECT1_0 = "NO";
    defparam dff_15_11_add_4_7.INJECT1_1 = "NO";
    CCU2D dff_15_11_add_4_1 (.A0(GND_net), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[0]), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .COUT(n110), .S1(n38[0]));
    defparam dff_15_11_add_4_1.INIT0 = 16'hF000;
    defparam dff_15_11_add_4_1.INIT1 = 16'h0555;
    defparam dff_15_11_add_4_1.INJECT1_0 = "NO";
    defparam dff_15_11_add_4_1.INJECT1_1 = "NO";
    FD1S3IX dff_15_11__i6 (.D(n38[6]), .CK(osc), .CD(n14), .Q(count[6]));
    defparam dff_15_11__i6.GSR = "ENABLED";
    FD1S3IX dff_15_11__i5 (.D(n38[5]), .CK(osc), .CD(n14), .Q(count[5]));
    defparam dff_15_11__i5.GSR = "ENABLED";
    FD1S3IX dff_15_11__i4 (.D(n38[4]), .CK(osc), .CD(n14), .Q(count[4]));
    defparam dff_15_11__i4.GSR = "ENABLED";
    FD1S3IX dff_15_11__i3 (.D(n38[3]), .CK(osc), .CD(n14), .Q(count[3]));
    defparam dff_15_11__i3.GSR = "ENABLED";
    FD1S3IX dff_15_11__i2 (.D(n38[2]), .CK(osc), .CD(n14), .Q(count[2]));
    defparam dff_15_11__i2.GSR = "ENABLED";
    FD1S3IX dff_15_11__i1 (.D(n38[1]), .CK(osc), .CD(n14), .Q(count[1]));
    defparam dff_15_11__i1.GSR = "ENABLED";
    FD1S3IX dff_15_11__i0 (.D(n38[0]), .CK(osc), .CD(n14), .Q(count[0]));
    defparam dff_15_11__i0.GSR = "ENABLED";
    CCU2D dff_15_11_add_4_5 (.A0(count[3]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[4]), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .CIN(n111), .COUT(n112), .S0(n38[3]), .S1(n38[4]));
    defparam dff_15_11_add_4_5.INIT0 = 16'hfaaa;
    defparam dff_15_11_add_4_5.INIT1 = 16'hfaaa;
    defparam dff_15_11_add_4_5.INJECT1_0 = "NO";
    defparam dff_15_11_add_4_5.INJECT1_1 = "NO";
    
endmodule
//
// Verilog Description of module PUR
// module not written out since it is a black-box. 
//

