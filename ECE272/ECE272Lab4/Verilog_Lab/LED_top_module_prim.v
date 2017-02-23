// Verilog netlist produced by program LSE :  version Diamond (64-bit) 2.2.0.101
// Netlist written on Wed May 07 13:54:45 2014
//
// Verilog Description of module LED_top_module
//

module LED_top_module (reset_n, LEDs) /* synthesis syn_module_defined=1 */ ;   // z:/windows.documents/desktop/ece272lab4/../led state machine/source files/led_fsm_top.v(1[8:22])
    input reset_n;   // z:/windows.documents/desktop/ece272lab4/../led state machine/source files/led_fsm_top.v(2[8:15])
    output [7:0]LEDs;   // z:/windows.documents/desktop/ece272lab4/../led state machine/source files/led_fsm_top.v(4[15:19])
    
    wire reset_n_c;   // z:/windows.documents/desktop/ece272lab4/../led state machine/source files/led_fsm_top.v(2[8:15])
    wire [7:0]LEDs_c;   // z:/windows.documents/desktop/ece272lab4/../led state machine/source files/led_fsm_top.v(4[15:19])
    wire clk;   // z:/windows.documents/desktop/ece272lab4/../led state machine/source files/led_fsm_top.v(7[8:11])
    wire clk_5;   // z:/windows.documents/desktop/ece272lab4/../led state machine/source files/led_fsm_top.v(8[8:13])
    
    wire GND_net, VCC_net;
    
    VHI i112 (.Z(VCC_net));
    VLO i1 (.Z(GND_net));
    OSCH osc_int (.STDBY(GND_net), .OSC(clk)) /* synthesis syn_instantiated=1 */ ;
    defparam osc_int.NOM_FREQ = "2.03";
    OB LEDs_pad_6 (.I(LEDs_c[6]), .O(LEDs[6]));
    OB LEDs_pad_7 (.I(LEDs_c[3]), .O(LEDs[7]));
    PUR PUR_INST (.PUR(VCC_net));
    defparam PUR_INST.RST_PULSE = 1;
    state_machine FSM_1 (.n94(LEDs_c[3]), .clk_5(clk_5), .n91(LEDs_c[6]), 
            .n96(LEDs_c[1]), .n93(LEDs_c[4])) /* synthesis syn_module_defined=1 */ ;
    clock_counter counter_1 (.clk(clk), .clk_5(clk_5), .GND_net(GND_net)) /* synthesis syn_module_defined=1 */ ;
    GSR GSR_INST (.GSR(reset_n_c));
    OB LEDs_pad_5 (.I(LEDs_c[1]), .O(LEDs[5]));
    OB LEDs_pad_4 (.I(LEDs_c[4]), .O(LEDs[4]));
    OB LEDs_pad_3 (.I(LEDs_c[3]), .O(LEDs[3]));
    OB LEDs_pad_2 (.I(LEDs_c[6]), .O(LEDs[2]));
    OB LEDs_pad_1 (.I(LEDs_c[1]), .O(LEDs[1]));
    OB LEDs_pad_0 (.I(LEDs_c[4]), .O(LEDs[0]));
    IB reset_n_pad (.I(reset_n), .O(reset_n_c));
    
endmodule
//
// Verilog Description of module PUR
// module not written out since it is a black-box. 
//

//
// Verilog Description of module state_machine
//

module state_machine (n94, clk_5, n91, n96, n93) /* synthesis syn_module_defined=1 */ ;
    output n94;
    input clk_5;
    output n91;
    output n96;
    output n93;
    
    wire [3:0]n22;
    
    FD1S3AY state_FSM_i0 (.D(n22[3]), .CK(clk_5), .Q(n94));
    defparam state_FSM_i0.GSR = "ENABLED";
    LUT4 i85_2_lut (.A(n22[1]), .B(n94), .Z(n91)) /* synthesis lut_function=(!((B)+!A)) */ ;
    defparam i85_2_lut.init = 16'h2222;
    FD1S3AX state_FSM_i3 (.D(n22[2]), .CK(clk_5), .Q(n22[3]));
    defparam state_FSM_i3.GSR = "ENABLED";
    FD1S3AX state_FSM_i2 (.D(n22[1]), .CK(clk_5), .Q(n22[2]));
    defparam state_FSM_i2.GSR = "ENABLED";
    FD1S3AX state_FSM_i1 (.D(n94), .CK(clk_5), .Q(n22[1]));
    defparam state_FSM_i1.GSR = "ENABLED";
    LUT4 i82_2_lut_3_lut (.A(n22[1]), .B(n94), .C(n22[2]), .Z(n96)) /* synthesis lut_function=(!(A+(B+!(C)))) */ ;
    defparam i82_2_lut_3_lut.init = 16'h1010;
    LUT4 i97_2_lut_3_lut (.A(n22[1]), .B(n94), .C(n22[2]), .Z(n93)) /* synthesis lut_function=(!(A+(B+(C)))) */ ;
    defparam i97_2_lut_3_lut.init = 16'h0101;
    
