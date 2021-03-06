module lab06_1(cin,cout,clk,load,in);
input [2:0]cin;
input clk,in;
input [7:0]load;
output reg[7:0]cout;
always @(posedge clk)
	case(cin)
	0:cout <= 0;
	1:cout <= load;
	2:cout <= {1'b0,cout[7:1]};
	3:cout <= {cout[6:0],1'b0};
	4:cout <= {cout[7],cout[7:1]};
	5:cout <= {in,cout[7:1]};
	6:cout <= {cout[0],cout[7:1]};
	7:cout <= {cout[6:0],cout[7]};
	default cout <= 8'bx;
	endcase
endmodule
