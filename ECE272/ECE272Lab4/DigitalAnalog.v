module DigitalAnalog( input clk,
					  input reset,
					  output reg [2:0] turn
					);

			wire R_push, L_push, osc, clock2;

		//oscillator for clock signal
		OSCH #("2.08") osc_int (					
			.STDBY(1'b0),							
			.OSC(osc),							
			.SEDSTDBY());	
			
		//added clock counter to lower oscillator frequency
		  clock_counter slow( 
		          .clk(clk),
		          .reset(reset),
				  .clk_o(clock2);
							);
		  end	
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
				always @ (posedge clk, negedge reset)
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
						 s0: if(~R_push)
							 state_n = s1;
						 else if(~L_push)
							 state_n = s3;
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
						 
						 
				 //output encoding
				 always @ (*)
					 begin
					   if(state == s0)
						   turn = 3'b000;
					   else if(state == s1)
						   turn = 3'b001;
					   else if(state == s2)
						   turn = 3'b010;
					   else if(state == s3)
						   turn = 3'b011;
					   else if (state == s4)
						   turn = 3'b100;
					 end
						
endmodule