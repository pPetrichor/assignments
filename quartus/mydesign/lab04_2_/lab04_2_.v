module lab04_2_(clk,in_d,out_q,clr);//异步清零，清零端低电平有效，时钟上升沿设置
	input clk;
	input in_d;
	input clr;
	output reg out_q;
	always@(posedge clk or negedge clr)
	begin
		if(!clr) //为什么写成if(clr)会报错？
			out_q <= 0;
		else
			out_q <= in_d;
			end
endmodule
