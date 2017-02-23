// Verilog netlist produced by program LSE :  version Diamond (64-bit) 2.2.0.101
// Netlist written on Wed Jun 04 10:25:33 2014
//
// Verilog Description of module checkpoint2
//

module checkpoint2 (reset, a_o, b_o, c_o, d_o, e_o, f_o, g_o, 
            display) /* synthesis syn_module_defined=1 */ ;   // z:/windows.documents/desktop/ece272final/finalproject.v(71[8:19])
    input reset;   // z:/windows.documents/desktop/ece272final/finalproject.v(71[26:31])
    output a_o;   // z:/windows.documents/desktop/ece272final/finalproject.v(72[27:30])
    output b_o;   // z:/windows.documents/desktop/ece272final/finalproject.v(72[32:35])
    output c_o;   // z:/windows.documents/desktop/ece272final/finalproject.v(72[37:40])
    output d_o;   // z:/windows.documents/desktop/ece272final/finalproject.v(72[42:45])
    output e_o;   // z:/windows.documents/desktop/ece272final/finalproject.v(72[47:50])
    output f_o;   // z:/windows.documents/desktop/ece272final/finalproject.v(72[52:55])
    output g_o;   // z:/windows.documents/desktop/ece272final/finalproject.v(72[57:60])
    output [2:0]display;   // z:/windows.documents/desktop/ece272final/finalproject.v(73[33:40])
    
    wire GND_net;   // z:/windows.documents/desktop/ece272final/finalproject.v(72[32:35])
    wire reset_c;   // z:/windows.documents/desktop/ece272final/finalproject.v(71[26:31])
    wire g_o_c;   // z:/windows.documents/desktop/ece272final/finalproject.v(72[27:30])
    wire display_c;   // z:/windows.documents/desktop/ece272final/finalproject.v(72[37:40])
    wire e_o_c;   // z:/windows.documents/desktop/ece272final/finalproject.v(72[47:50])
    wire [2:0]display_c1;   // z:/windows.documents/desktop/ece272final/finalproject.v(73[33:40])
    wire osc;   // z:/windows.documents/desktop/ece272final/finalproject.v(78[8:11])
    wire VCC_net;   // z:/windows.documents/desktop/ece272final/finalproject.v(72[52:55])
    
    wire n52;
    
    VHI i5 (.Z(VCC_net));
    OSCH osc_int (.STDBY(GND_net), .OSC(osc)) /* synthesis syn_instantiated=1 */ ;
    defparam osc_int.NOM_FREQ = "2.08";
    OB a_o_pad (.I(g_o_c), .O(a_o));
    GSR GSR_INST (.GSR(n52));
    OB b_o_pad (.I(GND_net), .O(b_o));
    VLO i1 (.Z(GND_net));
    sevenseg seven (.osc(osc), .GND_net(GND_net), .display_c(display_c), 
            .n45(display_c1[1]), .n44(display_c1[2]), .e_o_c(e_o_c), .g_o_c(g_o_c)) /* synthesis syn_module_defined=1 */ ;
    PUR PUR_INST (.PUR(VCC_net));
    defparam PUR_INST.RST_PULSE = 1;
    LUT4 i13_1_lut (.A(reset_c), .Z(n52)) /* synthesis lut_function=(!(A)) */ ;
    defparam i13_1_lut.init = 16'h5555;
    OB c_o_pad (.I(display_c), .O(c_o));
    OB d_o_pad (.I(g_o_c), .O(d_o));
    OB e_o_pad (.I(e_o_c), .O(e_o));
    OB f_o_pad (.I(VCC_net), .O(f_o));
    OB g_o_pad (.I(g_o_c), .O(g_o));
    OB display_pad_2 (.I(display_c1[2]), .O(display[2]));
    OB display_pad_1 (.I(display_c1[1]), .O(display[1]));
    OB display_pad_0 (.I(display_c), .O(display[0]));
    IB reset_pad (.I(reset), .O(reset_c));
    
endmodule
//
// Verilog Description of module sevenseg
//

