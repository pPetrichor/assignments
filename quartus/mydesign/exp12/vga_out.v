module vga_out(shang, colordata, valid, ascii, v_addr, h_addr, vga_clk, color, read_addr, q, vga_data,
 bottom, score_h, score_l, pause, s_addr, s_data, time_h, time_l, heart, rq, addr);

 
input [23:0]colordata;
input valid;
input [7:0] ascii;
input [9:0] h_addr;
input [9:0] v_addr;
input [11:0] q;
input vga_clk;
input [3:0]score_h;
input [3:0]score_l;
input [3:0]time_h;
input [3:0]time_l;
input [2:0]heart;
input pause;
input [8:0]s_data;
input [11:0]rq;

output [12:0]read_addr;
output reg [23:0]vga_data;
output reg[5:0] shang;
output reg bottom;
output [2:0]color;
output [1:0]s_addr;
output reg [12:0]addr;

integer yushu;//避免计算除以9
integer yushu1,yushu2,yushu3,yushu4,yushu5;
integer shang1,shang2,shang3,shang4,shang5;

reg[8:0] offset[63:0];
reg[8:0] offset_tmp[63:0];

//reg[15:0] cnt;
//wire[10:0] speed;
//assign speed = 1024;
assign read_addr = (ascii << 4) + v_addr - offset[shang];
assign color = ascii[2:0];
assign s_addr = ascii[1:0];
//调整下滑：改变offset
	
always @(posedge vga_clk)
begin
//读取信息并显示
	if(ascii == 0)
	begin
		offset[shang] = 0;
	end
	
	if(h_addr < 576 && valid)
	begin
		if(yushu == 8) 
		begin 
			shang = shang + 1; 
			yushu = 0;
		end
		else 
			yushu = yushu + 1;
		
		if(v_addr - offset[shang] < 16 && ascii != 0) 
		begin
						/*速度的控制*/
			offset_tmp[shang] = offset_tmp[shang] + 1;
			if(offset_tmp[shang] >= s_data && pause == 0)	//达到阈值
			begin
				offset_tmp[shang] = 0;
				offset[shang] = offset[shang] + 1;
				if(offset[shang] >= 475)
				begin
					bottom = 1;
					offset[shang] = 0;
				end
				else
					bottom = 0;
			end
			
			if(q[yushu - 1]) 
				vga_data = colordata;
			else 
				vga_data = 24'h000000;//黑
		end
		else
			vga_data = 24'h000000;//黑
		
	end
	else
	begin
		if(v_addr >= 80 & v_addr < 96 & h_addr >= 585 & h_addr < 630)//显示SCORE
		begin
			if(yushu1 == 8) begin yushu1 = 0;shang1 = shang1 + 1;end
			else yushu1 = yushu1 + 1;
			if(shang1 == 5) shang1 = 0;//SCORE长度为5
			addr = v_addr + 80 + (shang1 << 4); //v_addr - 80 + 160
			if(rq[yushu1 - 1]) vga_data = 24'h0000FF;
			else vga_data = 24'hFFFFFF;
		end
		else if(v_addr >= 100 & v_addr < 116 & h_addr >= 585 & h_addr < 603)//显示分数
		begin
			if(yushu4 == 8) begin yushu4 = 0;shang4 = shang4 + 1;end
			else yushu4 = yushu4 + 1;
			if(shang4 == 2) shang4 = 0;//显示两位数
			if(shang4 == 0) addr = v_addr - 100 + (score_h << 4);//shang4为0，显示的是十位数
			else addr = v_addr - 100 + (score_l << 4);//shang4为1，显示的是个位数
			if(rq[yushu4 - 1]) vga_data = 24'hDAA520;
			else vga_data = 24'hFFFFFF;
		end
		else if(v_addr >= 240 & v_addr < 256 & h_addr >= 585 & h_addr < 621)//显示TIME
		begin
			if(yushu2 == 8) begin yushu2 = 0;shang2 = shang2 + 1;end
			else yushu2 = yushu2 + 1;
			if(shang2 == 4) shang2 = 0;//TIME长度为4
			addr = v_addr + (shang2 << 4); //v_addr - 240 + 240
			if(rq[yushu2 - 1]) vga_data = 24'h0000FF;
			else vga_data = 24'hFFFFFF;
		end
		else if(v_addr >= 260 & v_addr < 276 & h_addr >= 585 & h_addr < 603)//显示时间
		begin
			if(yushu5 == 8) begin yushu5 = 0;shang5 = shang5 + 1;end
			else yushu5 = yushu5 + 1;
			if(shang5 == 2) shang5 = 0;//显示两位数
			if(shang5 == 0) addr = v_addr - 260 + (time_h << 4);//shang5为0，显示的是十位数
			else addr = v_addr - 260 + (time_l << 4);//shang5为1，显示的是个位数
			if(rq[yushu5 - 1]) vga_data = 24'hDAA520;
			else vga_data = 24'hFFFFFF;
		end
		else if(v_addr >= 400 & v_addr < 416 & h_addr >= 585 & h_addr < 612)
		begin
			if(yushu3 == 8) begin yushu3 = 0;shang3 = shang3 + 1;end
			else yushu3 = yushu3 + 1;
			if(shang3 == 3) shang3 = 0;
			if(heart > shang3)
			begin
				addr = v_addr - 96;
				if(rq[yushu3 - 1]) vga_data = 24'hFF0000;
				else vga_data = 24'hFFFFFF;
			end
			else vga_data = 24'hFFFFFF;
		end
		else vga_data = 24'hFFFFFF;
	end
	
end

endmodule 

