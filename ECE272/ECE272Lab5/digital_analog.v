//Digital Version of the Analog Board
module DigitalAnalog(
					  input reset,
					  input R_bump, L_bump,
					  output Le, Ldir, Re, Rdir
					);

			wire osc, clock2, sel;
			supply0 GND;

		//oscillator for clock signal
		OSCH #("2.08") osc_int (					
			.STDBY(1'b0),							
			.OSC(osc),							
			.SEDSTDBY());	
			
		//added clock counter to lower oscillator frequency
		  clock_counter slow( 
		          .clk(osc),
		          .reset(reset),
				  .clk_o(clock2)
					);
							
					//state and next state reg
					reg [2:0] state;
					reg [2:0] state_n;
					
					//state encodings
					parameter s0 = 3'b000;
					parameter s1 = 3'b001;
					parameter s2 = 3'b010;
					parameter s3 = 3'b011;
					parameter s4 = 3'b100;
					
				//reset logic
				always @ (posedge clock2, negedge reset)
					begin
						if(!reset)
							state = s0;
						else
							state = state_n;
					end
					
				//FSM logic
				always @ (*)
					begin
					 case(state)
						 s0:	
								if(!R_bump)
									state_n = s1;
								else if(!L_bump)
									state_n = s3;
								else
								   begin
									state_n = s0;
								   end	
						 s1: 
							 state_n = s2;
					     s3:
							 state_n = s4;
					     s4:
							 state_n = s0;
					     s2:
							 state_n = s0;
					     default: state_n = s0;
					  endcase
					end
						 	 
				
				 //output  Le-Re-Ldir-Rdir
				 assign Le = 0;
				 assign Re = 0;
				 assign Ldir = (state == s0 || state == s4);
				 assign Rdir = (state == s0 || state == s2);
				 				
endmodule

//clock counter to slow down the oscillator
module clock_counter(
	input clk,		//often, "tags" are added to variables to denote what they do for the user
	input reset,		//here, 'i' is used for input and 'o' for the output, while 'n' specifies an active low signal ("not") 
	output reg clk_o
		);		
		reg [20:0] count;								//register stores the counter value so that it can be modified on a clock edge. register size needs to store as large of a number as the counter reaches
														//for this implementation, count must reach 415999, so 2^n >= 415999, n = 19
		always @ (posedge clk, negedge reset)			
			begin
				count <= count + 1;						//at every positive edge, the counter is increased by 1
				if(!reset)
					begin
						clk_o <= 0;
						count <= 0;						//if reset (active low) is pushed, the counter is reset
					end
				else
					if(count >=  1039999)					//count value of greater than or equal to this value causes the output clock to be inverted. the resulting frequency will be input_frequency/(1+count_value)
						begin							//for this implementation, a frequency of 5 Hz was desired, so 2.08e6/5 - 1 = 415999
							clk_o <= ~clk_o;	
							count <= 0;					//resets the counter after the output clock has been inverted
						end
			end
endmodule

