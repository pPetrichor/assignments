module trans(clk_50,off,PS,ASCII,if_shift,if_caps);
input clk_50,off,if_shift,if_caps;
input [7:0]PS;
output reg [7:0] ASCII;
reg [7:0] ASCII_s [255:0];
reg [7:0] ASCII_b [255:0];

initial
begin
$readmemh("D:/quartus/mydesign/lab11_1/ascii.txt", ASCII_s, 0, 255); 
$readmemh("D:/quartus/mydesign/lab11_1/ascii_big.txt", ASCII_b, 0, 255); 
end

always @(posedge clk_50)
begin
	if(!off)
	begin
	if(if_shift^if_caps) ASCII = ASCII_b[PS];
	else ASCII = ASCII_s[PS];
	end
end
endmodule

