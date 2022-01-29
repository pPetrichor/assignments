module mydesign(reset,vga_clk,vga_data,h_addr,v_addr,d_data,sw1,sw2);
output reg [23:0]vga_data;
input [9:0] h_addr;
input [9:0] v_addr;
input vga_clk,reset,sw1,sw2;
input d_data;

always @(posedge reset or posedge vga_clk)
begin
	if(reset) vga_data = 24'h000000;
	else if(sw1)
	begin
	if(h_addr<80) vga_data = 24'hBA55D3;
	else if(h_addr<160) vga_data = 24'h6495ED;
	else if(h_addr<240) vga_data = 24'h00FFFF;
	else if(h_addr<320) vga_data = 24'h7FFFAA;
	else if(h_addr<400) vga_data = 24'hFFFACD;
	else if(h_addr<480) vga_data = 24'hFF8C00;
	else if(h_addr<560) vga_data = 24'hF08080;
	else if(h_addr<640) vga_data = 24'hDCDCDC;
	else  vga_data = 24'hFF00FF;
	end
	else if(sw2)
	begin 
		vga_data = {q[11:8],4'b0000,q[7:4],4'b0000,q[3:0],4'b0000};
	end
	
	else vga_data = 24'hFF0000;
end
endmodule

