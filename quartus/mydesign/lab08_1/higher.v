module higher(clk_50,ready,data,PS,counter,off);//1s为一个周期
input ready,clk_50;
input [7:0] data;
output reg [7:0] PS;
output reg [9:0]counter;
reg not_f0;//指示状态机下一步状态
reg next;
output reg off;//指示按键松开
reg [1:0] state;
parameter Start = 0, A = 1, B = 2;

initial
begin
counter = 0;state = 0;off = 1;
end
always @(posedge clk_50)
	begin
		if(ready)
		begin
		PS = data;
		next = 1;
		if(PS == 8'b11110000) not_f0 = 0;
		else not_f0 = 1;
		end
		else next = 0;
		//一开始忘记将next置为0，所以counter一直加得很快,off也会自动成为0	
	
	case(state)//状态机
		Start:
		begin
			if(next) begin
			state = A;
			off = 0;
			if(counter == 99) counter = 0;
			else counter = counter + 1;
			end
			else counter = counter;
		end
		A:
		begin
		if(next)
		begin
			if(not_f0) state = A;
			else begin state = B; off = 1; end
		end
		else counter = counter;
		end
		B:
		begin
		if(next)
		begin
			state = Start; off = 1;
		end
		else counter = counter;
		end
		endcase
	end
endmodule
