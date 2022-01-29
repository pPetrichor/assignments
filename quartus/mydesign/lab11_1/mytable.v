module mytable(valid,ASCII,clk,vga_clk,h_addr,v_addr,vga_data,off,q,addr,ready);
input valid;
input [7:0]ASCII;//键盘输入ASCII
input clk,vga_clk;
input [9:0] h_addr;
input [9:0] v_addr;
input [11:0] q;
reg [7:0] mytable [1919:0];//实现一个64*30的自定义字符阵
integer current_v,current_h;//记录当前输出到哪一行、哪一列
reg data;//一位表示黑白
output reg [23:0]vga_data;
input off;
integer paddr;//paddr = 4*v + h/9,用于查字符表，addr是找到要显示的字符后，该点在该字符中的相对位置
//addr = v % 16 + 查到字符的首地址 第h%9个值
output reg [12:0]addr;
integer shang,yushu;//避免计算除以9
reg [7:0] get_asc;
input ready;
reg [5:0]delete_pos[29:0];//记录回车点
integer counter;reg flag;

initial
begin
current_v = 0;current_h = 1;shang = 0;yushu = 0;flag = 0;counter = 0;delete_pos[0] = 0;
mytable[0] = 8'h24;
mytable[1] = 8'hAA;
end


always @(posedge vga_clk)//读字符阵，h_addr,v_addr发生变化
begin
	mytable[0] = 8'h24;
	if(counter == 5000) begin counter = 0;flag = ~flag;end
	else if(counter == 2500) begin flag = ~flag;counter = counter + 1;end
	else counter = counter + 1;
	if(valid && h_addr>=0&&h_addr < 576)
	begin
	paddr = ((v_addr >> 4) << 6) + shang;
	get_asc = mytable[paddr];
	if(get_asc == 8'hAA)
		begin
			if(flag) data = 0;
			else
			begin
				if(v_addr - ((v_addr >> 4) << 4) == 15) data = 1;
				else data = 0;
			end
		end
	else
	begin
	addr = (get_asc << 4) + (v_addr - ((v_addr >> 4) << 4));
	data = q[yushu-1];
	end
	if(data) vga_data = 24'h00FF00;//绿
	else vga_data = 24'h000000;//黑
	if(yushu == 8) begin shang = shang + 1; yushu = 0;end
	else yushu = yushu + 1;
	if(shang == 64) shang = 0;
	else shang = shang;
	end
	else vga_data = 24'h000000;
end
always @(posedge clk)//键盘写入字符阵
begin
	if(ready)
	begin
	if(off == 0)
	begin
	if(ASCII == 8'b00001101)
		begin 
		mytable[(current_v << 6) + current_h] = 8'h00;
		delete_pos[current_v] = current_h;
		current_h = 1;
		current_v = current_v + 1;
		mytable[(current_v << 6)] = 8'h24;
		mytable[(current_v << 6) + 1] = 8'hAA;
		end
	else
	begin
	if(ASCII != 8'b11111111 && ASCII != 8'b11111110 && ASCII !=8'hEC&& ASCII !=8'h08)
	//ff为shift，fe为caps,08为delete,ec为f0
	begin
	mytable[(current_v << 6) + current_h] = ASCII;
	mytable[(current_v << 6) + current_h + 1] = 8'hAA;
	if(current_h == 64) begin current_v = current_v + 1;current_h = 0;end
	if(current_h == 63)
	begin delete_pos[current_v] = 63;current_h = 0;current_v = current_v + 1;end
	else current_h = current_h + 1;
	
	end
	else if(ASCII == 8'h08)
		begin
		mytable[(current_v << 6) + current_h] = 8'h00;
		mytable[(current_v << 6) + current_h - 1] = 8'hAA;
		current_h = current_h - 1;
		if(current_h == 0&&delete_pos[current_v-1]!=63)//跨换行删除
		begin
			mytable[(current_v << 6) + current_h] = 8'h00;
			current_v = current_v - 1;
			current_h = delete_pos[current_v];
			mytable[(current_v << 6) + current_h] = 8'hAA;
		end
		if(current_h == 0&&delete_pos[current_v-1]==63)//跨行删除
		begin
			current_v = current_v - 1;
			current_h = 64;
		end
		end
	else current_v = current_v;
	end
	end
	else current_v = current_v;
	end
	else current_v = current_v;
end 
endmodule
