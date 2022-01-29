/*module lab04_1_1(clk,D,R,S,en,Q1,QT1,Q2,QT2);
input D,R,S,en,clk;
output reg Q1,Q2;
output QT1,QT2;
always @(posedge clk)//D触发器设计
begin
	if(en)  Q1 = D;
	else Q1 = Q1;
end

always @(posedge clk)//RS触发器设计
begin
	if(en)
	begin
		case({R,S})
		2'b00:Q2 = Q2;
		2'b01:Q2 = 1;
		2'b10:Q2 = 0;
		2'b11:Q2 = 1'bx;
		endcase
	end
	else Q2 = Q2;
end
assign QT1 = ~Q1;
assign QT2 = ~Q2;
endmodule*/
module lab04_1_1(clk,in_d,out_q1,out_q2,clr);
	input clk;
	input in_d;
	input clr;
	output reg out_q1,out_q2;
	always@(posedge clk or posedge clr)
	begin
		if(clr)
			out_q1 = 0;
		else
			out_q1 = in_d;
	end
	always@(negedge clk)
	begin
		if(clr)
			out_q2 = 0;
		else
			out_q2 = in_d;
	end
endmodule
