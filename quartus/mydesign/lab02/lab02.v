module lab02(x,en,y,led,l);
	input [7:0]x;
	input en;
	output reg [6:0]led;
	output reg l;
	output reg [2:0]y;
	integer i;
	always@(x or en) begin
		l = x[0]|x[1]|x[2]|x[3]|x[4]|x[5]|x[6]|x[7];
		if(en) begin 
		y = 0;
		for(i=0;i<8;i=i+1)
			if(x[i]==1) y = i;
		end
		else y = 0;
		case (y)
		0: led = 7'b1000000;
		1: led = 7'b1111001;
		2: led = 7'b0100100;
		3: led = 7'b0110000;
		4: led = 7'b0011001;
		5: led = 7'b0010010;
		6: led = 7'b0000010;
		7: led = 7'b1111000;
		endcase
	end
endmodule 
		
			