module random(clk_50,X,ASCII,t_h, t_l, pause);
input clk_50;
input pause;
output reg [5:0]X;
output reg [7:0]ASCII;
output reg [3:0]t_h;
output reg [3:0]t_l;

integer count;
reg clk_s;
reg[4:0] num_ASCII;
reg in;
always @(negedge clk_50)
begin
	if(count == 25000000)
	begin
		count = 0;
		clk_s = ~clk_s;
	end
	else
		count = count + 1;
end
		
always @(negedge clk_s)
begin
	if(num_ASCII == 0) num_ASCII = 1;
	else
	begin
		in = (num_ASCII[2]^num_ASCII[0]);
		num_ASCII = {in,num_ASCII[4:1]};
	end
	ASCII = 8'h41 + num_ASCII % 26;//随机生成ASCII
	if(X == 0) X = 1;
	else
	begin
		in = X[1]^X[0];
		X = {in,X[5:1]};
	end//随机生成X
	if(pause == 0)
	begin
		t_l = t_l + 1;
		if(t_l == 10)
		begin
			t_l = 0;
			t_h = t_h + 1;
			if(t_h == 10)
				t_h = 0;
		end
	end
end
endmodule
