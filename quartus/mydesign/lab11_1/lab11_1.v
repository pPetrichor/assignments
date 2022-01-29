
//=======================================================
//  This code is generated by Terasic System Builder
//=======================================================

module lab11_1(

	//////////// CLOCK //////////
	input 		          		CLOCK2_50,
	input 		          		CLOCK3_50,
	input 		          		CLOCK4_50,
	input 		          		CLOCK_50,

	//////////// KEY //////////
	input 		     [3:0]		KEY,

	//////////// SW //////////
	input 		     [9:0]		SW,

	//////////// LED //////////
	output		     [9:0]		LEDR,

	//////////// VGA //////////
	output		          		VGA_BLANK_N,
	output		     [7:0]		VGA_B,
	output		          		VGA_CLK,
	output		     [7:0]		VGA_G,
	output		          		VGA_HS,
	output		     [7:0]		VGA_R,
	output		          		VGA_SYNC_N,
	output		          		VGA_VS,

	//////////// Audio //////////
	input 		          		AUD_ADCDAT,
	inout 		          		AUD_ADCLRCK,
	inout 		          		AUD_BCLK,
	output		          		AUD_DACDAT,
	inout 		          		AUD_DACLRCK,
	output		          		AUD_XCK,

	//////////// PS2 //////////
	inout 		          		PS2_CLK,
	inout 		          		PS2_CLK2,
	inout 		          		PS2_DAT,
	inout 		          		PS2_DAT2,

	//////////// I2C for Audio and Video-In //////////
	output		          		FPGA_I2C_SCLK,
	inout 		          		FPGA_I2C_SDAT
);



//=======================================================
//  REG/WIRE declarations
//=======================================================
assign VGA_SYNC_N = 1'b0;
wire [7:0] ASCII;
wire [23:0] vga_data;
wire [9:0] h_addr; //提供给上层模块的当前扫描像素点坐标 
wire [9:0] v_addr; 
wire [7:0]data;
wire ready,overflow,together,off,if_shift,if_caps;
wire [7:0] PS;
wire [3:0]state;
wire d_data;
wire [12:0]addr;
wire [11:0]q;

//=======================================================
//  Structural coding
//=======================================================
clkgen #(25000000) my_vgaclk(CLOCK_50,SW[0],1'b1,VGA_CLK);
vga_ctrl my_vga(VGA_CLK,SW[0],vga_data,h_addr,v_addr,VGA_HS,VGA_VS,VGA_BLANK_N,VGA_R,VGA_G,VGA_B);
ps2_keyboard PS_(CLOCK_50,1'b1,PS2_CLK,PS2_DAT,data,ready,overflow);
higher HIGH(CLOCK_50,ready,data,PS,together,if_caps,if_shift,off,state);
trans TRANS(CLOCK_50,off,PS,ASCII,if_shift,if_caps);
mytable my(VGA_BLANK_N,ASCII,CLOCK_50,VGA_CLK,h_addr,v_addr,vga_data,off,q,addr,ready);
reftable myref(CLOCK_50,0,addr,0,0,q);
assign LEDR[0]=off;
endmodule
