module mysynchro(clk,in_d,out_q,clr);//同步清零，清零端高电平有效，时钟下降沿设置
	input clk;
	input in_d;
	input clr;
	output reg out_q;
	
	always@(negedge clk)
		if(!clr)
			out_q <= in_d;
		else
			out_q <= 0;
			
endmodule
