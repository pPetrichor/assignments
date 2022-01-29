module vga_slct(
	input [9:0]h_addr,
	input [9:0]v_addr,
	input [11:0]qromascii,//ascii某一行的数据
	input [23:0]colordata,
	input [7:0]indata, //当前打算写入ram的ascii码
	input vga_clk,
	input clk_50,
	input wren,
	output [23:0]vga_data,
	output [11:0]y_addr,//哪一行
	output [2:0]coloraddr
	);
	
	reg [7:0]mytable [2099:0];//70*30的自定义字符阵
	reg [7:0]vga_ascii;//读出的vga当前应该访问的mytable内字符数据
	reg [6:0]enterpoint [29:0];//记录回车的x位置
	reg [11:0]writeaddr;//当前写下标
	reg [6:0]writex;//当前应写位置的横坐标
	reg [4:0]writey;//当前应写位置的纵坐标
	reg [11:0]pos_cnt;//行尾下标
	reg pos_flag;//下标闪烁指示符
	
	wire [3:0]vgax_cnt;//字符内x坐标
	wire [3:0]vgay_cnt;//字符内y坐标
	wire [11:0]vga_pos;//当前应访问的mytable下标
	wire [7:0]flash_pos; //记录应该闪烁的位置
	
	assign vgax_cnt = (h_addr % 9);
	assign vgay_cnt = (v_addr % 16);
	assign vga_pos = (v_addr >> 4) * 70 + (h_addr / 9);
	
	assign flash_pos = pos_flag ? 8'h5f : 0;
	
	assign y_addr = {vga_ascii[7:0], vgay_cnt[3:0]};
	assign vga_data = (qromascii[vgax_cnt - 1] == 1'b1 && h_addr < 630) ? colordata : 24'h0;
	assign coloraddr = h_addr[9:7] + v_addr[9:7];
	initial
	begin
		writeaddr = 1;
		writex = 1; writey = 0;
	end
	
	always @(posedge vga_clk) //读
	begin
		vga_ascii = mytable[vga_pos];
		if(pos_cnt == 12'hfed)
			pos_flag = ~pos_flag;
		pos_cnt = pos_cnt + 1;
	end
	
	always @(posedge clk_50)//写
	begin
		mytable[0] = 8'h24;
		if(wren)
		begin
			if(indata == 8'h0d) //enter
			begin
				enterpoint[writey] = writex;	//记录回车点
				mytable[writeaddr] = 0;	//清零光标
				writeaddr = writeaddr - writex + 70;
				mytable[writeaddr] = 8'h24;	//写入命令提示符
				writeaddr = writeaddr + 1;
				writex = 1;
				writey = writey + 1;
			end
			else if(indata == 8'h08) //delete
			begin
				if(writex == 0)//删除行首
				begin
					if(writey != 0)
					begin
						mytable[writeaddr] = 0;
						writeaddr = enterpoint[writey - 1] + writeaddr - writex - 70;
						writex = enterpoint[writey - 1];writey = writey - 1;
						mytable[writeaddr] = 0;
					end
				end
				else
				begin
					mytable[writeaddr] = 0;
					writeaddr = writeaddr - 1;
					writex = writex - 1;
					mytable[writeaddr] = 0;
				end
			end
			else
			begin
				mytable[writeaddr] = indata; 
				if(writex == 69)
				begin
					enterpoint[writey] = writex;
					writex = 0;
					if(writey == 29)
						writey = 0;
					else
						writey = writey + 1;
				end
				else
					writex = writex + 1;
					
				if(writeaddr == 2099)
					writeaddr = 0;
				else
					writeaddr = writeaddr + 1;
			end
		end
		else
			mytable[writeaddr] = flash_pos;
	end
endmodule 