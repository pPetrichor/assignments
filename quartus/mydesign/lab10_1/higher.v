module higher(clk_50,ready,data,PS,if_shift,off);
input ready,clk_50;
input [7:0] data;
output reg [7:0] PS;
reg [9:0]counter;
reg not_f0;//指示状态机下一步状态
reg next;
output reg if_shift;
reg together,if_caps;
output reg off;
reg [3:0] state;
parameter Start = 0, A = 1, B = 2, C = 3, D = 4, E = 5, F = 6, G = 7;
reg [7:0] PS_temp;

initial
begin
counter = 0;state = 0;together = 0;off = 1;if_caps = 0;if_shift = 0;
end
always @(posedge clk_50)
	begin
		if(ready)
		begin
		PS = data;
		next = 1;//next设置为1,未在一次循环中设置为0，所以松开时一直为1，可能使状态机直接想前走一个状态，应在结尾将next置零?
		//flag = 1;
		if(PS == 8'b11110000) not_f0 = 0;
		else not_f0 = 1;
		end
		else next = 0;//一开始忘记将next置为0，所以counter一直加得很快,off也会自动成为0	
	case(state)//状态机
		Start:
		begin
			if(next)
			begin
			if(PS != 8'b00010010 && PS != 8'b00010100)
			begin
			state = A;
			off = 0;
			if(counter == 99) counter = 0;
			else counter = counter + 1;
			end
			else
			begin
			PS_temp = PS;
			state = C;
			off = 1;
			if(counter == 99) counter = 0;
			else counter = counter + 1;
			end
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
			state = Start;
			off = 1;
			if(PS == 8'b01011000) if_caps = ~if_caps;
		end
		else counter = counter;
		end
		C:
		begin
		if(next)
		begin
			if(PS == PS_temp)  state = C;
			else
			begin
				state = D; off = 0;
				counter = counter + 1;
				together = 1;
				if(PS_temp == 8'b00010010) if_shift = 1;
				else if_shift = if_shift;
			end
		end
		else counter = counter;
		end
		D:
		begin
		if(next)
		begin
			if(not_f0) state = D;
			else begin state = E;off = 1;end
		end
		else counter = counter;
		end
		E:
		begin
		if(next)
		begin
			state = F;
			together = 0;
			
		end
		else counter = counter;
		end
		F:
		begin
		if(next)
		begin
			if(not_f0)
			begin
				if(PS == PS_temp) state = F;
				else 
				begin
					counter = counter + 1;
					together = 1;
					off = 0;
					state = D;
				end
			end
			else 
			begin
				state = G;
				
				off = 1;
				if(PS_temp == 8'b00010010) if_shift = 0;
				else if_shift = if_shift;
			end
		end
		else counter = counter;
		end
		G:
		begin
		if(next)
			begin state = Start;off = 1; end
		else counter = counter;
		end
		endcase
	end
endmodule