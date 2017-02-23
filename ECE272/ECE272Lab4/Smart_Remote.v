//ECE272 Lab 4 Smart Remote Verilog Source
//Jing Guang Sia

module SmartRemote(
	input A,
	input B,
	input C,
	input ANLG_Ldir,
	input ANLG_Rdir,
	output LE_o,
	output LD_o,
	output RE_o,
	output RD_o

);
	supply0 GND;  //sets GND as ground
	wire LEN, LDIR, REN, RDIR, ANLG_L, ANLG_R, SEL, A, B, C; //wires for Smart Remote 

	assign SEL = ANLG_Ldir & ANLG_Rdir;

		RemoteControllerSchematic myremote(
      .A(A),
	  .B(B),
	  .C(C),
	  .Le(LEN),
	  .Ld(LDIR),
	  .Re(REN),
	  .Rd(RDIR)
	);

	
	mux2 LE_MUX( //left enable mux
		   .d0(GND),
		   .d1(LEN),
		   .s(SEL),
		   .Q(LE_o)
		);
		
	mux2 LD_MUX(  //left direction mux
		   .d0(ANLG_Ldir),
		   .d1(LDIR),
		   .s(SEL),
		   .Q(LD_o)
	);
	mux2 RE_MUX( // right enable mux
			.d0(GND),
			.d1(REN),
			.s(SEL),
			.Q(RE_o)
	);
	mux2 RD_MUX( //right direction mux
			.d0(ANLG_Rdir),
			.d1(RDIR),
			.s(SEL),
			.Q(RD_o)
	);
endmodule

module mux2( input d0, d1,  //generic mux implementation
			 input s,
			 output Q
	        );
		assign Q = s ? d1 : d0;			

endmodule