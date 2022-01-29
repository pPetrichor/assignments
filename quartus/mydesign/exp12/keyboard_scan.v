module keyboard_scan(input clk, input [7:0]data, input ready, 
	output reg nextdata_n, output reg [7:0] sc, output write);
	
	parameter st = 2'b0, f0 = 2'b01, w1 = 2'b10, sh = 2'b11;
	
	wire qf0, qf1, qf2;
	reg ctrl;
	reg [7:0]P1;
	reg [7:0]P0;
	reg [1:0]state;
	assign qf0 = (P0 == 8'hf0);
	assign qf1 = (P1 == 8'hf0);
	
	assign write = (state == w1);
	
	initial
	begin
		P1 = 0;P0 = 0;state = 0;sc = 0;
	end
	
	always @(posedge clk)
	begin
		if(ready)	
		begin
			P1 <= P0;
			P0 <= data;
		end
		
		case(state)
		st:begin
				if(ready)
				begin
					if(qf0)
						state <= f0;
					else //检测当前键盘码是否为F0，如果是，跳到f0，否则跳到w1
						state <= w1;		
				end
			end
		f0:begin
				if(qf1)
					state <= st;
			end
		w1:begin
				sc <= P0;
				if(qf0)
					state <= f0;					
			end
		endcase
	end
	
endmodule 