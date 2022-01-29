module mydesign(clk,vga_data,h_addr,v_addr,q,pv_addr,ph_addr);
output reg [23:0]vga_data;
input [9:0] h_addr;
input [9:0] v_addr;
input clk;
input [11:0] q;
output reg [9:0]pv_addr;
output reg [9:0]ph_addr;
integer count;
reg [1:0] add_sub_flag;
wire if_in;
assign if_in = (v_addr >= pv_addr)&(v_addr < pv_addr + 99)&(h_addr >= ph_addr)&(h_addr < ph_addr + 99);
initial
begin
pv_addr = 0;ph_addr = 0;count = 0;add_sub_flag = 0;
end

always @(posedge clk)
begin
	if(if_in)//在图片显示范围内
	begin
	vga_data = {q[11:8],4'b0000,q[7:4],4'b0000,q[3:0],4'b0000};//访存读值
	end
	else
	vga_data = 24'h000000;//否则为黑色	
	if(count == 250000)//分屏器，通过count控制图片移动的速度
	begin
	count = 0;
	if(pv_addr == 0) add_sub_flag[0] = 1;//向下移动
	else add_sub_flag[0] = add_sub_flag[0];
	if(pv_addr == 379) add_sub_flag[0] = 0;//向上移动
	else add_sub_flag[0] = add_sub_flag[0];
	if(ph_addr == 0) add_sub_flag[1] = 1;//向右移动
	else add_sub_flag[1] = add_sub_flag[1];
	if(ph_addr == 539) add_sub_flag[1] = 0;//向左移动
	else add_sub_flag[1] = add_sub_flag[1];
	case(add_sub_flag)
	2'b00:
		begin
		pv_addr = pv_addr - 1;
		ph_addr = ph_addr - 1;
		end
	2'b01:
		begin
		pv_addr = pv_addr + 1;
		ph_addr = ph_addr - 1;
		end
	2'b10:
		begin
		pv_addr = pv_addr - 1;
		ph_addr = ph_addr + 1;
		end
	2'b11:
		begin
		pv_addr = pv_addr + 1;
		ph_addr = ph_addr + 1;
		end
	endcase
	end
	else count = count + 1;
end
endmodule
