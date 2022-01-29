module lab06_2(clk_50,cout_l,cout_h);
input clk_50;integer count_clk;reg clk_s;
reg [7:0] cout;
output reg [6:0]cout_l;
output reg [6:0]cout_h;
reg in;reg [3:0]cout_l_;reg [3:0]cout_h_;

always @(posedge clk_50)
		begin
			if(count_clk==25000000)
				begin
				count_clk =0;
				clk_s = ~clk_s;
				end
			else
				count_clk = count_clk+1;
		end
always @(posedge clk_s)
begin
	if(cout == 0) cout = 1;
	else
	begin
		in = (((cout[4]^cout[3])^cout[2])^cout[0]);
		cout = {in,cout[7:1]};
	end 
	cout_l_ = cout[3:0];
	cout_h_ = cout[7:4];
	case(cout_l_)
	0:cout_l = 7'b1000000;
	1:cout_l = 7'b1111001;
	2:cout_l = 7'b0100100;
	3:cout_l = 7'b0110000;
	4:cout_l = 7'b0011001;
	5:cout_l = 7'b0010010;
	6:cout_l = 7'b0000010;
	7:cout_l = 7'b1111000;
	8:cout_l = 7'b0000000;
	9:cout_l = 7'b0010000;
	10:cout_l = 7'b0001000;
	11:cout_l = 7'b0000011;
	12:cout_l = 7'b1000110;
	13:cout_l = 7'b0100001;
	14:cout_l = 7'b0000110;
	15:cout_l = 7'b0001110;
	endcase
	case(cout_h_)
	0:cout_h = 7'b1000000;
	1:cout_h = 7'b1111001;
	2:cout_h = 7'b0100100;
	3:cout_h = 7'b0110000;
	4:cout_h = 7'b0011001;
	5:cout_h = 7'b0010010;
	6:cout_h = 7'b0000010;
	7:cout_h = 7'b1111000;
	8:cout_h = 7'b0000000;
	9:cout_h = 7'b0010000;
	10:cout_h = 7'b0001000;
	11:cout_h = 7'b0000011;
	12:cout_h = 7'b1000110;
	13:cout_h = 7'b0100001;
	14:cout_h = 7'b0000110;
	15:cout_h = 7'b0001110;
	endcase
	end
endmodule
