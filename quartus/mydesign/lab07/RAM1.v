module RAM1(dout,clk, we, inaddr, outaddr, din, dout_ones, dout_tens);
input clk, we;
input[3:0] inaddr; input[3:0] outaddr;
input[7:0] din;
output reg [6:0]dout_ones;
output reg [6:0]dout_tens;

output reg[7:0] dout;
reg [7:0] ram [15:0];
reg [3:0] dout_l;
reg [3:0] dout_h;

initial
begin
	$readmemh("D:/quartus/mydesign/lab07/mem1.txt", ram, 0, 15); 
end
always@ (posedge clk)
begin
	if(we)
	ram[inaddr] = din;
	else
	dout = ram[outaddr];
	dout_l = dout[3:0];
	dout_h = dout[7:4];
	case(dout_l)
	0:dout_ones = 7'b1000000;
	1:dout_ones = 7'b1111001;
	2:dout_ones = 7'b0100100;
	3:dout_ones = 7'b0110000;
	4:dout_ones = 7'b0011001;
	5:dout_ones = 7'b0010010;
	6:dout_ones = 7'b0000010;
	7:dout_ones = 7'b1111000;
	8:dout_ones = 7'b0000000;
	9:dout_ones = 7'b0010000;
	10:dout_ones = 7'b0001000;
	11:dout_ones = 7'b0000011;
	12:dout_ones = 7'b1000110;
	13:dout_ones = 7'b0100001;
	14:dout_ones = 7'b0000110;
	15:dout_ones = 7'b0001110;
	endcase
	case(dout_h)
	0:dout_tens = 7'b1000000;
	1:dout_tens = 7'b1111001;
	2:dout_tens = 7'b0100100;
	3:dout_tens = 7'b0110000;
	4:dout_tens = 7'b0011001;
	5:dout_tens = 7'b0010010;
	6:dout_tens = 7'b0000010;
	7:dout_tens = 7'b1111000;
	8:dout_tens = 7'b0000000;
	9:dout_tens = 7'b0010000;
	10:dout_tens = 7'b0001000;
	11:dout_tens = 7'b0000011;
	12:dout_tens = 7'b1000110;
	13:dout_tens = 7'b0100001;
	14:dout_tens = 7'b0000110;
	15:dout_tens = 7'b0001110;
	endcase
end
endmodule