endmodule
//
// Verilog Description of module clock_counter
//

module clock_counter (clk, clk_5, GND_net) /* synthesis syn_module_defined=1 */ ;
    input clk;
    output clk_5;
    input GND_net;
    
    wire [18:0]count;   // z:/windows.documents/desktop/ece272lab4/../led state machine/source files/clock_counter.v(8[14:19])
    
    wire n27;
    wire [18:0]n82;
    
    wire n29, n117, n14, n130, n4, n13, n6, n5, n125, n124, 
        n123, n122, n121, n120, n119, n118;
    
    FD1S3IX dff_11_34__i0 (.D(n82[0]), .CK(clk), .CD(n27), .Q(count[0]));
    defparam dff_11_34__i0.GSR = "ENABLED";
    FD1S3AX clk_o_13 (.D(n29), .CK(clk), .Q(clk_5));
    defparam clk_o_13.GSR = "ENABLED";
    CCU2D dff_11_34_add_4_1 (.A0(GND_net), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[0]), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .COUT(n117), .S1(n82[0]));
    defparam dff_11_34_add_4_1.INIT0 = 16'hF000;
    defparam dff_11_34_add_4_1.INIT1 = 16'h0555;
    defparam dff_11_34_add_4_1.INJECT1_0 = "NO";
    defparam dff_11_34_add_4_1.INJECT1_1 = "NO";
    LUT4 i15_2_lut (.A(clk_5), .B(n27), .Z(n29)) /* synthesis lut_function=(!(A (B)+!A !(B))) */ ;
    defparam i15_2_lut.init = 16'h6666;
    LUT4 i6_4_lut (.A(count[2]), .B(count[5]), .C(count[3]), .D(count[1]), 
         .Z(n14)) /* synthesis lut_function=(A (B (C (D)))) */ ;
    defparam i6_4_lut.init = 16'h8000;
    LUT4 i1_4_lut (.A(count[15]), .B(n130), .C(count[14]), .D(count[13]), 
         .Z(n4)) /* synthesis lut_function=(A+(B (C)+!B (C (D)))) */ ;
    defparam i1_4_lut.init = 16'hfaea;
    LUT4 i5_4_lut (.A(count[0]), .B(count[6]), .C(count[4]), .D(count[7]), 
         .Z(n13)) /* synthesis lut_function=(A (B (C (D)))) */ ;
    defparam i5_4_lut.init = 16'h8000;
    LUT4 i2_2_lut (.A(count[10]), .B(count[8]), .Z(n6)) /* synthesis lut_function=(A+(B)) */ ;
    defparam i2_2_lut.init = 16'heeee;
    LUT4 i1_3_lut (.A(n13), .B(count[9]), .C(n14), .Z(n5)) /* synthesis lut_function=(A (B+(C))+!A (B)) */ ;
    defparam i1_3_lut.init = 16'hecec;
    LUT4 i2_4_lut (.A(count[12]), .B(count[11]), .C(n5), .D(n6), .Z(n130)) /* synthesis lut_function=(A (B (C+(D)))) */ ;
    defparam i2_4_lut.init = 16'h8880;
    CCU2D dff_11_34_add_4_19 (.A0(count[17]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[18]), .B1(GND_net), .C1(GND_net), 
          .D1(GND_net), .CIN(n125), .S0(n82[17]), .S1(n82[18]));
    defparam dff_11_34_add_4_19.INIT0 = 16'hfaaa;
    defparam dff_11_34_add_4_19.INIT1 = 16'hfaaa;
    defparam dff_11_34_add_4_19.INJECT1_0 = "NO";
    defparam dff_11_34_add_4_19.INJECT1_1 = "NO";
    CCU2D dff_11_34_add_4_17 (.A0(count[15]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[16]), .B1(GND_net), .C1(GND_net), 
          .D1(GND_net), .CIN(n124), .COUT(n125), .S0(n82[15]), .S1(n82[16]));
    defparam dff_11_34_add_4_17.INIT0 = 16'hfaaa;
    defparam dff_11_34_add_4_17.INIT1 = 16'hfaaa;
    defparam dff_11_34_add_4_17.INJECT1_0 = "NO";
    defparam dff_11_34_add_4_17.INJECT1_1 = "NO";
    CCU2D dff_11_34_add_4_15 (.A0(count[13]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[14]), .B1(GND_net), .C1(GND_net), 
          .D1(GND_net), .CIN(n123), .COUT(n124), .S0(n82[13]), .S1(n82[14]));
    defparam dff_11_34_add_4_15.INIT0 = 16'hfaaa;
    defparam dff_11_34_add_4_15.INIT1 = 16'hfaaa;
    defparam dff_11_34_add_4_15.INJECT1_0 = "NO";
    defparam dff_11_34_add_4_15.INJECT1_1 = "NO";
    CCU2D dff_11_34_add_4_13 (.A0(count[11]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[12]), .B1(GND_net), .C1(GND_net), 
          .D1(GND_net), .CIN(n122), .COUT(n123), .S0(n82[11]), .S1(n82[12]));
    defparam dff_11_34_add_4_13.INIT0 = 16'hfaaa;
    defparam dff_11_34_add_4_13.INIT1 = 16'hfaaa;
    defparam dff_11_34_add_4_13.INJECT1_0 = "NO";
    defparam dff_11_34_add_4_13.INJECT1_1 = "NO";
    CCU2D dff_11_34_add_4_11 (.A0(count[9]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[10]), .B1(GND_net), .C1(GND_net), 
          .D1(GND_net), .CIN(n121), .COUT(n122), .S0(n82[9]), .S1(n82[10]));
    defparam dff_11_34_add_4_11.INIT0 = 16'hfaaa;
    defparam dff_11_34_add_4_11.INIT1 = 16'hfaaa;
    defparam dff_11_34_add_4_11.INJECT1_0 = "NO";
    defparam dff_11_34_add_4_11.INJECT1_1 = "NO";
    CCU2D dff_11_34_add_4_9 (.A0(count[7]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[8]), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .CIN(n120), .COUT(n121), .S0(n82[7]), .S1(n82[8]));
    defparam dff_11_34_add_4_9.INIT0 = 16'hfaaa;
    defparam dff_11_34_add_4_9.INIT1 = 16'hfaaa;
    defparam dff_11_34_add_4_9.INJECT1_0 = "NO";
    defparam dff_11_34_add_4_9.INJECT1_1 = "NO";
    CCU2D dff_11_34_add_4_7 (.A0(count[5]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[6]), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .CIN(n119), .COUT(n120), .S0(n82[5]), .S1(n82[6]));
    defparam dff_11_34_add_4_7.INIT0 = 16'hfaaa;
    defparam dff_11_34_add_4_7.INIT1 = 16'hfaaa;
    defparam dff_11_34_add_4_7.INJECT1_0 = "NO";
    defparam dff_11_34_add_4_7.INJECT1_1 = "NO";
    CCU2D dff_11_34_add_4_5 (.A0(count[3]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[4]), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .CIN(n118), .COUT(n119), .S0(n82[3]), .S1(n82[4]));
    defparam dff_11_34_add_4_5.INIT0 = 16'hfaaa;
    defparam dff_11_34_add_4_5.INIT1 = 16'hfaaa;
    defparam dff_11_34_add_4_5.INJECT1_0 = "NO";
    defparam dff_11_34_add_4_5.INJECT1_1 = "NO";
    CCU2D dff_11_34_add_4_3 (.A0(count[1]), .B0(GND_net), .C0(GND_net), 
          .D0(GND_net), .A1(count[2]), .B1(GND_net), .C1(GND_net), .D1(GND_net), 
          .CIN(n117), .COUT(n118), .S0(n82[1]), .S1(n82[2]));
    defparam dff_11_34_add_4_3.INIT0 = 16'hfaaa;
    defparam dff_11_34_add_4_3.INIT1 = 16'hfaaa;
    defparam dff_11_34_add_4_3.INJECT1_0 = "NO";
    defparam dff_11_34_add_4_3.INJECT1_1 = "NO";
    FD1S3IX dff_11_34__i18 (.D(n82[18]), .CK(clk), .CD(n27), .Q(count[18]));
    defparam dff_11_34__i18.GSR = "ENABLED";
    LUT4 i2_4_lut_adj_1 (.A(count[18]), .B(count[17]), .C(count[16]), 
         .D(n4), .Z(n27)) /* synthesis lut_function=(A (B (C+(D)))) */ ;
    defparam i2_4_lut_adj_1.init = 16'h8880;
    FD1S3IX dff_11_34__i17 (.D(n82[17]), .CK(clk), .CD(n27), .Q(count[17]));
    defparam dff_11_34__i17.GSR = "ENABLED";
    FD1S3IX dff_11_34__i16 (.D(n82[16]), .CK(clk), .CD(n27), .Q(count[16]));
    defparam dff_11_34__i16.GSR = "ENABLED";
    FD1S3IX dff_11_34__i15 (.D(n82[15]), .CK(clk), .CD(n27), .Q(count[15]));
    defparam dff_11_34__i15.GSR = "ENABLED";
    FD1S3IX dff_11_34__i14 (.D(n82[14]), .CK(clk), .CD(n27), .Q(count[14]));
    defparam dff_11_34__i14.GSR = "ENABLED";
    FD1S3IX dff_11_34__i13 (.D(n82[13]), .CK(clk), .CD(n27), .Q(count[13]));
    defparam dff_11_34__i13.GSR = "ENABLED";
    FD1S3IX dff_11_34__i12 (.D(n82[12]), .CK(clk), .CD(n27), .Q(count[12]));
    defparam dff_11_34__i12.GSR = "ENABLED";
    FD1S3IX dff_11_34__i11 (.D(n82[11]), .CK(clk), .CD(n27), .Q(count[11]));
    defparam dff_11_34__i11.GSR = "ENABLED";
    FD1S3IX dff_11_34__i10 (.D(n82[10]), .CK(clk), .CD(n27), .Q(count[10]));
    defparam dff_11_34__i10.GSR = "ENABLED";
    FD1S3IX dff_11_34__i9 (.D(n82[9]), .CK(clk), .CD(n27), .Q(count[9]));
    defparam dff_11_34__i9.GSR = "ENABLED";
    FD1S3IX dff_11_34__i8 (.D(n82[8]), .CK(clk), .CD(n27), .Q(count[8]));
    defparam dff_11_34__i8.GSR = "ENABLED";
    FD1S3IX dff_11_34__i7 (.D(n82[7]), .CK(clk), .CD(n27), .Q(count[7]));
    defparam dff_11_34__i7.GSR = "ENABLED";
    FD1S3IX dff_11_34__i6 (.D(n82[6]), .CK(clk), .CD(n27), .Q(count[6]));
    defparam dff_11_34__i6.GSR = "ENABLED";
    FD1S3IX dff_11_34__i5 (.D(n82[5]), .CK(clk), .CD(n27), .Q(count[5]));
    defparam dff_11_34__i5.GSR = "ENABLED";
    FD1S3IX dff_11_34__i4 (.D(n82[4]), .CK(clk), .CD(n27), .Q(count[4]));
    defparam dff_11_34__i4.GSR = "ENABLED";
    FD1S3IX dff_11_34__i3 (.D(n82[3]), .CK(clk), .CD(n27), .Q(count[3]));
    defparam dff_11_34__i3.GSR = "ENABLED";
    FD1S3IX dff_11_34__i2 (.D(n82[2]), .CK(clk), .CD(n27), .Q(count[2]));
    defparam dff_11_34__i2.GSR = "ENABLED";
    FD1S3IX dff_11_34__i1 (.D(n82[1]), .CK(clk), .CD(n27), .Q(count[1]));
    defparam dff_11_34__i1.GSR = "ENABLED";
    
endmodule
