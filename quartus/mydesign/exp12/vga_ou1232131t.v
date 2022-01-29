/*module vga_out(shang, colordata, valid, ascii, v_addr, h_addr, vga_clk, color, read_addr, q, vga_data);

input [23:0]colordata;
input valid;
input [7:0] ascii;
input [9:0] h_addr;
input [9:0] v_addr;
input [11:0] q;
input vga_clk;

//output reg [12:0]read_addr;
output [12:0]read_addr;
output reg [23:0]vga_data;
output reg[5:0] shang;
//output reg[2:0]color;
output [2:0]color;



//output [9:0] LEDR;
integer yushu;//避免计算除以9

reg[8:0] offset[63:0];
reg[8:0] offset_tmp[63:0];

reg[15:0] cnt;
//wire[10:0] speed;
//assign speed = 1024;
assign read_addr = (ascii << 4) + v_addr - offset[shang];
assign color = ascii[2:0];
//调整下滑：改变offset
	
always @(posedge vga_clk)
begin
//读取信息并显示
	if(h_addr < 576 && valid)
	begin
		if(yushu == 8) 
		begin 
			shang = shang + 1; 
			yushu = 0;
		end
		else 
			yushu = yushu + 1;
			
		if(v_addr - offset[shang] < 16 && ascii != 0 && h_addr > 8) 
		begin
			offset_tmp[shang] = offset_tmp[shang] + 1;
			if(offset_tmp[shang] == 511)	//sudu
			begin
				offset[shang] = offset[shang] + 1;
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
	//分数等显示
		
	end
	
	
end

endmodule */
module vga_out(vga_clk,v_addr,h_addr,addr,q,valid,vga_data);//,score_h,score_l,time_h,time_l,heart);
/*input [3:0] score_h;
input [3:0] score_l;
input [3:0] time_h;
input [3:0] time_l;*/
reg [2:0] heart;
input [9:0] h_addr;
input [9:0] v_addr;
input [11:0] q;
input vga_clk;
output reg[12:0] addr;
input valid;
reg [6:0]time_count;
integer yushu1,yushu2,yushu3;
integer shang1,shang2,shang3;
output reg[23:0]vga_data;
integer cnt;

initial
begin
heart = 3;
end

always @(posedge vga_clk)
begin
	if(cnt == 5000 & heart > 0) begin cnt = 0;heart = heart - 1;end
	else cnt = cnt + 1;
	if(h_addr >= 576 & valid)
		begin
			if(v_addr >= 80 & v_addr < 96 & h_addr >= 585 & h_addr < 630)//显示SCORE
				begin
					if(yushu1 == 8) begin yushu1 = 0;shang1 = shang1 + 1;end
					else yushu1 = yushu1 + 1;
					if(shang1 == 5) shang1 = 0;
					addr = v_addr + 80 + (shang1 << 4); //v_addr - 80 + 160
					if(q[yushu1 - 1]) vga_data = 24'h0000FF;
					else vga_data = 24'hFFFFFF;
				end
			else if(v_addr >= 240 & v_addr < 256 & h_addr >= 585 & h_addr < 621)
				begin
					if(yushu2 == 8) begin yushu2 = 0;shang2 = shang2 + 1;end
					else yushu2 = yushu2 + 1;
					if(shang2 == 4) shang2 = 0;
					addr = v_addr + (shang2 << 4); //v_addr - 240 + 240
					if(q[yushu2 - 1]) vga_data = 24'h0000FF;
					else vga_data = 24'hFFFFFF;
				end
			else if(v_addr >= 400 & v_addr < 416 & h_addr >= 585 & h_addr < 612)
				begin
					if(yushu3 == 8) begin yushu3 = 0;shang3 = shang3 + 1;end
					else yushu3 = yushu3 + 1;
					if(shang3 == 3) shang3 = 0;
					if(heart >= shang3)
					begin
						addr = v_addr - 96;
						if(q[yushu3 - 1]) vga_data = 24'hFF0000;
						else vga_data = 24'hFFFFFF;
					end
					else vga_data = 24'hFFFFFF;
				end
			else vga_data = 24'hFFFFFF;
		end
	else
	vga_data = 24'hFFFFFF;
end
endmodule
