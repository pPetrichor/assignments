module myasynchro(clk,in_d,out_q,clr);//异步清零，清零端高电平有效，时钟上升沿设置
	input clk;
	input in_d;
	input clr;
	output reg out_q;
	always@(posedge clk or posedge clr)
	begin
		if(clr)
			out_q <= 0;
		else
			out_q <= in_d;
	end
endmodule
