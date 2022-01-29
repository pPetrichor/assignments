module lab05_1(clk_50,result_ones,result_tens,led,start,stop,clear);
	input clk_50,start,stop,clear;
	reg [24:0]count_clk;
	reg clk_1s;
	reg [3:0]result_h;
	reg [3:0]result_l;
	reg [6:0]result;
	output reg [6:0]result_ones;
	output reg [6:0]result_tens;
	output reg led;
		always @(posedge clk_50)
		begin
			if(count_clk==10)
				begin
				count_clk =0;
				clk_1s = ~clk_1s;
				end
			else
				count_clk = count_clk+1;
		end
		always@(posedge clk_1s)
		begin
			if(start)
			begin
				if(clear) 
				begin
				result = 0;
				end
				else
				begin
					if(!stop)
					begin
						if(result == 99)
						begin
						result = 0;
						end
						else 
						begin
						result = result+1;
						end
					end
					else result = result;
				end
			end
			else result = 7'b1111111;//表示无效，结束	
		end
		always@(posedge clk_1s)
		begin
		if(result == 7'b1111111)
			begin
			result_h = 15;result_l = 15;
			end
		else
		begin
			if(result == 99) led = 1;
			else led = 0;
			result_l = result % 10;
			result_h = (result - result_l)/10;
		end
	
	case(result_h)
		0: result_tens = 7'b1000000;
		1: result_tens = 7'b1111001;
		2: result_tens = 7'b0100100;
		3: result_tens = 7'b0110000;
		4: result_tens = 7'b0011001;
		5: result_tens = 7'b0010010;
		6: result_tens = 7'b0000010;
		7: result_tens = 7'b1111000;
		8: result_tens = 7'b0000000;
		9: result_tens = 7'b0010000;
		15: result_tens = 7'b1111111;
		endcase
	case(result_l)
		0: result_ones = 7'b1000000;
		1: result_ones = 7'b1111001;
		2: result_ones = 7'b0100100;
		3: result_ones = 7'b0110000;
		4: result_ones = 7'b0011001;
		5: result_ones = 7'b0010010;
		6: result_ones = 7'b0000010;
		7: result_ones = 7'b1111000;
		8: result_ones = 7'b0000000;
		9: result_ones = 7'b0010000;
		15: result_ones = 7'b1111111;
		endcase
		end
endmodule

