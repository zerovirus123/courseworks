/*
module FinalProject( input reset_n,
			         input [7:0] in,
					 output CS_n,
					 output RD_n,
					 output WR_n,
					 output [7:0] led
					);
					
	 wire [7:0] connect;
	 		
	 supply0 GND;

	 assign led = in;	 
	 OSCH #("2.08") osc_int(
		.STDBY(1'b0),
		.OSC(osc),
		.SEDSTDBY());

					
	  ADCinterface ADC(
	            .reset_n(reset_n),
				.clk(osc),
				.CS_n(CS_n),
				.RD_n(RD_n),
				.WR_n(WR_n)
					 );
	 
	  		
endmodule


module ADCinterface(
		input reset_n,
		input clk,			//input clock should be 2.08MHz
		
		output CS_n,
		output RD_n,
		output reg WR_n
	);
	
	supply0 GND;
			
	reg [7:0] count;
	
	assign CS_n = GND;			//in standalone mode, CS tied to ground
	assign RD_n = GND;			//in standalone mode, RD tied to ground
		
	always @ (posedge clk, negedge reset_n)			
		begin
			count <= count + 1;								
			if(!reset_n)
				begin
					WR_n <= 0;		//WR_n level starts low
					count <= 0;						
				end
			else
				if(count == 20)					//in current setup, clock low for first 1/10 of period, ~10uS
						WR_n <= ~WR_n;	
				else if(count == 200)			//in current setup, clock period is (1/2.08E6)(200) = ~100uS
					begin
						WR_n <= ~WR_n;
						count <= 0;	
					end
		end
endmodule
*/


module checkpoint2(input reset,
                   output a_o, b_o, c_o, d_o, e_o, f_o, g_o,
                   output [2:0] display				   
				  );

	OSCH #("2.08") osc_int(
		.STDBY(1'b0),
		.OSC(osc),
		.SEDSTDBY());

    
	sevenseg seven(
	               .reset(reset),
				   .clock(osc),
				   .a_o(a_o),
				   .b_o(b_o),
				   .c_o(c_o),
				   .d_o(d_o),
				   .e_o(e_o),
				   .f_o(f_o),
				   .g_o(g_o),
				   .out(display)
			
				   );

endmodule



module sevenseg(
	               input reset,
				   input clock,
	               output a_o, b_o, c_o, d_o, e_o, f_o, g_o,		           output [2:0] out				  );

localparam N = 15;
 
reg [N-1:0] count;
reg [6:0] sseg_temp;
reg [2:0] sel;

always @ (posedge clock or posedge reset)
 begin
  if (reset)
   count <= 0;
  else
   count <= count + 1;
 end


always @ (*)
 begin
  case(count[N-1:N-2]) 
	  
  2'b00 :  
	begin   //ones digit
	 sel[0] = 0;
	 sel[1] = 0;
	 sel[2] = 0; 
     sseg_temp = 7'b1111001; 	 
    end
	
  2'b01:  
    begin //tens digit
	 sel[0] = 1;
	 sel[1] = 0;
	 sel[2] = 0;
	 sseg_temp = 7'b0100100;
    end
   
  2'b10:   
    begin //hundreds digit
	 sel[0] = 1;
	 sel[1] = 1;
	 sel[2] = 0;
	 sseg_temp = 7'b0100100; 
    end
	
  2'b11:   
    begin //thousands digit
	 sel[0] = 0;
	 sel[1] = 0;
	 sel[2] = 1;
	 sseg_temp = 7'b0110000;
    end
	
  endcase
 end
assign out = sel;
assign {g_o, f_o, e_o, d_o, c_o, b_o, a_o} = sseg_temp;

/*
always @ (*)
 begin
  case(in)       //g-a
   4'h0 : sseg_temp = 7'b1000000; 
   4'h1 : sseg_temp = 7'b1111001; 
   4'h2 : sseg_temp = 7'b0100100; 
   4'h3 : sseg_temp = 7'b0110000; 
   4'h4 : sseg_temp = 7'b0011001; 
   4'h5 : sseg_temp = 7'b0010010; 
   4'h6 : sseg_temp = 7'b0000010; 
   4'h7 : sseg_temp = 7'b1111000; 
   4'h8 : sseg_temp = 7'b0000000; 
   4'h9 : sseg_temp = 7'b0010000; 
   default : sseg_temp = 7'b1111111; 
  endcase
 end
assign {g_o, f_o, e_o, d_o, c_o, b_o, a_o} = sseg_temp; 
*/

endmodule