module sevenseg (osc, GND_net, display_c, n45, n44, e_o_c, g_o_c) /* synthesis syn_module_defined=1 */ ;
    input osc;
    input GND_net;
    output display_c;
    output n45;
    output n44;
    output e_o_c;
    output g_o_c;
    
    wire [14:0]count;   // z:/windows.documents/desktop/ece272final/finalproject.v(107[13:18])
    wire [14:0]n115;
    wire [14:0]n66;
    
    wire n59, n65, n64, n63, n62, n61, n60;
    
    FD1S3AX count_5__i0 (.D(n66[0]), .CK(osc), .Q(n115[0]));
    defparam count_5__i0.GSR = "ENABLED";
    CCU2D count_5_add_4_1 (.A0(GND_net), .B0(GND_net), .C0(GND_net), .D0(GND_net), 
          .A1(n115[0]), .B1(GND_net), .C1(GND_net), .D1(GND_net), .COUT(n59), 
          .S1(n66[0]));
    defparam count_5_add_4_1.INIT0 = 16'hF000;
    defparam count_5_add_4_1.INIT1 = 16'h0555;
    defparam count_5_add_4_1.INJECT1_0 = "NO";
    defparam count_5_add_4_1.INJECT1_1 = "NO";
    LUT4 i11_2_lut (.A(count[13]), .B(count[14]), .Z(display_c)) /* synthesis lut_function=(!(A (B)+!A !(B))) */ ;
    defparam i11_2_lut.init = 16'h6666;
    FD1S3AX count_5__i14 (.D(n66[14]), .CK(osc), .Q(count[14]));
    defparam count_5__i14.GSR = "ENABLED";
    LUT4 i15_2_lut (.A(count[13]), .B(count[14]), .Z(n45)) /* synthesis lut_function=(!(A+!(B))) */ ;
    defparam i15_2_lut.init = 16'h4444;
    LUT4 i14_2_lut (.A(count[13]), .B(count[14]), .Z(n44)) /* synthesis lut_function=(A (B)) */ ;
    defparam i14_2_lut.init = 16'h8888;
    LUT4 i20_2_lut (.A(count[13]), .B(count[14]), .Z(e_o_c)) /* synthesis lut_function=(A (B)+!A !(B)) */ ;
    defparam i20_2_lut.init = 16'h9999;
    CCU2D count_5_add_4_15 (.A0(count[13]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[14]), .B1(GND_net), .C1(GND_net), 
          .D1(GND_net), .CIN(n65), .S0(n66[13]), .S1(n66[14]));
    defparam count_5_add_4_15.INIT0 = 16'hfaaa;
    defparam count_5_add_4_15.INIT1 = 16'hfaaa;
    defparam count_5_add_4_15.INJECT1_0 = "NO";
    defparam count_5_add_4_15.INJECT1_1 = "NO";
    CCU2D count_5_add_4_13 (.A0(n115[11]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(n115[12]), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .CIN(n64), .COUT(n65), .S0(n66[11]), .S1(n66[12]));
    defparam count_5_add_4_13.INIT0 = 16'hfaaa;
    defparam count_5_add_4_13.INIT1 = 16'hfaaa;
    defparam count_5_add_4_13.INJECT1_0 = "NO";
    defparam count_5_add_4_13.INJECT1_1 = "NO";
    CCU2D count_5_add_4_11 (.A0(n115[9]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(n115[10]), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .CIN(n63), .COUT(n64), .S0(n66[9]), .S1(n66[10]));
    defparam count_5_add_4_11.INIT0 = 16'hfaaa;
    defparam count_5_add_4_11.INIT1 = 16'hfaaa;
    defparam count_5_add_4_11.INJECT1_0 = "NO";
    defparam count_5_add_4_11.INJECT1_1 = "NO";
    CCU2D count_5_add_4_9 (.A0(n115[7]), .B0(GND_net), .C0(GND_net), .D0(GND_net), 
          .A1(n115[8]), .B1(GND_net), .C1(GND_net), .D1(GND_net), .CIN(n62), 
          .COUT(n63), .S0(n66[7]), .S1(n66[8]));
    defparam count_5_add_4_9.INIT0 = 16'hfaaa;
    defparam count_5_add_4_9.INIT1 = 16'hfaaa;
    defparam count_5_add_4_9.INJECT1_0 = "NO";
    defparam count_5_add_4_9.INJECT1_1 = "NO";
    CCU2D count_5_add_4_7 (.A0(n115[5]), .B0(GND_net), .C0(GND_net), .D0(GND_net), 
          .A1(n115[6]), .B1(GND_net), .C1(GND_net), .D1(GND_net), .CIN(n61), 
          .COUT(n62), .S0(n66[5]), .S1(n66[6]));
    defparam count_5_add_4_7.INIT0 = 16'hfaaa;
    defparam count_5_add_4_7.INIT1 = 16'hfaaa;
    defparam count_5_add_4_7.INJECT1_0 = "NO";
    defparam count_5_add_4_7.INJECT1_1 = "NO";
    CCU2D count_5_add_4_5 (.A0(n115[3]), .B0(GND_net), .C0(GND_net), .D0(GND_net), 
          .A1(n115[4]), .B1(GND_net), .C1(GND_net), .D1(GND_net), .CIN(n60), 
          .COUT(n61), .S0(n66[3]), .S1(n66[4]));
    defparam count_5_add_4_5.INIT0 = 16'hfaaa;
    defparam count_5_add_4_5.INIT1 = 16'hfaaa;
    defparam count_5_add_4_5.INJECT1_0 = "NO";
    defparam count_5_add_4_5.INJECT1_1 = "NO";
    CCU2D count_5_add_4_3 (.A0(n115[1]), .B0(GND_net), .C0(GND_net), .D0(GND_net), 
          .A1(n115[2]), .B1(GND_net), .C1(GND_net), .D1(GND_net), .CIN(n59), 
          .COUT(n60), .S0(n66[1]), .S1(n66[2]));
    defparam count_5_add_4_3.INIT0 = 16'hfaaa;
    defparam count_5_add_4_3.INIT1 = 16'hfaaa;
    defparam count_5_add_4_3.INJECT1_0 = "NO";
    defparam count_5_add_4_3.INJECT1_1 = "NO";
    LUT4 i25_2_lut (.A(count[13]), .B(count[14]), .Z(g_o_c)) /* synthesis lut_function=(!(A+(B))) */ ;
    defparam i25_2_lut.init = 16'h1111;
    FD1S3AX count_5__i1 (.D(n66[1]), .CK(osc), .Q(n115[1]));
    defparam count_5__i1.GSR = "ENABLED";
    FD1S3AX count_5__i13 (.D(n66[13]), .CK(osc), .Q(count[13]));
    defparam count_5__i13.GSR = "ENABLED";
    FD1S3AX count_5__i12 (.D(n66[12]), .CK(osc), .Q(n115[12]));
    defparam count_5__i12.GSR = "ENABLED";
    FD1S3AX count_5__i11 (.D(n66[11]), .CK(osc), .Q(n115[11]));
    defparam count_5__i11.GSR = "ENABLED";
    FD1S3AX count_5__i10 (.D(n66[10]), .CK(osc), .Q(n115[10]));
    defparam count_5__i10.GSR = "ENABLED";
    FD1S3AX count_5__i9 (.D(n66[9]), .CK(osc), .Q(n115[9]));
    defparam count_5__i9.GSR = "ENABLED";
    FD1S3AX count_5__i8 (.D(n66[8]), .CK(osc), .Q(n115[8]));
    defparam count_5__i8.GSR = "ENABLED";
    FD1S3AX count_5__i7 (.D(n66[7]), .CK(osc), .Q(n115[7]));
    defparam count_5__i7.GSR = "ENABLED";
    FD1S3AX count_5__i6 (.D(n66[6]), .CK(osc), .Q(n115[6]));
    defparam count_5__i6.GSR = "ENABLED";
    FD1S3AX count_5__i5 (.D(n66[5]), .CK(osc), .Q(n115[5]));
    defparam count_5__i5.GSR = "ENABLED";
    FD1S3AX count_5__i4 (.D(n66[4]), .CK(osc), .Q(n115[4]));
    defparam count_5__i4.GSR = "ENABLED";
    FD1S3AX count_5__i3 (.D(n66[3]), .CK(osc), .Q(n115[3]));
    defparam count_5__i3.GSR = "ENABLED";
    FD1S3AX count_5__i2 (.D(n66[2]), .CK(osc), .Q(n115[2]));
    defparam count_5__i2.GSR = "ENABLED";
    
endmodule
//
// Verilog Description of module PUR
// module not written out since it is a black-box. 
//

