module array_ctrl(input clk, input bottom, input [5:0]vga_x, input reset, input key_en, input [7:0]key_data, input [7:0]ran_data, input [5:0]ran_x, 
	output [7:0]v_ascii, output pause, output reg [3:0]score_l, output reg[3:0]score_h, output [9:0]LEDR, output reg [2:0]heart);
	
	reg [7:0]asciiarray[63:0];	//每一列的ASCII值
	reg [5:0]letter_vaddr[25:0]; //村26个字母的列信息
	
	wire [7:0] key_tag;	//键盘字符对应的下标(0到25)
	wire [5:0] key_x;		//
	wire [7:0] ran_tag;	//随机字符对应的下标(0到25)
	
	wire p_pause;	
	reg p_score;
	
	assign v_ascii = asciiarray[vga_x];
	assign key_tag = key_data - 8'h41;
	assign key_x = letter_vaddr[key_tag];
	assign ran_tag = ran_data - 8'h41;
	
	assign p_pause = (key_data == 8'hFF);
	assign pause = p_pause | (heart == 0);
	
	assign LEDR[0] = key_en;
	assign LEDR[1] = pause;
	
	initial
	begin
		heart = 3;
	end
	
	always @(negedge clk)
	begin
		if(pause == 0)
		begin
			if(key_en)
			begin
				if(letter_vaddr[key_tag] != 0)
				begin
					asciiarray[key_x] = 8'h00;
					letter_vaddr[key_tag] = 6'b000000;
						/*   分数计算	*/
					if(p_score == 1'b0)	
					begin
						score_l = score_l + 1;
						p_score = 1'b1;
						if(score_l == 10)
						begin
							score_l = 0;
							score_h = score_h + 1;
							if(score_h == 10)
							score_h = 0;
						end	
					end
				end
			end
			else
				p_score = 1'b0;
			if(letter_vaddr[ran_tag] == 0 && key_en == 0) //不存在该字母，增加
			begin
				asciiarray[ran_x] = ran_data;
				letter_vaddr[ran_tag] = ran_x;
			end
		end
	end
	
	//减少生命
	always @(posedge bottom)
	begin
		heart = heart - 1;
	end
endmodule 