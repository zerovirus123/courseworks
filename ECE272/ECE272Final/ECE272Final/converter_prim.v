// Verilog netlist produced by program LSE :  version Diamond (64-bit) 2.2.0.101
// Netlist written on Wed May 28 09:42:41 2014
//
// Verilog Description of module converter
//

module converter (CS_n, RD_n, WR_n, led);   // z:/windows.documents/desktop/ece272final/finalproject.v(62[8:17])
    input CS_n;   // z:/windows.documents/desktop/ece272final/finalproject.v(62[24:28])
    input RD_n;   // z:/windows.documents/desktop/ece272final/finalproject.v(63[12:16])
    input WR_n;   // z:/windows.documents/desktop/ece272final/finalproject.v(64[12:16])
    output [7:0]led;   // z:/windows.documents/desktop/ece272final/finalproject.v(65[19:22])
    
    
    wire GND_net, VCC_net;
    
    VHI i14 (.Z(VCC_net));
    OBZ led_pad_7 (.I(GND_net), .T(VCC_net), .O(led[7]));
    OBZ led_pad_6 (.I(GND_net), .T(VCC_net), .O(led[6]));
    OBZ led_pad_5 (.I(GND_net), .T(VCC_net), .O(led[5]));
    OBZ led_pad_4 (.I(GND_net), .T(VCC_net), .O(led[4]));
    OBZ led_pad_3 (.I(GND_net), .T(VCC_net), .O(led[3]));
    OBZ led_pad_2 (.I(GND_net), .T(VCC_net), .O(led[2]));
    PUR PUR_INST (.PUR(VCC_net));
    defparam PUR_INST.RST_PULSE = 1;
    OBZ led_pad_1 (.I(GND_net), .T(VCC_net), .O(led[1]));
    VLO i4 (.Z(GND_net));
    OBZ led_pad_0 (.I(GND_net), .T(VCC_net), .O(led[0]));
    GSR GSR_INST (.GSR(VCC_net));
    
endmodule
//
// Verilog Description of module PUR
// module not written out since it is a black-box. 
//

