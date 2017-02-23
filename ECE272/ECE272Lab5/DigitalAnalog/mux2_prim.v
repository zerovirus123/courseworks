// Verilog netlist produced by program LSE :  version Diamond (64-bit) 2.2.0.101
// Netlist written on Wed May 14 13:12:01 2014
//
// Verilog Description of module mux2
//

module mux2 (d0, d1, s, Q) /* synthesis syn_module_defined=1 */ ;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(113[8:12])
    input d0;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(113[20:22])
    input d1;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(113[24:26])
    input s;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(114[11:12])
    output Q;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(115[12:13])
    
    wire d0_c;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(113[20:22])
    wire d1_c;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(113[24:26])
    wire s_c;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(114[11:12])
    wire Q_c;   // z:/windows.documents/desktop/ece272lab5/digital_analog.v(115[12:13])
    
    wire VCC_net;
    
    VHI i18 (.Z(VCC_net));
    OB Q_pad (.I(Q_c), .O(Q));
    IB d0_pad (.I(d0), .O(d0_c));
    IB d1_pad (.I(d1), .O(d1_c));
    IB s_pad (.I(s), .O(s_c));
    GSR GSR_INST (.GSR(VCC_net));
    LUT4 i4_3_lut (.A(d0_c), .B(d1_c), .C(s_c), .Z(Q_c)) /* synthesis lut_function=(A (B+!(C))+!A (B (C))) */ ;
    defparam i4_3_lut.init = 16'hcaca;
    PUR PUR_INST (.PUR(VCC_net));
    defparam PUR_INST.RST_PULSE = 1;
    
endmodule
//
// Verilog Description of module PUR
// module not written out since it is a black-box. 
//

