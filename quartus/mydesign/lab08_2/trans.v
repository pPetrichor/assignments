module trans(clk_50,off,PS,if_caps,if_shift,counter,counter_h,counter_l,PS2_l,PS2_h,ASCII_l,ASCII_h);

input clk_50,off,if_caps,if_shift;
input [7:0]PS;
input [9:0]counter;
output reg [6:0] counter_h;
output reg [6:0] counter_l;
output reg [6:0] PS2_l;
output reg [6:0] PS2_h;
output reg [6:0] ASCII_l;
output reg [6:0] ASCII_h;

reg [3:0] PS_L;reg [3:0] PS_H;
reg [3:0] counter_ones;reg [3:0] counter_tens;
reg [3:0] ASCII_L;reg [3:0] ASCII_H;

always @(posedge clk_50)
begin
	counter_ones = counter % 10;
	counter_tens = (counter - counter % 10) / 10;
	case(counter_ones)
		0:counter_l = 7'b1000000;
		1:counter_l = 7'b1111001;
		2:counter_l = 7'b0100100;
		3:counter_l = 7'b0110000;
		4:counter_l = 7'b0011001;
		5:counter_l = 7'b0010010;
		6:counter_l = 7'b0000010;
		7:counter_l = 7'b1111000;
		8:counter_l = 7'b0000000;
		9:counter_l = 7'b0010000;
	endcase
	case(counter_tens)
		0:counter_h = 7'b1000000;
		1:counter_h = 7'b1111001;
		2:counter_h = 7'b0100100;
		3:counter_h = 7'b0110000;
		4:counter_h = 7'b0011001;
		5:counter_h = 7'b0010010;
		6:counter_h = 7'b0000010;
		7:counter_h = 7'b1111000;
		8:counter_h = 7'b0000000;
		9:counter_h = 7'b0010000;
	endcase
	if(!off)
	begin
	PS_L = {PS[3],PS[2],PS[1],PS[0]};
	PS_H = {PS[7],PS[6],PS[5],PS[4]};
	case(PS_L)
		0:PS2_l = 7'b1000000;
		1:PS2_l = 7'b1111001;
		2:PS2_l = 7'b0100100;
		3:PS2_l = 7'b0110000;
		4:PS2_l = 7'b0011001;
		5:PS2_l = 7'b0010010;
		6:PS2_l = 7'b0000010;
		7:PS2_l = 7'b1111000;
		8:PS2_l = 7'b0000000;
		9:PS2_l = 7'b0010000;
		10:PS2_l = 7'b0001000;
		11:PS2_l = 7'b0000011;
		12:PS2_l = 7'b1000110;
		13:PS2_l = 7'b0100001;
		14:PS2_l = 7'b0000110;
		15:PS2_l = 7'b0001110;
	endcase
	case(PS_H)
		0:PS2_h = 7'b1000000;
		1:PS2_h = 7'b1111001;
		2:PS2_h = 7'b0100100;
		3:PS2_h = 7'b0110000;
		4:PS2_h = 7'b0011001;
		5:PS2_h = 7'b0010010;
		6:PS2_h = 7'b0000010;
		7:PS2_h = 7'b1111000;
		8:PS2_h = 7'b0000000;
		9:PS2_h = 7'b0010000;
		10:PS2_h = 7'b0001000;
		11:PS2_h = 7'b0000011;
		12:PS2_h = 7'b1000110;
		13:PS2_h = 7'b0100001;
		14:PS2_h = 7'b0000110;
		15:PS2_h = 7'b0001110;
	endcase
	case(PS)
		8'b00010101://Q
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b1111000;
				ASCII_l = 7'b1111001;
				end
				else
				begin
				ASCII_h = 7'b0010010;
				ASCII_l = 7'b1111001;
				end
			end
		8'b00011101://W
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b1111000;
				ASCII_l = 7'b1111000;
				end
				else
				begin
				ASCII_h = 7'b0010010;
				ASCII_l = 7'b1111000;
				end
			end
		8'b00100100://E
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b0000010;
				ASCII_l = 7'b0010010;
				end
				else
				begin
				ASCII_h = 7'b0011001;
				ASCII_l = 7'b0010010;
				end
			end
		8'b00101101://R
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b1111000;
				ASCII_l = 7'b0100100;
				end
				else
				begin
				ASCII_h = 7'b0010010;
				ASCII_l = 7'b0100100;
				end
			end
		8'b00101100://T
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b1111000;
				ASCII_l = 7'b0011001;
				end
				else
				begin
				ASCII_h = 7'b0010010;
				ASCII_l = 7'b0011001;
				end
			end
		8'b00110101://Y
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b1111000;
				ASCII_l = 7'b0010000;
				end
				else
				begin
				ASCII_h = 7'b0010010;
				ASCII_l = 7'b0010000;
				end
				
			end
		8'b00111100://U
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b1111000;
				ASCII_l = 7'b0010010;
				end
				else
				begin
				ASCII_h = 7'b0010010;
				ASCII_l = 7'b0010010;
				end
				
			end
		8'b01000011://I
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b0000010;
				ASCII_l = 7'b0010000;
				end
				else
				begin
				ASCII_h = 7'b0011001;
				ASCII_l = 7'b0010000;
				end
				
			end
		8'b01000100://O
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b0000010;
				ASCII_l = 7'b0001110;
				end
				else
				begin
				ASCII_h = 7'b0011001;
				ASCII_l = 7'b0001110;
				end
				
			end
		8'b01001101://P
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b1111000;
				ASCII_l = 7'b1000000;
				end
				else
				begin
				ASCII_h = 7'b0010010;
				ASCII_l = 7'b1000000;
				end
				
			end
		8'b00011100://A
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b0000010;
				ASCII_l = 7'b1111001;
				end
				else
				begin
				ASCII_h = 7'b0011001;
				ASCII_l = 7'b1111001;
				end
				
			end
		8'b00011011://S
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b1111000;
				ASCII_l = 7'b0110000;
				end
				else
				begin
				ASCII_h = 7'b0010010;
				ASCII_l = 7'b0110000;
				end
				
			end
		8'b00100011://D
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b0000010;
				ASCII_l = 7'b0011001;
				end
				else
				begin
				ASCII_h = 7'b0011001;
				ASCII_l = 7'b0011001;
				end
				
			end
		8'b00101011://F
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b0000010;
				ASCII_l = 7'b0000010;
				end
				else
				begin
				ASCII_h = 7'b0011001;
				ASCII_l = 7'b0000010;
				end
				
			end
		8'b00110100://G
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b0000010;
				ASCII_l = 7'b1111000;
				end
				else
				begin
				ASCII_h = 7'b0011001;
				ASCII_l = 7'b1111000;
				end
				
			end
		8'b00110011://H
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b0000010;
				ASCII_l = 7'b0000000;
				end
				else
				begin
				ASCII_h = 7'b0011001;
				ASCII_l = 7'b0000000;
				end
				
			end
		8'b00111011://J
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b0000010;
				ASCII_l = 7'b0001000;
				end
				else
				begin
				ASCII_h = 7'b0011001;
				ASCII_l = 7'b0001000;
				end
				
			end
		8'b01000010://K
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b0000010;
				ASCII_l = 7'b0000011;
				end
				else
				begin
				ASCII_h = 7'b0011001;
				ASCII_l = 7'b0000011;
				end
				
			end
		8'b01001011://L
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b0000010;
				ASCII_l = 7'b1000110;
				end
				else
				begin
				ASCII_h = 7'b0011001;
				ASCII_l = 7'b1000110;
				end
				
			end
		8'b00011010://Z
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b1111000;
				ASCII_l = 7'b0001000;
				end
				else
				begin
				ASCII_h = 7'b0010010;
				ASCII_l = 7'b0001000;
				end
				
			end
		8'b00100010://X
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b1111000;
				ASCII_l = 7'b0000000;
				end
				else
				begin
				ASCII_h = 7'b0010010;
				ASCII_l = 7'b0000000;
				end
				
			end
		8'b00100001://C
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b0000010;
				ASCII_l = 7'b0110000;
				end
				else
				begin
				ASCII_h = 7'b0011001;
				ASCII_l = 7'b0110000;
				end
				
			end
		8'b00101010://V
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b1111000;
				ASCII_l = 7'b0000010;
				end
				else
				begin
				ASCII_h = 7'b0010010;
				ASCII_l = 7'b0000010;
				end
				
			end
		8'b00110010://B
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b0000010;
				ASCII_l = 7'b0100100;
				end
				else
				begin
				ASCII_h = 7'b0011001;
				ASCII_l = 7'b0100100;
				end
				
			end
		8'b00110001://N
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b0000010;
				ASCII_l = 7'b0000110;
				end
				else
				begin
				ASCII_h = 7'b0011001;
				ASCII_l = 7'b0000110;
				end
				
			end
		8'b00111010://M
			begin
				if((!if_shift&&!if_caps)||(if_shift&&if_caps))
				begin
				ASCII_h = 7'b0000010;
				ASCII_l = 7'b0100001;
				end
				else
				begin
				ASCII_h = 7'b0011001;
				ASCII_l = 7'b0100001;
				end
				
			end
		8'b00010110://1
			begin
				if(if_shift)
				begin
				ASCII_h = 7'b0100100;
				ASCII_l = 7'b1111001;
				end
				else
				begin
				ASCII_h = 7'b0110000;
				ASCII_l = 7'b1111001;
				end
				
			end
		8'b00011110://2
			begin
			if(if_shift)
				begin
				ASCII_h = 7'b0011001;
				ASCII_l = 7'b1000000;
				end
				else
				begin
				ASCII_h = 7'b0110000;
				ASCII_l = 7'b0100100;
				end
				
			end
		8'b00100110://3
			begin
				if(if_shift)
				begin
				ASCII_h = 7'b0100100;
				ASCII_l = 7'b0110000;
				end
				else
				begin
				ASCII_h = 7'b0110000;
				ASCII_l = 7'b0110000;
				end
				
			end
		8'b00100101://4
			begin
				if(if_shift)
				begin
				ASCII_h = 7'b0100100;
				ASCII_l = 7'b0011001;
				end
				else
				begin
				ASCII_h = 7'b0110000;
				ASCII_l = 7'b0011001;
				end
			end
		8'b00101110://5
			begin
				if(if_shift)
				begin
				ASCII_h = 7'b0100100;
				ASCII_l = 7'b0010010;
				end
				else
				begin
				ASCII_h = 7'b0110000;
				ASCII_l = 7'b0010010;
				end
				
			end
		8'b00110110://6
			begin
				if(if_shift)
				begin
				ASCII_h = 7'b0010010;
				ASCII_l = 7'b0000110;
				end
				else
				begin
				ASCII_h = 7'b0110000;
				ASCII_l = 7'b0000010;
				end
				
			end
		8'b00111101://7
			begin
				if(if_shift)
				begin
				ASCII_h = 7'b0100100;
				ASCII_l = 7'b0000010;
				end
				else
				begin
				ASCII_h = 7'b0110000;
				ASCII_l = 7'b1111000;
				end
				
			end
		8'b00111110://8
			begin
				if(if_shift)
				begin
				ASCII_h = 7'b0100100;
				ASCII_l = 7'b0001000;
				end
				else
				begin
				ASCII_h = 7'b0110000;
				ASCII_l = 7'b0000000;
				end
				
			end
		8'b01000110://9
			begin
				if(if_shift)
				begin
				ASCII_h = 7'b0100100;
				ASCII_l = 7'b0000000;
				end
				else
				begin
				ASCII_h = 7'b0110000;
				ASCII_l = 7'b0010000;
				end
				
			end
		8'b01000101://0
			begin
				if(if_shift)
				begin
				ASCII_h = 7'b0100100;
				ASCII_l = 7'b0010000;
				end
				else
				begin
				ASCII_h = 7'b0110000;
				ASCII_l = 7'b1000000;
				end
				
			end
		default:
		begin
		ASCII_h = 7'b1111111;
		ASCII_l = 7'b1111111;
		end
	endcase
	end
	else
	begin
	ASCII_h = 7'b1111111;
	ASCII_l = 7'b1111111;
	PS2_h = 7'b1111111;
	PS2_l = 7'b1111111;
	end
end
endmodule

